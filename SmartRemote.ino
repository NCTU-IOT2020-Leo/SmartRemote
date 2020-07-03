#include <EEPROM.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "DAI.h"
#include "remoter.h"

#define EEPROM_OFFSET 64

const uint16_t kSendPin = 5;

IRsend irsend(kSendPin);
TatungAcData ac;
long cycleTimestamp;

void write_ac_data(const TatungAcData &data)
{
    EEPROM.put(EEPROM_OFFSET, data.encode());
    EEPROM.commit();
}

int read_ac_data(TatungAcData &data)
{
    uint32_t code = 0;
    EEPROM.get(EEPROM_OFFSET, code);
    return TatungAcData::decode(code, data);
}

void setup() {
    Serial.begin(115200);
    irsend.begin();

    if (read_ac_data(ac)) {
        Serial.println("Warning: Failed to read saved data!");
        ac.temperature = 27;
        ac.fan_speed = tt_fan_speed_med;
        ac.mode = tt_mode_cooling;
        ac.power = true;
    } else {
        Serial.println("Load saved data successfully!");
        ac.serial_print();
    }

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
                modified = (ac.power != false);
                ac.power = false;
            } else if (result == "1") {
                modified = (ac.power != true);
                ac.power = true;
            } else {
                Serial.print("Warning: Got invalid power state value: ");
                Serial.println(result);
            }
        }

        // Fetching temperature
        result = pull("SettingTemperature-O");
        if (result != "___NULL_DATA___") {
            int temp = result.toInt();
            if (temp >= 17 && temp <= 31) {
                modified = (ac.temperature != temp);
                ac.temperature = temp;
            } else {
                Serial.print("Warning: Got invalid temperature value: ");
                Serial.print(" ");
                Serial.print(temp);
                Serial.print(" ");
                Serial.println(result);
            }
        }

        // Fetching fan speed
        result = pull("Speed-O");
        if (result != "___NULL_DATA___") {
            int fan = result.toInt();
            switch (fan) {
                case 0:
                    modified = (ac.fan_speed != tt_fan_speed_auto);
                    ac.fan_speed = tt_fan_speed_auto;
                    break;
                case 1:
                    modified = (ac.fan_speed != tt_fan_speed_low);
                    ac.fan_speed = tt_fan_speed_low;
                    break;
                case 2:
                    modified = (ac.fan_speed != tt_fan_speed_med);
                    ac.fan_speed = tt_fan_speed_med;
                    break;
                case 3:
                    modified = (ac.fan_speed != tt_fan_speed_high);
                    ac.fan_speed = tt_fan_speed_high;
                    break;
                default:
                    Serial.print("Warning: Got invalid fan speed value: ");
                    Serial.println(result);
                    break;
            }
        }

        if (modified) {
            ac.serial_print();

            Serial.println("Sending...");
            uint32_t rawdata = ac.encode();
            Serial.print("Data: ");
            Serial.println(rawdata, HEX);
            remoter_send(irsend, rawdata);

            write_ac_data(ac);
        }
    }

    dai_loop();
    delay(50);
}
