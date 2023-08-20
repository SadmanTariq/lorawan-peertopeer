#include <Adafruit_SSD1306.h>
#include <LoRa.h>

#include "display.h"

// #define DEBUG

#define BAND 915E6

#define RF95_CS 2
#define RF95_RST 3
#define RF95_DIO0 0
#define RF95_DIO1 1

uint8_t idx;
uint16_t value;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG
    while (!Serial)
        yield();
#endif

    initialiseScreen(&idx, &value);

    Serial.println("Receiver initialising...");

    LoRa.setPins(RF95_CS, RF95_RST, RF95_DIO0);

    int err = LoRa.begin(BAND);
    while (err == 0)
    {
        Serial.print("LoRa init failed, code ");
        Serial.println(err);

        Serial.println("Trying again in 1 second...");

        blink();
        delay(1000);
        noBlink();

        LoRa.end();
        err = LoRa.begin(BAND);
    }

    // LoRa.onReceive(onReceive);

    Serial.println("Receiver initialised.");
}

void loop()
{
    int packetSize = LoRa.parsePacket();
    if (packetSize <= 0)
    {
        return;
    }

    if (packetSize != 2)
    {
        Serial.print("Invalid packet size: ");
        Serial.println(packetSize);
        pulse(3, 100);
        return;
    }

    // read packet
    parsePacket(&idx, &value);

    Serial.print("Received packet with idx = ");
    Serial.print(idx);
    Serial.print(" and value = ");
    Serial.print(value, DEC);

    // print RSSI of packet
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());

    updateDisplay();

    pulse(1, 100);
}

void blink()
{
    digitalWrite(LED_BUILTIN, LOW);
}

void noBlink()
{
    digitalWrite(LED_BUILTIN, HIGH);
}

void pulse(unsigned int count, unsigned int duration)
{
    for (int i = 0; i < count; i++)
    {
        blink();
        delay(duration);
        noBlink();
        delay(duration);
    }
}

void parsePacket(uint8_t *idx, uint16_t *value)
{
    uint16_t payload = LoRa.read() << 8 | LoRa.read(); // 2 bytes
    *idx = payload >> 12;
    *value = payload & 0x0FFF;
}