#define BAUDRATE 4800

void setup()
{
    Serial.begin(BAUDRATE);
}

void loop()
{
    if (Serial.available())
        Serial.write(Serial.read());
}

