#pragma once

// Data bus — PB0:PB7 (must be consecutive for parallel I/O via GPIOB->MODER)
#define DATA_BUS_PORT  GPIOB
#define DATA_BUS_MASK  0x00FFu

// Host control inputs (active-low unless noted)
#define PIN_CS1  PB8   // Active LOW — chip select
#define PIN_CS3  PB10  // Active LOW — selects Baud Rate Generator reg
#define PIN_CD   PB9   // HIGH = control/status register, LOW = data register
#define PIN_RE   PB12  // Active LOW — Read Enable
#define PIN_WE   PB13  // Active LOW — Write Enable
#define PIN_MR   PB14  // Active HIGH — Master Reset

// Channel (USART1 — PA9/PA10)
#define PIN_TXD    PA9
#define PIN_RXD    PA10
#define PIN_CTS    PA6   // Active LOW input
#define PIN_RTS    PA7   // Active LOW output
#define PIN_TXRDY  PA4   // HIGH = TX holding register empty
#define PIN_RXRDY  PA5   // HIGH = RX data available
#define PIN_TXE    PA8   // HIGH = TX shift register empty (line idle)

// Pins not wired — emulation only:
//   BRKDET : reflected in SR6 only
//   SELCLK : tie HIGH on PCB (always use internal BRG)
//   XCI/BCO: not implemented
//   XTAL1/2: no crystal required (software BRG)
