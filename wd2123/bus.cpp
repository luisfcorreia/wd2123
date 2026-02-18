#include "bus.h"
#include "pins.h"
#include "channel.h"

extern Channel ch;

static void busSetInput() {
    DATA_BUS_PORT->MODER &= ~0x0000FFFFu;
    DATA_BUS_PORT->PUPDR &= ~0x0000FFFFu;
}

static void busSetOutput() {
    uint32_t m = DATA_BUS_PORT->MODER & ~0x0000FFFFu;
    DATA_BUS_PORT->MODER = m | 0x00005555u;
}

static uint8_t busRead() {
    return (uint8_t)(DATA_BUS_PORT->IDR & DATA_BUS_MASK);
}

static void busWrite(uint8_t val) {
    uint32_t odr = DATA_BUS_PORT->ODR & ~(uint32_t)DATA_BUS_MASK;
    DATA_BUS_PORT->ODR = odr | val;
}

// Returns: 0=none, 1=ctrl, 2=data, 3=BRG rate
static uint8_t decodeSelect() {
    bool cs1 = (digitalRead(PIN_CS1) == LOW);
    bool cs3 = (digitalRead(PIN_CS3) == LOW);
    bool cd  = (digitalRead(PIN_CD)  == HIGH);

    if      (cs1 && !cs3) return cd ? 1 : 2;
    else if (!cs1 && cs3) return 3;
    return 0;
}

static void onWE() {
    uint8_t val = busRead();
    switch (decodeSelect()) {
        case 1: ch.writeControl(val);   break;
        case 2: ch.writeTxHolding(val); break;
        case 3: ch.writeRateReg(val);   break;
        default: break;
    }
}

static void onRE() {
    if (digitalRead(PIN_RE) == LOW) {
        uint8_t val = 0xFF;
        switch (decodeSelect()) {
            case 1: val = ch.readStatus();    break;
            case 2: val = ch.readRxHolding(); break;
            default: break;
        }
        busSetOutput();
        busWrite(val);
    } else {
        busSetInput();
    }
}

void busBegin() {
    busSetInput();
    pinMode(PIN_CS1, INPUT);
    pinMode(PIN_CS3, INPUT);
    pinMode(PIN_CD,  INPUT);
    pinMode(PIN_MR,  INPUT);
    pinMode(PIN_WE,  INPUT);
    pinMode(PIN_RE,  INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_WE), onWE, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_RE), onRE, CHANGE);
}

void busUpdate() {
    if (digitalRead(PIN_MR) == HIGH) {
        ch.masterReset();
    }
}
