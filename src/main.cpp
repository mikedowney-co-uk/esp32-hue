#include <Arduino.h>

/*
 * Read the ADC and turn a light on when it gets dark.
 * Press the button to toggle the lights.
 * Turning the light on sets the override mode so the light sensor won't turn it off.
 * Long press to exit override mode.
 * TO DO: increase the threshold before it turns the light off to stop it oscillating
 * or detect oscillations and increase the threshold when it happens?
 * (when it turns the light on and off, measure the change in intensity and set that
 * to be the threshold difference?)
 */
#include "../include/lights.h"

bool light_on = true;
bool light_override = false;
int buttonPressedFor = 0;

// Don't change the light immediately, it must be past the threshold for a few intervals.
int beenDarkFor = 0;
int beenLightFor = 0;

void setup() {
    Serial.begin(115200);
    pinMode(LED_GPIO, OUTPUT);
    pinMode(BUTTON_GPIO, INPUT);
    pinMode(STATUS_GPIO, OUTPUT);
    setup_wifi();
    light_on = read_status();
}

void loop() {
    if(button_pressed()) {
        buttonPressedFor++;
        delay(10);
        return;
    }
    if (buttonPressedFor>50){
        // Long press, disable override mode
        light_override = false;
    }else if (buttonPressedFor>0){
        // short press, toggle light and enter override mode
        light_override = true;
        light_on = !light_on;
        set_light(light_on);
    }
    buttonPressedFor = 0;

    if(!light_override){
        int light_level = read_light_sensor();
        int threshold = read_pot();
        if((light_level > threshold + 5) && light_on){
            if(beenLightFor++ > 1000){
                light_on = false;
                set_light(light_on);
                beenLightFor = 0;
            }
        }
        else if((light_level < threshold - 5) && !light_on){
            if(beenDarkFor++ > 500){
                light_on = true;
                set_light(light_on);
                beenDarkFor = 0;
            }
        }
        else {
            beenDarkFor = 0;
            beenLightFor = 0;
        }
    }

    digitalWrite(LED_GPIO, light_override);
    digitalWrite(STATUS_GPIO, light_on);
    delay(10);
}
