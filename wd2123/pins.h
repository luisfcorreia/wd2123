#pragma once

// Data bus — PB0:PB7 (must be consecutive for parallel I/O via GPIOB->MODER)
#define DATA_BUS_PORT  GPIOB
#define DATA_BUS_MASK  0x00FFu

// Host control inputs (active-low unless noted)
#define WD_CS1  PB8   // Active LOW — selects Channel A
#define WD_CS2  PB9   // Active LOW — selects Channel B
#define WD_CS3  PB10  // Active LOW — selects Baud Rate Generator regs
#define WD_CD   PB11  // HIGH = control/status register, LOW = data register
#define WD_RE   PB12  // Active LOW — Read Enable
#define WD_WE   PB13  // Active LOW — Write Enable
#define WD_MR   PB14  // Active HIGH — Master Reset

// Channel A (USART1 — PA9/PA10)
#define WD_TXD_A    PA9
#define WD_RXD_A    PA10
#define WD_CTS_A    PA6   // Active LOW input
#define WD_RTS_A    PA7   // Active LOW output
#define WD_TXRDY_A  PA4   // HIGH = TX holding register empty
#define WD_RXRDY_A  PA5   // HIGH = RX data available
#define WD_TXE_A    PA8   // HIGH = TX shift register empty (line idle)

// Channel B (USART2 — PA2/PA3)
#define WD_TXD_B    PA2
#define WD_RXD_B    PA3
#define WD_CTS_B    PA0   // Active LOW input
#define WD_RTS_B    PA1   // Active LOW output
#define WD_TXRDY_B  PB15  // HIGH = TX holding register empty
#define WD_RXRDY_B  PA15  // HIGH = RX data available
#define WD_TXE_B    PC13  // HIGH = TX shift register empty
                           // Note: PC13 also drives the onboard LED (active LOW);
                           // the LED will illuminate briefly during Channel B TX.

// Pins not wired — emulation only:
//   BRKDET-A/B  : reflected in SR6 only
//   SELCLK-A/B  : tie HIGH on PCB (always use internal BRG)
//   XCI/BCO-A/B : not implemented (no external clock input/output)
//   XTAL1/XTAL2 : no crystal required (software BRG)


