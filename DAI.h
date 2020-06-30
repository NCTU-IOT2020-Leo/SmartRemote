#ifndef DAI_H
#define DAI_H

#define DefaultIoTtalkServerIP "140.113.199.200"
#define DM_name "NodeMCU"
#define DF_list                                           \
    {                                                     \
        "D0~", "D1~", "D2~", "D5", "D6", "D7", "D8", "A0" \
    }
#define nODF 10 // The max number of ODFs which the DA can pull.

#include <Arduino.h>

void init_dai();
void dai_loop();
String pull(char *df_name);
int push(char *df_name, String value);

#endif

