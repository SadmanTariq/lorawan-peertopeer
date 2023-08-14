#include <LoRa.h>

#define BAND 915E6

#define RF95_CS 2
#define RF95_RST 3
#define RF95_DIO0 0
#define RF95_DIO1 1

#define SENSOR_PIN A4

// #define DEBUG

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG
    while (!Serial)
        yield();
#endif

    pinMode(SENSOR_PIN, INPUT);

    Serial.println("Sender initialising...");

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

    Serial.println("Sender initialised.");
}

uint8_t payloadIdx = 0;

void loop()
{
    uint16_t payload = (payloadIdx << 12) | (0x0FFF & analogRead(SENSOR_PIN)); // 4 bits of index, 12 bits of data
    payloadIdx++;

    Serial.print("Sending packet with payload ");
    Serial.println(payload, BIN);

    LoRa.beginPacket();

    // Send payload split into two bytes
    LoRa.write(payload >> 8);
    LoRa.write(payload & 0xFF);

    LoRa.endPacket();

    blink();
    delay(500);
    noBlink();
    delay(500);
}

void blink()
{
    digitalWrite(LED_BUILTIN, LOW);
}

void noBlink()
{
    digitalWrite(LED_BUILTIN, HIGH);
}