// WD2123 Serial Port Emulator
// Target: STM32F411CE "Black Pill" (WeAct Studio)
// Board:  Generic STM32F4 series -> BlackPill F411CE
// USB:    CDC (generic Serial supersede U(S)ART)
// Upload: STM32CubeProgrammer (SWD)

#include <Arduino.h>
#include <stm32f4xx.h>
#include "pins.h"
#include "channel.h"
#include "bus.h"

Channel ch(Serial1, USART1,
           PIN_RXD, PIN_CTS, PIN_RTS,
           PIN_TXRDY, PIN_RXRDY, PIN_TXE);

void setup() {
    ch.begin();
    busBegin();
}

void loop() {
    busUpdate();
    ch.update();
}
