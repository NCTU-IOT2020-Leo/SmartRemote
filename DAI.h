#ifndef DAI_H
#define DAI_H

#define DefaultIoTtalkServerIP "140.113.199.205"
#define DM_name "SimpleAirConditioner"
#define DF_list                                           \
    {                                                     \
        "SettingTemperature-O", "Speed-O", "Switch-O" \
    }
#define nODF 3 // The max number of ODFs which the DA can pull.

#include <Arduino.h>

void init_dai();
void dai_loop();
String pull(char *df_name);
int push(char *df_name, String value);

#endif

