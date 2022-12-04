# esp32-hue
Remote light switch using an ESP32 to control Philips Hue lights

Circuit diagram to follow...

## Usage

- The potentiometer adjusts the sensitivity. The light will automatically switch on when the light level falls below the threshold. 
- A short press on the button toggles the light on and off, and puts it in 'override' mode
- When in override mode, it ignores any changes in light level
- A long press (about a second) takes it out of override mode
- LED 1 (STATUS_GPIO) indicates whether the light is on or not and briefly comes on when wifi connects.
- LED 2 (LED_GPIO) flashes while it is connecting to wifi, and indicates 'override' mode
