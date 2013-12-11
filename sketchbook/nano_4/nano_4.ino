#include <SoftwareSerial.h>

// software serial #1: TX = digital pin 2, RX = digital pin 3
static SoftwareSerial serial2(2, 3);

#define N_LEITURAS 8
#define BAUDRATE 19200

void setup()
{
    Serial.begin(BAUDRATE);
    serial2.begin(BAUDRATE);
}

struct sensor_t {
    uint8_t  sensor;
    SoftwareSerial *serial;
    uint16_t soma;
};

static struct sensor_t sensores[] = {
    /*  0 */    {  0 },
    /*  1 */    {  0, &serial2 },
    /*  2 */    {  0, &serial2 },
    /*  3 */    {  0, &serial2 },
    /*  4 */    { A4 }, /* OK */
    /*  5 */    { A1 }, /* OK */
    /*  6 */    {  0, &serial2 },
    /*  7 */    {  0, &serial2 },
    /*  8 */    { A5 }, /* OK */
    /*  9 */    { A3 }, /* OK */
    /* 10 */    {  0, &serial2 },
    /* 11 */    {  0, &serial2 },
    /* 12 */    { A6 }, /* OK */
    /* 13 */    { A2 }, /* OK */
    /* 14 */    {  0, &serial2 },
    /* 15 */    {  0, &serial2 },
    /* 16 */    {  0, &serial2 },
    /* 17 */    {  0, &serial2 },
};

static struct sensor_t *leituras[] = {
    &sensores[ 4],
    &sensores[ 5],
    &sensores[ 8],
    &sensores[ 9],
    &sensores[12],
    &sensores[13],
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

