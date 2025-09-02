#ifndef KEYPAD_H
#define KEYPAD_H
extern const char keymap[16];

#include <stdint.h>

void KEY_init(void);

uint8_t KEYPAD_Scan(uint8_t *pkey);

#endif