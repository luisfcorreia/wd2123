# PIN_ASSIGNMENTS.md — WD2123 Emulator on STM32F411CE Black Pill

## WD2123 (40-pin DIP) → STM32F411CE Mapping

| WD2123 Pin | WD2123 Name | Direction | STM32 Pin | Notes |
|-----------|-------------|-----------|-----------|-------|
| 1         | D7          | Bidir     | PB7       | Data bus bit 7 |
| 2         | D6          | Bidir     | PB6       | Data bus bit 6 |
| 3         | D5          | Bidir     | PB5       | Data bus bit 5 |
| 4         | D4          | Bidir     | PB4       | Data bus bit 4 |
| 5         | D3          | Bidir     | PB3       | Data bus bit 3 |
| 6         | D2          | Bidir     | PB2       | Data bus bit 2 |
| 7         | D1          | Bidir     | PB1       | Data bus bit 1 |
| 8         | D0          | Bidir     | PB0       | Data bus bit 0 |
| 9         | GND         | —         | GND       | |
| 10        | CS1         | In (AL)   | PB8       | Chip select Ch A |
| 11        | CS2         | In (AL)   | PB9       | Chip select Ch B |
| 12        | CS3         | In (AL)   | PB10      | Chip select BRG |
| 13        | C/D         | In        | PB11      | HIGH=control LOW=data |
| 14        | RE          | In (AL)   | PB12      | Read enable |
| 15        | WE          | In (AL)   | PB13      | Write enable |
| 16        | MR          | In (AH)   | PB14      | Master reset |
| 17        | TXRDY-A     | Out       | PA4       | Ch A TX ready |
| 18        | RXRDY-A     | Out       | PA5       | Ch A RX ready |
| 19        | TXE-A       | Out       | PA8       | Ch A TX empty |
| 20        | CTS-A       | In (AL)   | PA6       | Ch A clear to send |
| 21        | RTS-A       | Out (AL)  | PA7       | Ch A request to send |
| 22        | RXD-A       | In        | PA10      | Ch A serial in (USART1) |
| 23        | TXD-A       | Out       | PA9       | Ch A serial out (USART1) |
| 24        | BRKDET-A    | —         | —         | Status register SR6 only |
| 25        | VCC         | —         | 3.3V      | Via level shifter |
| 26        | VCC         | —         | 3.3V      | |
| 27        | SELCLK-A    | In        | Tie HIGH  | Always use internal BRG |
| 28        | XCI/BCO-A   | —         | —         | Not implemented |
| 29        | SELCLK-B    | In        | Tie HIGH  | Always use internal BRG |
| 30        | XCI/BCO-B   | —         | —         | Not implemented |
| 31        | XTAL1       | —         | —         | No crystal required |
| 32        | XTAL2       | —         | —         | No crystal required |
| 33        | TXD-B       | Out       | PA2       | Ch B serial out (USART2) |
| 34        | RXD-B       | In        | PA3       | Ch B serial in (USART2) |
| 35        | BRKDET-B    | —         | —         | Status register SR6 only |
| 36        | RTS-B       | Out (AL)  | PA1       | Ch B request to send |
| 37        | CTS-B       | In (AL)   | PA0       | Ch B clear to send |
| 38        | TXRDY-B     | Out       | PB15      | Ch B TX ready |
| 39        | RXRDY-B     | Out       | PA15      | Ch B RX ready |
| 40        | TXE-B       | Out       | PC13      | Ch B TX empty (also onboard LED) |

AL = Active Low, AH = Active High

## Avoided Pins

| STM32 Pin | Reason |
|-----------|--------|
| PA11      | USB D- |
| PA12      | USB D+ |
| PA13      | SWDIO  |
| PA14      | SWDCLK |
| PC14      | LSE oscillator input |
| PC15      | LSE oscillator output |
