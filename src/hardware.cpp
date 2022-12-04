#include <Arduino.h>
#include "../include/lights.h"

// Returns value in the range 0-255
int read_light_sensor(){
    return analogRead(LIGHT_SENSOR_GPIO) / 16;
}

bool button_pressed(){
    return !digitalRead(BUTTON_GPIO);
}

int read_pot(){
    return analogRead(POTENTIOMETER_GPIO) / 16; // 12 bit to 8 bit
}
