# AI Agent Instructions for Cámara de Germinación

## Project overview
- Arduino-based seed germination chamber for automated environmental control and data logging.
- Uses an Arduino sketch located at `camara_germinacion/camara_germinacion.ino`.
- Intended functionality: read temperature and humidity, keep RTC time, log data to SD, and support actuators via relay outputs.

## Key files
- `camara_germinacion/camara_germinacion.ino` — main Arduino program.
- `README.md` — project description, hardware components, and usage context.

## Development environment
- Primary workflow is Arduino IDE.
- Open `camara_germinacion/camara_germinacion.ino` in the Arduino IDE and verify compilation for the target board (Arduino UNO / NANO).
- If available, `arduino-cli` may also be used to compile the sketch.

## Libraries and dependencies
- `Wire.h` and `SPI.h` for bus communication.
- `SD.h` for SD card logging.
- `RTClib.h` for DS3231 RTC.
- `SHT2x.h` for SHT31/SHT2x temperature and humidity sensor.

## What to focus on
- Keep the sketch compatible with Arduino Uno/Nano pin mappings and the hardware described in `README.md`.
- Preserve existing sensor initialization and error-handling behavior for SD, RTC, and SHT modules.
- Avoid adding unrelated dependencies or desktop-only code.

## Notes for AI coding agents
- This repository is small and hardware-centric; there are no unit tests or CI config files.
- Prefer minimal edits and keep the logic readable for someone validating the hardware operation in a physical setup.
- Do not assume the presence of a computer-based test harness beyond Arduino IDE compilation.
