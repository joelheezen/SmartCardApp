#include <Arduino.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <Adafruit_PN532.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "numbers.h"
// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"
#include "barcode_types.h"
#include <EEPROM.h>

#define BARCODE_HEIGHT 75
#define BARCODE_Y_START 10
// #define RST_PIN 9
#define SS_PIN 4
#define SS_SCREEN 15
#define DELIMITER "|"
#define BUFFSIZE 1024
#define BARCODE_INDEX 3
#define BAR_WIDTH 1
#define BUTTON_PIN 10
#define FWD_BUTTON_PIN 2


PN532_SPI pn532spi(SPI, SS_PIN);
PN532 nfc(pn532spi);

typedef struct {
  char name[25];
  char barcode[64];
  int type;
} Card;

Card cards[100] = {};

int mid = 0;
int cardsIndex = 0;
int currentCard = -1;
int prevCurrentCard = -1;
int eeAddress = 0;

void printCards();
void printText(String str, int height);


void setupNFC(){
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
   // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  nfc.setPassiveActivationRetries(0x02);
  // nfc.setPassiveActivationRetries(0xFF);
  // configure board to read RFID tags
  nfc.SAMConfig(); 
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Hello from smartcard");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FWD_BUTTON_PIN, INPUT_PULLUP);

  display.init(115200, true, 2, false);
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.display();

  printText("TEST", 49);
  display.display();

  // setupNFC();
}

Card parseCard(String input){
  Card retCard;  
  int name_start = 0;
  int name_end = input.indexOf(DELIMITER);
  strncpy(retCard.name, input.substring(name_start, name_end).c_str(), sizeof(retCard.name) - 1);

  int barcode_start = input.indexOf(DELIMITER, name_end)+1;
  int barcode_end = input.indexOf(DELIMITER, barcode_start+1);
  strncpy(retCard.barcode, input.substring(barcode_start, barcode_end).c_str(), sizeof(retCard.barcode) -1);

  int type_start = input.indexOf(DELIMITER, barcode_end)+1;
  int type_end = input.length();
  retCard.type = atoi(input.substring(type_start, type_end).c_str());
  return retCard;
}

void printCards(){
  Serial.println("PRINTING CARDS");
  for(int i = 0; i < cardsIndex; i++){
    Serial.print(cards[i].name); Serial.print('\t'); Serial.print(cards[i].barcode);
    Serial.print('\t'); Serial.println(cards[i].type);
  }

}

bool getCards(){
  bool success;
  uint8_t responseLength = 128;
  success = nfc.inListPassiveTarget();

  if(success){
    String respBuffer;
    Serial.println("Found something");
    uint8_t selectApdu[] = { 0x00, /* CLA */
                              0xA4, /* INS */
                              0x04, /* P1  */
                              0x00, /* P2  */
                              0x07, /* Length of AID  */
                              0xF0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, /* AID defined on Android App */
                              0x00  /* Le  */ };
                              
    uint8_t response[128];  
     
    success = nfc.inDataExchange(selectApdu, sizeof(selectApdu), response, &responseLength);
    if(success){
      Serial.print("responseLength: "); Serial.println(responseLength);
      for (uint8_t i = 0; i < responseLength; i++) {
          char c = response[i];
          if (c <= 0x1f || c > 0x7f) {
              respBuffer += '.';
          } else {
              respBuffer += c;
          }
      }
    }
    do{
      uint8_t apdu[] = "";
      uint8_t output[200];
      uint8_t length = 200;

      success = nfc.inDataExchange(apdu, sizeof(apdu), output, &length);
      Serial.println("IN LOOP");
      if(success){
        Serial.print("responseLength: "); Serial.println(length);
        for (uint8_t i = 0; i < length; i++) {
            char c= output[i];
            if (c <= 0x1f || c > 0x7f) {
                respBuffer += c;
            } else {
                respBuffer += c;
            }
        }
      }
    }while(success);

    if(respBuffer.indexOf("DONE") == -1){
      Serial.println("DATA TRANSFER NOT CORRECT!!!");
      display.init(115200, false, 10, false);
      printText("Transfer didn't succeed, try again!", 90);
      display.display();
      delay(500);
      setupNFC();
    }else{
      memset(cards, 0, sizeof(cards));
      cardsIndex = 0;
      int startpos = respBuffer.indexOf('[') + 1;
      while(startpos <= respBuffer.lastIndexOf(',')){
        int entrypos = respBuffer.indexOf(',', startpos);
        String entry = respBuffer.substring(startpos, entrypos);
        cards[cardsIndex++] = parseCard(entry);
        startpos = entrypos + 2;
        entrypos = respBuffer.indexOf(',', entrypos + 1);
      }
      String entry = respBuffer.substring(startpos, respBuffer.indexOf(']'));
      cards[cardsIndex++] = parseCard(entry);
      printCards();
      EEPROM.put(eeAddress, cardsIndex);
      eeAddress += sizeof(cardsIndex);
      for(int i = 0; i < cardsIndex; i++){
        EEPROM.put((i * sizeof(Card)) + eeAddress, cards[i]);
      }
      eeAddress += sizeof(cards);
      EEPROM.commit();
      currentCard = 0;
      if(currentCard !=-1){
        currentCard = (currentCard + 1) % cardsIndex;
      }
      return true;
    }
  }
  return false;
}


void printText(String str, int height){
  display.setTextSize(2);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, height);
  display.print(str);  //value 1
}

int counter = 0;
void loop() {
    // getCards();
    if(digitalRead(BUTTON_PIN) && analogRead(A0) > 500){
      Serial.println("READING NFC");
      setupNFC();
      while(!getCards()){
        delay(1);
      }
      counter = 0;
    }
    if(digitalRead(BUTTON_PIN)){
      // display.init(115200, true, 10, false);
      display.fillScreen(GxEPD_WHITE);
      printText((String)counter, 49);
      display.display();
      counter++;
    }
    // if(currentCard != prevCurrentCard){
    //     Serial.print("CHANGING CARD TO:");
    //     Serial.println(currentCard);
    //     // displayCard(cards[currentCard]);
    //     // display.init(115200, true, 2, false);
    //     display.init(115200, false, 10, false);
    //     display.clearScreen();
    //     display.setRotation(1);
    //     display.fillScreen(GxEPD_WHITE);
    //     display.display();
    //     printText("TEST" , 49);
    //     display.display();
    //     prevCurrentCard = currentCard;
    //     setupNFC();
    // }
}