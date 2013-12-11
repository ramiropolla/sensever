#include <SoftwareSerial.h>

// software serial #1: TX = digital pin 3, RX = digital pin 2
static SoftwareSerial serial16(3, 2);
// software serial #2: TX = digital pin 4, RX = digital pin 5
static SoftwareSerial serial14(4, 5);

#define N_LEITURAS 8
#define BAUDRATE 19200

void setup()
{
    Serial.begin(BAUDRATE);
    serial14.begin(BAUDRATE);
    serial16.begin(BAUDRATE);
}

struct sensor_t {
    uint8_t  sensor;
    SoftwareSerial *serial;
    uint16_t soma;
};

static struct sensor_t sensores[] = {
    /*  0 */    {  0 },
    /*  1 */    { A7 }, /* OK */
    /*  2 */    { A3 }, /* OK */
    /*  3 */    { A2 }, /* OK */
    /*  4 */    {  0 },
    /*  5 */    {  0 },
    /*  6 */    { A5 }, /* OK */
    /*  7 */    { A1 }, /* OK */
    /*  8 */    {  0 },
    /*  9 */    {  0 },
    /* 10 */    { A6 }, /* OK */
    /* 11 */    { A4 }, /* OK */
    /* 12 */    {  0 },
    /* 13 */    {  0 },
    /* 14 */    {  0, &serial14 },
    /* 15 */    {  0, &serial14 },
    /* 16 */    {  0, &serial16 },
    /* 17 */    {  0, &serial16 },
};

static struct sensor_t *leituras[] = {
    &sensores[ 1],
    &sensores[ 2],
    &sensores[ 3],
    &sensores[ 6],
    &sensores[ 7],
    &sensores[10],
    &sensores[11],
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

