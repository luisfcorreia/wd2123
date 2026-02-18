#include "channel.h"

#ifndef SERIAL_TX_BUFFER_SIZE
#define SERIAL_TX_BUFFER_SIZE 64
#endif

Channel::Channel(HardwareSerial &uart, USART_TypeDef *usart,
                 int pinRXD, int pinCTS, int pinRTS,
                 int pinTXRDY, int pinRXRDY, int pinTXE)
    : _uart(uart), _usart(usart),
      _pinRXD(pinRXD), _pinCTS(pinCTS), _pinRTS(pinRTS),
      _pinTXRDY(pinTXRDY), _pinRXRDY(pinRXRDY), _pinTXE(pinTXE),
      modeReg(0), commandReg(0), statusReg(0), rateReg(0),
      _nextWriteIsMode(true),
      _txHoldFull(false), _txHold(0),
      _rxHoldFull(false), _rxHold(0),
      _rxdWasLow(false), _rxdLowStart(0)
{}

void Channel::begin() {
    pinMode(_pinCTS,   INPUT);
    pinMode(_pinRTS,   OUTPUT);
    pinMode(_pinTXRDY, OUTPUT);
    pinMode(_pinRXRDY, OUTPUT);
    pinMode(_pinTXE,   OUTPUT);
    masterReset();
}

void Channel::masterReset() {
    modeReg          = 0;
    commandReg       = 0;
    rateReg          = 0;
    statusReg        = SR_TXRDY | SR_TXE;
    _nextWriteIsMode = true;
    _txHoldFull      = false;
    _txHold          = 0;
    _rxHoldFull      = false;
    _rxHold          = 0;
    _rxdWasLow       = false;

    _usart->CR1 &= ~USART_CR1_SBK;

    digitalWrite(_pinRTS,   HIGH);
    digitalWrite(_pinTXRDY, LOW);
    digitalWrite(_pinRXRDY, LOW);
    digitalWrite(_pinTXE,   HIGH);
}

void Channel::writeControl(uint8_t val) {
    if (_nextWriteIsMode) {
        modeReg          = val;
        _nextWriteIsMode = false;
        applyUartConfig();
        return;
    }

    if (val & CR_IR) {
        commandReg       = val & ~(CR_TEN | CR_REN);
        _nextWriteIsMode = true;
        refreshOutputPins();
        return;
    }

    commandReg = val;

    if (val & CR_ER) {
        statusReg &= ~(SR_PE | SR_OE | SR_FE | SR_BRK);
    }

    if (val & CR_SBK) {
        _usart->CR1 |= USART_CR1_SBK;
    }

    refreshOutputPins();
}

void Channel::writeTxHolding(uint8_t val) {
    if (commandReg & CR_LB) {
        if (_rxHoldFull) {
            statusReg |= SR_OE;
        } else {
            _rxHold     = val & dataMask();
            _rxHoldFull = true;
            statusReg  |= SR_RXRDY;
        }
        return;
    }

    _txHold     = val;
    _txHoldFull = true;
    statusReg  &= ~SR_TXRDY;
    refreshOutputPins();
}

void Channel::writeRateReg(uint8_t val) {
    rateReg = val & 0x0Fu;
    if (!_nextWriteIsMode) applyUartConfig();
}

uint8_t Channel::readRxHolding() {
    uint8_t data = _rxHold;
    _rxHold      = 0;
    _rxHoldFull  = false;
    statusReg   &= ~SR_RXRDY;
    digitalWrite(_pinRXRDY, LOW);
    return data;
}

uint8_t Channel::readStatus() {
    if (digitalRead(_pinCTS) == LOW) statusReg |=  SR_CTS;
    else                              statusReg &= ~SR_CTS;
    return statusReg;
}

