#include "bus.h"
#include "pins.h"
#include "channel.h"

extern Channel chA;
extern Channel chB;

// ---------------------------------------------------------------------------
// Data bus helpers (PB0:PB7 via GPIOB direct register access)
// ---------------------------------------------------------------------------

static void busSetInput() {
    DATA_BUS_PORT->MODER  &= ~0x0000FFFFu;   // 00 = input
    DATA_BUS_PORT->PUPDR  &= ~0x0000FFFFu;   // no pull
}

static void busSetOutput() {
    uint32_t m = DATA_BUS_PORT->MODER & ~0x0000FFFFu;
    DATA_BUS_PORT->MODER  = m | 0x00005555u; // 01 = GP output, 2 bits per pin
}

static uint8_t busRead() {
    return (uint8_t)(DATA_BUS_PORT->IDR & DATA_BUS_MASK);
}

static void busWrite(uint8_t val) {
    uint32_t odr = DATA_BUS_PORT->ODR & ~(uint32_t)DATA_BUS_MASK;
    DATA_BUS_PORT->ODR = odr | val;
}

// ---------------------------------------------------------------------------
// Chip-select decode
//   Returns: 0=none, 1=chA ctrl, 2=chA data, 3=chB ctrl, 4=chB data,
//            5=BRG-A rate, 6=BRG-B rate
// ---------------------------------------------------------------------------

static uint8_t decodeSelect() {
    bool cs1 = (digitalRead(WD_CS1) == LOW);
    bool cs2 = (digitalRead(WD_CS2) == LOW);
    bool cs3 = (digitalRead(WD_CS3) == LOW);
    bool cd  = (digitalRead(WD_CD)  == HIGH);

    if      (cs1 && !cs2 && !cs3) return cd ? 1 : 2;
    else if (!cs1 && cs2 && !cs3) return cd ? 3 : 4;
    else if (!cs1 && !cs2 && cs3) return cd ? 5 : 6;
    return 0;
}

// ---------------------------------------------------------------------------
// WE ISR — triggered on falling edge of WD_WE
// ---------------------------------------------------------------------------

static void onWE() {
    uint8_t val = busRead();
    uint8_t sel = decodeSelect();

    switch (sel) {
        case 1: chA.writeControl(val);   break;
        case 2: chA.writeTxHolding(val); break;
        case 3: chB.writeControl(val);   break;
        case 4: chB.writeTxHolding(val); break;
        case 5: chA.writeRateReg(val);   break;
        case 6: chB.writeRateReg(val);   break;
        default: break;
    }
}

// ---------------------------------------------------------------------------
// RE ISR — triggered on CHANGE of WD_RE
//   Falling edge: drive data onto bus
//   Rising edge:  release bus back to input
// ---------------------------------------------------------------------------

static void onRE() {
    if (digitalRead(WD_RE) == LOW) {
        // Falling edge: host is reading — put data on bus
        uint8_t sel = decodeSelect();
        uint8_t val = 0xFF;

        switch (sel) {
            case 1: val = chA.readStatus();    break;
            case 2: val = chA.readRxHolding(); break;
            case 3: val = chB.readStatus();    break;
            case 4: val = chB.readRxHolding(); break;
            default: break;
        }

        busSetOutput();
        busWrite(val);
    } else {
        // Rising edge: host done reading — release bus
        busSetInput();
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void busBegin() {
    busSetInput();

    pinMode(WD_CS1, INPUT);
    pinMode(WD_CS2, INPUT);
    pinMode(WD_CS3, INPUT);
    pinMode(WD_CD,  INPUT);
    pinMode(WD_MR,  INPUT);
    pinMode(WD_WE,  INPUT);
    pinMode(WD_RE,  INPUT);

    attachInterrupt(digitalPinToInterrupt(WD_WE), onWE, FALLING);
    attachInterrupt(digitalPinToInterrupt(WD_RE), onRE, CHANGE);
}

void busUpdate() {
    if (digitalRead(WD_MR) == HIGH) {
        chA.masterReset();
        chB.masterReset();
    }
}
