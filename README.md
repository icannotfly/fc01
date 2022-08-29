# fc01
icannotfly's Rocket Flight Computer mk1



## Capabilities
- Can track
  - Temperature
  - Pressure
  - Altitude
- Logs tracked data 

## Hardware Required
- [Adafruit Feather 32u4 Adalogger](https://www.adafruit.com/product/2795)
- [Adafruit DPS310 Barometer](https://www.adafruit.com/product/4494) (added in 0.1.0)
- A micro SD card (for storing logs) (addeed in 0.2.0)

## Data Logged
- Time (ms) since boot
- Temperature (C)
- Pressure (mbar)
- Altitude (m)

## Error Codes
The on-board red LED will blink to indicate errors. 
Blinks | Error | Troubleshooting
:-:|---|---
1 | Barometer not initialized | Make sure the wiring between the barometer and the Adalogger is good.
2 | SD not initialized | Make sure the SD card reader is not damaged and that the SD card is plugged in and is not damaged.
3 | No R/W access to SD card | Make sure card has not wiggled loose. Remove and re-seat the SD card.
