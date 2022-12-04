//
// Created by Mike Downey on 04/12/2022.
//

#ifndef HUE_LIGHTS_LIGHTS_H
#define HUE_LIGHTS_LIGHTS_H

#define LIGHT_SENSOR_GPIO 36
#define POTENTIOMETER_GPIO 32
#define BUTTON_GPIO 17
#define LED_GPIO 26
#define STATUS_GPIO 18

// From hardware.cpp
int read_light_sensor();
bool button_pressed();
int read_pot();

//from wifi.cpp
void setup_wifi();
void set_light(bool light_on);
bool read_status();
bool connectWifi();
void send_light_command(bool light_on, int brightness);


#endif //HUE_LIGHTS_LIGHTS_H
