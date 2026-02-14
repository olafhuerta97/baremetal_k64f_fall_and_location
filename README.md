# Bare-metal K64F Fall Detection and Location System

This repository contains a **two-node embedded system** developed on the **NXP FRDM-K64F (MK64FN1M0)** board to detect emergency events and transmit patient location/event data over radio.

The project was originally built as an academic project at Tecnológico de Monterrey and is implemented fully in **bare-metal C** (no high-level embedded framework).

## Project overview

The system is split into two firmware applications:

- **Tx (Transmitter)**
  - Reads sensors and serial GPS data.
  - Detects emergency conditions (fall/zone alert).
  - Packs and transmits event payloads through an nRF24L01 module.

- **Rx (Receiver)**
  - Listens for nRF24L01 packets.
  - Displays/stores event information.
  - Generates acoustic alerts for different event categories.

## Repository structure

- `Tx/` → transmitter firmware project.
- `Rx/` → receiver firmware project.
- Each side includes:
  - `Sources/` for application/peripheral code.
  - `Project_Headers/` for register/device headers.
  - `Project_Settings/` for linker/startup/debug settings.
  - `FLASH/` and `RAM/` auto-generated build folders.

## Hardware used

- NXP Kinetis **K64F** MCU (FRDM-K64F platform)
- **nRF24L01** transceiver (Tx/Rx link)
- GPS receiver (UART input, transmitter side)
- On-board/connected accelerometer (event trigger)
- Receiver-side peripherals:
  - 16x2 LCD
  - Buzzer/tone output using timer output compare
  - External I2C EEPROM for event storage

## Software architecture highlights

### Transmitter (`Tx`)

- Initializes PIT, ADC, UART, SPI, I2C, accelerometer, and low-power timer.
- Parses GPS data and periodically computes averaged latitude/longitude values.
- Uses interrupt-driven event triggers (accelerometer and RF status).
- Sends 5-byte event payloads repeatedly while an event flag is active.

### Receiver (`Rx`)

- Initializes LCD, RF receiver, timers, I2C memory, and GPIO button interrupts.
- Receives 5-byte RF payloads, decodes date/time/event information.
- Saves events to external memory and allows navigation through stored records.
- Generates different tone patterns for different high-level alert types.

## Build notes

This codebase uses **auto-generated GNU Make projects** (from an NXP/CodeWarrior-style toolchain setup).

### Typical generated targets

- `Tx/FLASH/makefile` builds `DispositivoMovil.elf`
- `Rx/FLASH/makefile` builds `nrf24l01rx.elf`

### Toolchain expectations

The generated makefiles reference environment variables such as:

- `ARMSourceryDirEnv`
- `GNU_Make_Install_DirEnv`

So building from CLI requires a compatible ARM GCC + make setup with those variables configured the same way as the original IDE/toolchain.

## Documentation links

- Project presentation (EN):
  - https://docs.google.com/presentation/d/1O1mpz2Zpg-PNperZPEnL-2zeOvIShckMjRdAIiNy-4w/edit?usp=sharing
- Project presentation (ES):
  - https://docs.google.com/presentation/d/1DdGcfKBSRmUkOVxrM-Q6-cPUrZTwif7XFlaJo-RJAoQ/edit?usp=sharing

## License

No explicit license file is included in this repository. If you plan to reuse or distribute this work, add a license or confirm permission with the repository owner.
