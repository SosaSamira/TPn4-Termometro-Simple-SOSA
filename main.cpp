#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

#define DHTPIN 2
#define DHTTYPE DHT22

#define CLK 3
#define DIO 4

DHT dht(DHTPIN, DHTTYPE);
TM1637Display display(CLK, DIO);

#define SEG_BLANK  0b00000000
#define SEG_DASH   0b01000000
#define SEG_E      0b01111001
#define SEG_r      0b01010000

void setup() {
    Serial.begin(9600);
    dht.begin();
    delay(2000);             
    display.setBrightness(7);
}

void loop() {

    float temp = dht.readTemperature();

    Serial.print("Temp leída: ");
    Serial.println(temp);

    if (isnan(temp)) {
        uint8_t segs[4] = { SEG_E, SEG_r, SEG_r, SEG_BLANK };
        display.setSegments(segs);
        delay(2000);
        return;
    }

    int entero = (int)temp;
    int decimal = abs((int)((temp - entero) * 10));

    uint8_t segs[4];

    if (temp < 0)
        segs[0] = SEG_DASH;
    else
        segs[0] = (abs(entero) < 10) ? SEG_BLANK :
                   display.encodeDigit((abs(entero) / 10) % 10);

    segs[1] = display.encodeDigit(abs(entero) % 10);
    segs[2] = display.encodeDigit(decimal) | 0x80;
    segs[3] = SEG_BLANK;

    display.setSegments(segs);

    delay(2000);   