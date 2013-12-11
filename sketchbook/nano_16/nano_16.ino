#include <SoftwareSerial.h>

#define N_LEITURAS 8
#define BAUDRATE 19200

void setup()
{
    Serial.begin(BAUDRATE);
}

struct sensor_t {
    uint8_t  sensor;
    SoftwareSerial *serial;
    uint16_t soma;
};

static struct sensor_t sensores[] = {
    /*  0 */    {  0 },
    /*  1 */    {  0 },
    /*  2 */    {  0 },
    /*  3 */    {  0 },
    /*  4 */    {  0 },
    /*  5 */    {  0 },
    /*  6 */    {  0 },
    /*  7 */    {  0 },
    /*  8 */    {  0 },
    /*  9 */    {  0 },
    /* 10 */    {  0 },
    /* 11 */    {  0 },
    /* 12 */    {  0 },
    /* 13 */    {  0 },
    /* 14 */    {  0 },
    /* 15 */    {  0 },
    /* 16 */    { A0 }, /* OK */
    /* 17 */    { A1 }, /* OK */
};

static struct sensor_t *leituras[] = {
    &sensores[16],
    &sensores[17],
};

static void check_serial()
{
    while (Serial.available()) {
        uint8_t c = Serial.read();
        c -= '0';
        if (c <= sizeof(sensores)/sizeof(sensores[0])) {
            if        (sensores[c].sensor) {
                if (sensores[c].soma > 0xff)
                    sensores[c].soma = 0xff;
                Serial.write(sensores[c].soma);
            } else if (sensores[c].serial) {
                SoftwareSerial *s = sensores[c].serial;
                unsigned long t0;
                s->listen();
                s->write((uint8_t) c + '0');
                t0 = millis();
                while (!s->available() && (millis() - t0) < 200);
                if (s->available()) {
                    Serial.write(s->read());
                } else {
                    Serial.write((uint8_t) 0x00);
                }
            } else {
                Serial.write((uint8_t) 0x00);
            }
        } else {
            Serial.write((uint8_t) 0x00);
        }
    }
}

void loop()
{
    for (uint8_t i = 0; i < sizeof(leituras)/sizeof(leituras[0]); i++) {
        struct sensor_t *s = leituras[i];
        if (s->sensor) {
            unsigned long t0;
            analogRead(s->sensor);
            t0 = millis();
            while (millis() < (t0+10))
                check_serial();
            s->soma = 0;
            for (uint8_t j = 0; j < N_LEITURAS; j++)
                s->soma += analogRead(s->sensor);
        }
    }
}