void Channel::update() {
    bool ctsActive = (digitalRead(_pinCTS) == LOW);
    if (ctsActive) statusReg |=  SR_CTS;
    else           statusReg &= ~SR_CTS;

    if (commandReg & CR_LB) {
        statusReg |= (SR_TXRDY | SR_TXE);
        refreshOutputPins();
        return;
    }

    // Transmit
    if (_txHoldFull && (commandReg & CR_TEN) && ctsActive) {
        if (_uart.availableForWrite() > 0) {
            _uart.write(_txHold & dataMask());
            _txHoldFull = false;
            statusReg  |=  SR_TXRDY;
            statusReg  &= ~SR_TXE;
        }
    } else if (!_txHoldFull) {
        statusReg |= SR_TXRDY;
    }

    // TXE: hardware TC flag confirms shift register is empty
    if (!_txHoldFull && (_usart->SR & USART_SR_TC)) {
        statusReg |= SR_TXE;
    }

    // Receive
    if (commandReg & CR_REN) {
        uint32_t sr = _usart->SR;
        if (sr & USART_SR_FE)  statusReg |= SR_FE;
        if (sr & USART_SR_PE)  statusReg |= SR_PE;
        if (sr & USART_SR_ORE) statusReg |= SR_OE;

        int ch = _uart.read();
        if (ch >= 0) {
            if (_rxHoldFull) {
                statusReg |= SR_OE;
            } else {
                _rxHold     = (uint8_t)(ch & dataMask());
                _rxHoldFull = true;
                statusReg  |= SR_RXRDY;
            }
        }
    }

    // Break detection via RXD GPIO
    bool rxdLow = (digitalRead(_pinRXD) == LOW);
    if (rxdLow && !_rxdWasLow) {
        _rxdLowStart = micros();
        _rxdWasLow   = true;
    } else if (!rxdLow) {
        _rxdWasLow  = false;
        statusReg  &= ~SR_BRK;
    } else if (rxdLow && _rxdWasLow) {
        if ((uint32_t)(micros() - _rxdLowStart) >= charMicros()) {
            statusReg |= SR_BRK;
            statusReg |= SR_FE;
        }
    }

    refreshOutputPins();
}

void Channel::applyUartConfig() {
    uint32_t baud = baudRate();
    if (baud == 0) return;
    // begin() on STM32 Arduino core reinitialises cleanly without end();
    // calling end() first risks deadlock if TX buffer is not drained.
    _uart.begin(baud, serialConfig());
}

uint32_t Channel::baudRate() {
    uint8_t brf  = modeReg & (MR_B1 | MR_B2);
    uint32_t b1x = BAUD_TABLE[rateReg & 0x0Fu];
    if      (brf == MR_B1)           return b1x;
    else if (brf == MR_B2)           return b1x / 16u;
    else if (brf == (MR_B1 | MR_B2)) return b1x / 64u;
    return 0;
}

uint32_t Channel::serialConfig() {
    bool seven = (((modeReg >> 2) & 0x03u) == 0x02u);
    bool pen   = (modeReg & MR_PEN) != 0;
    bool ep    = (modeReg & MR_EP)  != 0;
    bool two   = (((modeReg >> 6) & 0x03u) == 0x03u);

    if (!pen) {
        if (seven) return two ? SERIAL_7N2 : SERIAL_7N1;
        else       return two ? SERIAL_8N2 : SERIAL_8N1;
    } else if (ep) {
        if (seven) return two ? SERIAL_7E2 : SERIAL_7E1;
        else       return two ? SERIAL_8E2 : SERIAL_8E1;
    } else {
        if (seven) return two ? SERIAL_7O2 : SERIAL_7O1;
        else       return two ? SERIAL_8O2 : SERIAL_8O1;
    }
}

uint8_t Channel::dataMask() {
    switch ((modeReg >> 2) & 0x03u) {
        case 0x00u: return 0x1Fu;
        case 0x01u: return 0x3Fu;
        case 0x02u: return 0x7Fu;
        default:    return 0xFFu;
    }
}

uint32_t Channel::charMicros() {
    uint32_t baud = baudRate();
    if (baud == 0) return 10000u;
    uint8_t databits  = (((modeReg >> 2) & 0x03u) + 5u);
    uint8_t totalbits = 1u + databits + ((modeReg & MR_PEN) ? 1u : 0u) + 1u;
    return (totalbits * 1000000uL) / baud;
}

void Channel::refreshOutputPins() {
    bool ctsActive = (digitalRead(_pinCTS) == LOW);
    bool ten       = (commandReg & CR_TEN) != 0;

    digitalWrite(_pinTXRDY, ((statusReg & SR_TXRDY) && ten && ctsActive) ? HIGH : LOW);
    digitalWrite(_pinRXRDY, (statusReg & SR_RXRDY) ? HIGH : LOW);
    digitalWrite(_pinTXE,   (statusReg & SR_TXE)   ? HIGH : LOW);
    digitalWrite(_pinRTS,   (commandReg & CR_RTS)   ? LOW  : HIGH);
}
