# WD2123 Serial Port Emulator

drop-in replacement for WD2123.

## Primary Target System


## Hardware Requirements

### Bill of Materials

### Pin Connections
See [PIN_ASSIGNMENTS.md](PIN_ASSIGNMENTS.md) for complete wiring details.

**Critical pins:**
- Data bus: PB0-PB7 (must be consecutive for parallel I/O)

**Avoid these pins:**
- PA11/PA12: USB D-/D+ (breaks serial)
- PA13/PA14: SWD programming pins
- PC14/PC15: LSE oscillator (causes system crashes)

## Software Setup

### Arduino IDE Configuration

1. **Install STM32 board support:**
   - Tools → Board Manager → Search "STM32"
   - Install "STM32 MCU based boards" by STMicroelectronics

2. **Board settings:**
   - Board: "Generic STM32F4 series" → "BlackPill F411CE"
   - USB support: "CDC (generic Serial supersede U(S)ART)"
   - Upload method: "STM32CubeProgrammer (SWD)" or your programmer

### Upload Process

**Using SWD programmer:**
1. Connect ST-Link or compatible programmer
2. Upload sketch
3. Press RESET on Black Pill

**Using DFU mode (if bootloader configured):**
1. Hold BOOT0 button
2. Press and release RESET
3. Release BOOT0
4. Upload sketch

## Project Structure

## Technical Details

## Credits

**Inspired by:**

**Hardware:**
- STM32F411 "Black Pill" by WeAct Studio

**Target System:**
- FDD 3000 floppy drive

## License

This project is provided as-is for educational and hobbyist use.

## Support

For issues, questions, or contributions, refer to the documentation files included with the project.

**Last updated:** February 2026
**Version:** 0.1 - nothing was created
