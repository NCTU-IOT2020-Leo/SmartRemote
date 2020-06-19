#include <EEPROM.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "remoter.h"

const uint16_t kSendPin = 5;
uint32_t iter = 0;

IRsend irsend(kSendPin);

void setup() {
    Serial.begin(115200);
    irsend.begin();
}

void loop() {
    TatungAcData ac;
    ac.temperature = 23 + (iter % 4);
    ac.fan_speed = tt_fan_speed_high;
    ac.mode = tt_mode_cooling;
    ac.power = true;

    Serial.println("Sending...");
    uint32_t rawdata = ac.encode();
    Serial.print("Data: ");
    Serial.println(rawdata, HEX);
    remoter_send(irsend, rawdata);
    iter++;
    delay(5000);
}
