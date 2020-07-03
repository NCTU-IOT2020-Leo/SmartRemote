#ifndef REMOTER_H
#define REMOTER_H

#include <stdint.h>
#include <IRsend.h>

// Fan Speed
const uint8_t tt_fan_speed_auto = 0b000;
const uint8_t tt_fan_speed_low = 0b001;
const uint8_t tt_fan_speed_med = 0b010;
const uint8_t tt_fan_speed_high = 0b100;
// Fuzzy
const uint8_t tt_fuzzy_awful = 0b01;
const uint8_t tt_fuzzy_smile = 0b10;
const uint8_t tt_fuzzy_angry = 0b11;
// Timer Hour
const uint8_t tt_timer_half = 0b1111;
// Timer Action
const bool tt_timer_act_on = true;
const bool tt_timer_act_off = false;
// Mode
const uint8_t tt_mode_auto = 0b00;
const uint8_t tt_mode_cooling = 0b01;
const uint8_t tt_mode_dehumid = 0b10;
const uint8_t tt_mode_fan = 0b11;

struct TatungAcData {
    uint8_t temperature;
    uint8_t fan_speed;
    bool sterilization;
    uint8_t fuzzy;
    uint8_t timer_hour;
    bool timer_enabled;
    bool timer_action;
    bool sleeping;
    bool rhythm;
    uint8_t mode;
    bool power;

    TatungAcData();

    void serial_print() const;
    uint32_t encode() const;
    static int decode(uint32_t code, TatungAcData &data);
};

void remoter_send(IRsend &ir, uint32_t data);

#endif
