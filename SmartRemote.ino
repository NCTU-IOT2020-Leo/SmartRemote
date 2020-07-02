#include <EEPROM.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "DAI.h"
#include "remoter.h"

const uint16_t kSendPin = 5;

IRsend irsend(kSendPin);
TatungAcData ac;
long cycleTimestamp;

void setup() {
    Serial.begin(115200);
    irsend.begin();

    ac.temperature = 27;
    ac.fan_speed = tt_fan_speed_med;
    ac.mode = tt_mode_cooling;
    ac.power = false;

    init_dai();
    cycleTimestamp = millis();
}

void loop() {
    if (millis() - cycleTimestamp > 1000) {
        String result;
        bool modified = false;

        // Fetching power
        result = pull("Switch-O");
        if (result != "___NULL_DATA___") {
            result.trim();
            if (result == "0") {
                modified = true;
                ac.power = false;
            } else if (result == "1") {
                modified = true;
                ac.power = true;
            } else {
                Serial.println("Warning: Got invalid power state value!");
            }
        }

        // Fetching temperature
        result = pull("SettingTemperature-O");
        if (result != "___NULL_DATA___") {
            int temp = result.toInt();
            if (temp >= 16 && temp < 32) {
                modified = true;
                ac.temperature = temp;
            } else {
                Serial.println("Warning: Got invalid temperature value!");
            }
        }

        // Fetching fan speed
        result = pull("Speed-O");
        if (result != "___NULL_DATA___") {
            int fan = result.toInt();
            switch (fan) {
                case 0:
                    ac.fan_speed = tt_fan_speed_auto;
                    modified = true;
                    break;
                case 1:
                    ac.fan_speed = tt_fan_speed_low;
                    modified = true;
                    break;
                case 2:
                    ac.fan_speed = tt_fan_speed_med;
                    modified = true;
                    break;
                case 3:
                    ac.fan_speed = tt_fan_speed_high;
                    modified = true;
                    break;
                default:
                    Serial.println("Warning: Got invalid fan speed value!");
                    break;
            }
        }

        if (modified) {
            Serial.println("Current State:");
            Serial.print("\tPower: ");
            Serial.println(ac.power);
            Serial.print("\tTemperature: ");
            Serial.println(ac.temperature);
            Serial.print("\tFan Speed: ");
            Serial.println(ac.fan_speed);

            Serial.println("Sending...");
            uint32_t rawdata = ac.encode();
            Serial.print("Data: ");
            Serial.println(rawdata, HEX);
            remoter_send(irsend, rawdata);
        }
    }

    dai_loop();
    delay(50);
}
