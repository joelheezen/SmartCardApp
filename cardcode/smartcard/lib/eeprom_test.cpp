#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <Adafruit_PN532.h>


void setup(){
    Serial.begin(115200);
    EEPROM.begin(512);
    float test = 20.3;
    EEPROM.put(0, test);
    Serial.println(EEPROM.commit());
    float t;
    EEPROM.get(0, t);
    Serial.println(t);
    Serial.println(EEPROM.read(0));
}

void loop(){

}