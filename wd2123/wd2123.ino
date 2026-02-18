// WD2123 Dual Serial Port Emulator
// Target: STM32F411CE "Black Pill" (WeAct Studio)
// Board:  Generic STM32F4 series -> BlackPill F411CE
// USB:    CDC (generic Serial supersede U(S)ART)
// Upload: STM32CubeProgrammer (SWD)

#include <Arduino.h>
#include <stm32f4xx.h>
#include "pins.h"
#include "channel.h"
#include "bus.h"

// Channel A: USART1 (Serial1), TX=PA9, RX=PA10
// Channel B: USART2 (Serial2), TX=PA2, RX=PA3
Channel chA(Serial1, USART1,
            PIN_RXD_A, PIN_CTS_A, PIN_RTS_A,
            PIN_TXRDY_A, PIN_RXRDY_A, PIN_TXE_A);

Channel chB(Serial2, USART2,
            PIN_RXD_B, PIN_CTS_B, PIN_RTS_B,
            PIN_TXRDY_B, PIN_RXRDY_B, PIN_TXE_B);

void setup() {
    chA.begin();
    chB.begin();
    busBegin();
}

void loop() {
    busUpdate();
    chA.update();
    chB.update();
}
