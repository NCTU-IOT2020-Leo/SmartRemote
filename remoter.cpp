#include "remoter.h"
#include <IRsend.h>
#include <IRutils.h>

using irutils::setBit;
using irutils::setBits;

const int R_HDR_MARK    = 3850;
const int R_HDR_GAP     = 2175;
const int R_BIT_MARK    = 850;
const int R_ZERO_GAP    = 375;
const int R_ONE_GAP     = 1600;
const int R_MSG_GAP     = 8250;

const int R_MSG_BITS    = 31;
const int R_FREQ        = 38;   // 38kHZ

void remoter_send(IRsend &ir, uint32_t data)
{
    ir.sendGeneric(
        R_HDR_MARK,
        R_HDR_GAP,
        R_BIT_MARK,
        R_ONE_GAP,
        R_BIT_MARK,
        R_ZERO_GAP,
        R_BIT_MARK,
        R_MSG_GAP,
        data,
        R_MSG_BITS,
        R_FREQ,
        false,
        1,
        kDutyDefault
    );
}

TatungAcData::TatungAcData()
{
    this->temperature = 25;
    this->fan_speed = tt_fan_speed_auto;
    this->sterilization = false;
    this->fuzzy = tt_fuzzy_smile;
    this->timer_hour = 0;
    this->timer_enabled = false;
    this->timer_action = tt_timer_act_off;
    this->sleeping = false;
    this->rhythm = false;
    this->mode = tt_mode_auto;
    this->power = false;
}

uint32_t TatungAcData::encode() const
{
    uint32_t data;
    setBits(&data, 26, 5, this->temperature);
    setBits(&data, 23, 3, this->fan_speed);
    setBits(&data, 20, 3, 0b111);
    setBit(&data, 19, this->sterilization);
    setBits(&data, 17, 2, 0);
    setBits(&data, 15, 2, this->fuzzy);
    setBits(&data, 11, 4, this->timer_hour);
    setBit(&data, 10, this->timer_enabled);
    setBit(&data, 9, this->timer_action);
    setBit(&data, 8, this->sleeping);
    setBit(&data, 7, this->rhythm);
    setBits(&data, 5, 2, 0);
    setBits(&data, 3, 2, this->mode);
    setBits(&data, 2, 1, 1);
    setBit(&data, 1, this->power);
    setBits(&data, 0, 1, 0);
    return data;
}
