#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include "display.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

uint8_t *idxPtr;
uint16_t *valuePtr;

void initialiseScreen(uint8_t *idx, uint16_t *value)
{
    idxPtr = idx;
    valuePtr = value;

    if (!display.begin())
    {
        while (1)
        {
            Serial.println(F("SSD1306 allocation failed"));
            delay(1000);
        }
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.display();
}

void displayValue()
{
    display.setTextSize(2);
    display.setCursor(0, 0);

    if (*idxPtr < 10)
    {
        // Pad with 0 to width of 2
        display.print("0");
    }
    display.print(*idxPtr, DEC);

    display.print(": ");
    display.print(*valuePtr, DEC);
}

void drawBar()
{
    const uint8_t height = 8;
    display.drawRect(0, SCREEN_HEIGHT - height, SCREEN_WIDTH, height, SSD1306_WHITE);
    display.fillRect(0, SCREEN_HEIGHT - height, map(*valuePtr, 0, 1023, 0, SCREEN_WIDTH), height, SSD1306_WHITE);
}

void updateDisplay()
{
    display.clearDisplay();
    displayValue();
    drawBar();
    display.display();
}

#endif // DISPLAY_CPP