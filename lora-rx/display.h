#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initialiseScreen(uint8_t *idx, uint16_t *value);
void updateDisplay();

#endif // DISPLAY_H