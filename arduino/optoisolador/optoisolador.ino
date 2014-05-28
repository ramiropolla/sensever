#include <SoftwareSerial.h>

// software serial #1: RX = digital pin 2, TX = digital pin 3
static SoftwareSerial serial2(2, 3);

#define BAUDRATE 19200

void setup()
{
    Serial.begin(BAUDRATE);
    serial2.begin(4800); /* opto-isolador */
}

void loop()
{
    if (Serial.available())
        serial2.write(Serial.read());
    if (serial2.available())
        Serial.write(serial2.read());
}

