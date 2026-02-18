#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <stm32f4xx.h>  // USART_TypeDef — available in STM32 Arduino core CMSIS headers

// Mode Register bit masks (first write after reset or IR command)
#define MR_B1   0x01u  // Baud rate factor bit 0
#define MR_B2   0x02u  // Baud rate factor bit 1 (00=undef 01=1X 10=16X 11=64X)
#define MR_L1   0x04u  // Character length bit 0
#define MR_L2   0x08u  // Character length bit 1 (00=5 01=6 10=7 11=8)
#define MR_PEN  0x10u  // Parity enable
#define MR_EP   0x20u  // Even parity (0=odd)
#define MR_S1   0x40u  // Stop bits bit 0
#define MR_S2   0x80u  // Stop bits bit 1 (01=1 10=1.5 11=2)

// Command Register bit masks (subsequent writes)
#define CR_TEN  0x01u  // Transmit enable
#define CR_DTR  0x02u  // Data Terminal Ready output (tied internally, exposed via SR)
#define CR_REN  0x04u  // Receive enable
#define CR_SBK  0x08u  // Send break
#define CR_ER   0x10u  // Error reset (clears PE/OE/FE/BRK)
#define CR_RTS  0x20u  // RTS: 1=assert (pin LOW), 0=deassert (pin HIGH)
#define CR_IR   0x40u  // Internal reset — next control write goes to mode reg
#define CR_LB   0x80u  // Local loopback (diagnostic)

// Status Register bit masks (read-only)
#define SR_TXRDY 0x01u  // TX holding register empty
#define SR_RXRDY 0x02u  // RX holding register has data
#define SR_TXE   0x04u  // TX shift register empty (line idle)
#define SR_PE    0x08u  // Parity error
#define SR_OE    0x10u  // Overrun error
#define SR_FE    0x20u  // Framing error
#define SR_BRK   0x40u  // Break detected
#define SR_CTS   0x80u  // CTS pin active (pin LOW = bit HIGH)

// Serial config constants — defined by STM32 Arduino core but guarded;
// provide fallbacks so this compiles on any core version.
#ifndef SERIAL_8N1
#define SERIAL_8N1 0x06
#endif
#ifndef SERIAL_8N2
#define SERIAL_8N2 0x0E
#endif
#ifndef SERIAL_7N1
#define SERIAL_7N1 0x04
#endif
#ifndef SERIAL_7N2
#define SERIAL_7N2 0x0C
#endif
#ifndef SERIAL_8E1
#define SERIAL_8E1 0x26
#endif
#ifndef SERIAL_8E2
#define SERIAL_8E2 0x2E
#endif
#ifndef SERIAL_7E1
#define SERIAL_7E1 0x24
#endif
#ifndef SERIAL_7E2
#define SERIAL_7E2 0x2C
#endif
#ifndef SERIAL_8O1
#define SERIAL_8O1 0x36
#endif
#ifndef SERIAL_8O2
#define SERIAL_8O2 0x3E
#endif
#ifndef SERIAL_7O1
#define SERIAL_7O1 0x34
#endif
#ifndef SERIAL_7O2
#define SERIAL_7O2 0x3C
#endif

// Baud rate table — 1X factor, index = rateReg[3:0] (Table 5 of datasheet)
static const uint32_t BAUD_TABLE[16] = {
    800, 1200, 1760, 2150, 2400, 3200, 4800, 9600,
    19200, 28800, 38400, 57600, 76800, 115200, 153600, 307200
};

class Channel {
public:
    Channel(HardwareSerial &uart, USART_TypeDef *usart,
            int pinRXD, int pinCTS, int pinRTS,
            int pinTXRDY, int pinRXRDY, int pinTXE);

    void    begin();
    void    masterReset();

    // Bus write handlers — called from WE ISR (must be short)
    void    writeControl(uint8_t val);
    void    writeTxHolding(uint8_t val);
    void    writeRateReg(uint8_t val);

    // Bus read handlers — called from RE ISR (must be short)
    uint8_t readRxHolding();
    uint8_t readStatus();

    // Called every loop() iteration
    void    update();

    // Register state (public for ISR access)
    volatile uint8_t modeReg;
    volatile uint8_t commandReg;
    volatile uint8_t statusReg;
    volatile uint8_t rateReg;

private:
    HardwareSerial &_uart;
    USART_TypeDef  *_usart;
    int _pinRXD, _pinCTS, _pinRTS;
    int _pinTXRDY, _pinRXRDY, _pinTXE;

    volatile bool    _nextWriteIsMode;
    volatile bool    _txHoldFull;
    volatile uint8_t _txHold;
    volatile bool    _rxHoldFull;
    volatile uint8_t _rxHold;

    bool     _rxdWasLow;
    uint32_t _rxdLowStart;

    void     applyUartConfig();
    void     refreshOutputPins();
    uint32_t baudRate();
    uint32_t serialConfig();
    uint8_t  dataMask();
    uint32_t charMicros();
};
