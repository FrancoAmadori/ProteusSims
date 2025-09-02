
#ifndef _UTILS_H
#define _UTILS_H
#include "rtc.h"
#include <string.h>
uint8_t is_valid_time(const char *str, rtc_t *alarm);

uint8_t isdigit(char c);

uint8_t is_valid_date(const char *date_str);

#endif