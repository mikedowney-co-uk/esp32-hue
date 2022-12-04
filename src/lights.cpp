#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../include/lights.h"

// WIFI credentials
const char *ssid = "wifi ssid goes here";
const char *password = "wifi password goes here";

const char *ask = "http://192.168.0.xx/api/API_ID_CODE/lights/LIGHT_ID";
const char *post = "http://192.168.0.xx/api/API_ID_CODE/lights/LIGHT_ID/state";


void setup_wifi() {
    bool weAreConnected = false;
    do {
        weAreConnected = connectWifi();
    } while (!weAreConnected);
}

bool connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(10);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    bool led = true;
    int attempts = 20;
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        digitalWrite(LED_GPIO, led);
        led = !led;
        Serial.print(".");
        if (attempts-- <= 0) {
            Serial.println();
            Serial.println("Failed to connect");
            delay(1000);
            return false;
        }
    }
    digitalWrite(STATUS_GPIO, true);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(50);
    return true;
}

void set_light(bool light_on) {
    Serial.print("Turning light ");
    Serial.println(light_on ? "on" : "off");
    send_light_command(light_on, 254);
}

void set_brightness(int brightness) {
    if (brightness == 0) {
        send_light_command(false, 0);
    } else {
        send_light_command(true, brightness);
    }
}

void send_light_command(bool light_on, int brightness) {
    if (brightness > 254) { brightness = 254; }
    if (brightness < 1) { brightness = 1; }

    HTTPClient http;
    http.begin(post);
    String body;
    if (brightness < 1 || !light_on) {
        body = String("{\"on\":false}");
    } else {
        body = String("{\"on\":true,\"bri\":");
        body.concat(brightness);
        body.concat("}");
    }
    int httpResponseCode = http.PUT(body);
    Serial.println("Sending:");
    Serial.println(body);
    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
    }
    http.end();
}

bool read_status() {
    HTTPClient http;
    http.begin(ask);
    bool light_status = false;
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        if (payload.startsWith("{\"state\":{\"on\":true")) {
            light_status = true;
            Serial.println("True");
        }
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
    return light_status;
}
