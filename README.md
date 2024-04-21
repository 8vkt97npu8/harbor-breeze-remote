# Harbor Breeze Remote

Replacement remote for Model TR214B (FCC# KUJCE10712). Sleeps when not in use to conserve battery life.

###Hardware:

- FireBeetle ESP32 (or similar)
- CC1101 Transceiver Module
- 5x momentary buttons
- 5x 1M pull-down resistors
- 5x 330 Ohm limiting resistors (optional)
- Lipo Battery (optional if USB powered)

Wire the buttons to the pins listed in the source code. Code expects external pull-down resistors. The external pull-downs are necessary to get the external wake to function.

Wire the CC1101 to the pins in the source code.
