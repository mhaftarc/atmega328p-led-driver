# ATmega328P LED Controller

Small embedded C project for the ATmega328P. The goal was to practice working with AVR peripherals, interrupts and non-blocking program logic.

The LED can be controlled using two buttons or commands sent over UART.


## Demo

Youtube link: https://youtu.be/jFmK18iZMQg
## What it does

The controller has 5 modes:

- `OFF`
- `ON`
- `SLOW` - switches every 500 ms
- `STROBE` - switches every 50 ms
- `BREATHING` - gradually increases and decreases brightness

Brightness is set using an analog input and the LED is driven using PWM.

Buttons use Pin Change Interrupts and a simple software debounce timer.

UART commands:

```text
ON
OFF
SLOW
STROBE
BREATHING
```

Commands are terminated with a newline.

## What I used

- ATmega328P
- C
- PlatformIO
- AVR interrupts
- Timer0 (CTC)
- Timer1 (PWM)
- ADC
- UART
- Watchdog Timer
- Git

## Project structure

```text
src/
├── adc.c / adc.h
├── buttons.c / buttons.h
├── commands.c / commands.h
├── led.c / led.h
├── main.c
├── timer.c / timer.h
├── uart.c / uart.h
└── watchdog.c / watchdog.h
```

The project is split into separate modules instead of keeping all of the firmware in `main.c`.

`main.c` initializes the peripherals and runs the main loop. Time-dependent LED modes use the millisecond counter from Timer0 instead of `_delay_ms()`.

## Notes

The buttons are connected to `PC0` and `PC1` and are handled using the `PCINT1` interrupt group.

The original Arduino pins used for the buttons were physically damaged, so I moved the buttons to the `PC0`/`PC1` pins and changed the implementation from external interrupts to Pin Change Interrupts.

The UART code is implemented, but the Arduino board I used has damaged pins 1-7, so physical UART testing is currently limited.

## Build

The project can be built with PlatformIO.

The current build uses approximately:

- Flash: 4.8%
- RAM: 7.0%
- Compiler warnings: 0

## Possible next steps

- Improve UART input handling
- Add better command/error handling
- Store settings in EEPROM
- Add more LED modes
- Test UART using an external USB-UART adapter
