#include <Arduino.h>
#include <SPI.h>

#include <PN532_SPI.h>
#include "PN532.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "numbers.h"

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

#define BARCODE_HEIGHT 75
#define BARCODE_Y_START 10
#define RST_PIN 9
#define SS_PIN 4

// PN532_SPI pn532spi(SPI, SS_PIN);
// PN532 nfc(pn532spi);
// String tagid = "None";

void helloWorld();
void displayBarcode();
void displayUPC(int index, int width, String str);
void displayNumber(int index, int number);
void displayCode39(int index, int width, String str);
String getBinCode(char str);
void drawCode39(int x, int y, int width, int height, int pitch,  String data);

void setup(){
    Serial.begin(115200);
    // SPI.begin();
    // nfc.begin();
    // uint32_t versiondata = nfc.getFirmwareVersion();
    // if (! versiondata){
    //     Serial.print("Didn't find PN53x board");
    //     while (1);
    // }
    // Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
    // nfc.SAMConfig();
    display.init(115200, true, 2, false);
    // delay(2);
    Serial.println("Scan PICC to see UID, SAK, type, and dat blocks...");
    // displayUPC();

    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    displayCode39(10, 1, "*0966307*");
    display.display();

    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    displayUPC(10, 1, "0966307");
    display.display();

    display.hibernate();
    
}

const char HelloWorld[] = "Hello World!";


void displayCode39(int index, int width, String str){
    Serial.println(str);
    for(int i = 0; i < str.length(); i++){
        String code = getCode39(str[i]);
        int color = GxEPD_WHITE;
        for(int j = 0; j < 9; j++){
            if(color == GxEPD_WHITE){
                color = GxEPD_BLACK;
            }else{
                color = GxEPD_WHITE;
            }
            int bw;
            if(code[j] == '0'){
                bw = width;
            }else{
                bw = width * 2;
            }
            display.fillRect(index, BARCODE_Y_START, bw, BARCODE_HEIGHT, color);
            index = index + bw;
        }
        display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE); // might be optional
        index = index + width;
    }
}

void displayUPC(int index, int width, String str){
    Serial.println(str);
    display.fillRect(index+=width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
    display.fillRect(index+=width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);
    display.fillRect(index+=width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);

    for(int i = 0; i < str.length(); i++){
        String code = getCode39(str[i]);
        int color = GxEPD_WHITE;
        for(int j = 0; j < 9; j++){
            if(color == GxEPD_WHITE){
                color = GxEPD_BLACK;
            }else{
                color = GxEPD_WHITE;
            }
            int bw;
            if(code[j] == '0'){
                bw = width;
            }else{
                bw = width * 2;
            }
            display.fillRect(index, BARCODE_Y_START, bw, BARCODE_HEIGHT, color);
            index = index + bw;
        }
        display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE); // might be optional
        index = index + width;
    }

    display.fillRect(index+=width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
    display.fillRect(index+=width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);
    display.fillRect(index+=width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
}

void helloWorld()
{
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorld);
  }
  while (display.nextPage());
}

// void readNFC(){
//     uint8_t success;
//     uint8_t uid[] = {0,0,0,0,0,0,0,0};
//     uint8_t uidLength;
//     success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
//     if(success){
//         Serial.println("Found an ISO14443A card");
//         Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
//         Serial.print("  UID Value: ");
//         nfc.PrintHex(uid, uidLength);
//         Serial.println("");
//     }
//     delay(1000);
// }

void loop(){
    // readNFC();

}