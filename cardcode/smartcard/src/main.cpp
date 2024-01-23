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
#define RST_PIN 9
#define SS_PIN 4
#define SS_SCREEN 15
#define DELIMITER "|"
#define BUFFSIZE 1024
#define BARCODE_INDEX 3
#define BAR_WIDTH 1
#define BUTTON_PIN 2


PN532_SPI pn532spi(SPI, 4);
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
void helloWorld();
void displayBarcode();
void displayUPC(int index, int width, String str);
void displayNumber(int index, int number);
void displayCode39(int index, int width, String str);
String getBinCode(char str);
void drawCode39(int x, int y, int width, int height, int pitch, String data);
void drawEAN13Module(int &index, int width, String code);
void displayEAN13(int index, int width, String str);
void printText(Card card);
void printText(String str);


void setupNFC(){
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
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
  Serial.println("Hello from smartcard");
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  display.init(115200, true, 2, false);
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.display();

  setupNFC();
  EEPROM.begin(1024);
  EEPROM.get(0, cardsIndex);
  Serial.println(cardsIndex);
  for(int i = 0; i < cardsIndex; i++){
    EEPROM.get((i*sizeof(Card))+sizeof(cardsIndex), cards[i]);
  }
  printCards();
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

Card* getCards(){
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
      // display.clearScreen();
      printText("Transfer didn't succeed, try again!");
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
    }
  }
  return cards;
}

void displayCode39(int index, int width, String str) {;
  Serial.println(str);
  str = '*' + str + '*';
  for (int i = 0; i < str.length(); i++) {
    String code = getCode39(str[i]);
    int color = GxEPD_WHITE;
    for (int j = 0; j < 9; j++) {
      if (color == GxEPD_WHITE) {
        color = GxEPD_BLACK;
      } else {
        color = GxEPD_WHITE;
      }
      int bw;
      if (code[j] == '0') {
        bw = width;
      } else {
        bw = width * 2;
      }
      display.fillRect(index, BARCODE_Y_START, bw, BARCODE_HEIGHT, color);
      index = index + bw;
    }
    // display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);  // might be optional
    index = index + width;
  }
}
void StartAndEndUpc(int index, int width) {
  String code = "101";
  int color = GxEPD_WHITE;
  for (int j = 0; j < 3; j++) {
    if (color == GxEPD_WHITE) {
      color = GxEPD_BLACK;
    } else {
      color = GxEPD_WHITE;
    }
    int bw;
    if (code[j] == '0') {
      bw = width;
    } else {
      bw = width * 2;
    }
    display.fillRect(index, BARCODE_Y_START, bw, BARCODE_HEIGHT, color);
    index = index + bw;
  }
  display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);  // might be optional
  index = index + width;
  display.fillRect(index += width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
  display.fillRect(index += width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);
  display.fillRect(index += width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
}


void displayUPC(int index, int width, String str) {
  Serial.println(str);
  //start bar 101
  // StartAndEndUpc(10,1);
  //left bar

  for (int i = 0; i < (str.length() / 2) + 1; i++) {
    if (i == 0) {
      StartAndEndUpc(10, 1);
    }
    String code = getUPCLeft(str[i]);
    int color = GxEPD_WHITE;
    for (int j = 0; j < 7; j++) {
      mid++;
      if (color == GxEPD_WHITE) {
        color = GxEPD_BLACK;
      } else {
        color = GxEPD_WHITE;
      }
      int bw;
      if (code[j] == '0') {
        bw = width;
      } else {
        bw = width * 2;
      }
      display.fillRect(index, BARCODE_Y_START, bw, BARCODE_HEIGHT, color);
      index = index + bw;
    }
    display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);  // might be optional
    index = index + width;
    if (mid == 7) {
      String code = GetUPCMid(str[1]);
    }
  }

  //right bar
  for (int i = 7; i < str.length(); i++) {
    String code = GetUPCRight(str[i]);
    int color = GxEPD_WHITE;
    for (int j = 0; j < 6; j++) {
      if (color == GxEPD_WHITE) {
        color = GxEPD_BLACK;
      } else {
        color = GxEPD_WHITE;
      }
      int bw;
      if (code[j] == '0') {
        bw = width;
      } else {
        bw = width * 2;
      }
      display.fillRect(index, BARCODE_Y_START, bw, BARCODE_HEIGHT, color);
      index = index + bw;
    }
    display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);  // might be optional
    index = index + width;
    Serial.print("R");
  }
  // StartAndEndUpc(10,1);

  display.fillRect(index += width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
  display.fillRect(index += width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);
  display.fillRect(index += width, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_BLACK);
}

void displayEAN13(int index, int width, String str) {
  Serial.println(str);

  // Start marker
  drawEAN13Module(index, width, "101");
  index += width;

  // Draw left-hand side

  String order;
  String code;

  char firstDigit = str[0];  // Get the first digit

  if (firstDigit == '0') {
    order = "LLLLLL";
  } else if (firstDigit == '1') {
    order = "LLGLGG";
  } else if (firstDigit == '2') {
    order = "LLGGLG";
  } else if (firstDigit == '3') {
    order = "LLGGGL";
  } else if (firstDigit == '4') {
    order = "LGLLGG";
  } else if (firstDigit == '5') {
    order = "LGGLLG";
  } else if (firstDigit == '6') {
    order = "LGGGLL";
  } else if (firstDigit == '7') {
    order = "LGLGLG";
  } else if (firstDigit == '8') {
    order = "LGLGGL";
  } else if (firstDigit == '9') {
    order = "LGGLGL";
  }

  for (int i = 0; i <= 5; i++) {
    if (order[i] == 'L') {
      Serial.println(order[i]);
      code = getEAN13LeftL(str[i + 1]);
    } else if (order[i] == 'G') {
      Serial.println(order[i]);
      code = getEAN13LeftG(str[i + 1]);
    }
    int bw = width;
    if (code[i] == '0') {
      bw = width;
    } else {
      bw = width * 2;
    }

    drawEAN13Module(index, bw, code);
    index += width;
  }

  drawEAN13Module(index, width, "01010");
  index += width;

  // Draw right-hand side
  for (int i = 7; i < 13; i++) {
    String code = getEAN13Right(str[i]);
    int bw = width;
    if (code[i] == '0') {
      bw = width;
    } else {
      bw = width * 2;
    }
    drawEAN13Module(index, bw, code);
    index += width;
  }
  //stop marker
  drawEAN13Module(index, width, "101");
  index += width;
}

void drawEAN13Module(int &index, int width, String code) {
  for (int j = 0; j < code.length(); j++) {
    int color = (code[j] == '1') ? GxEPD_BLACK : GxEPD_WHITE;
    display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, color);
    index += width;
  }
}

void printText(String str){
  display.setTextSize(2);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 90);
  display.print(str);  //value 1
}

void printText(Card card) {
  //print bij elke barcode welke barcode er weergegeven wordt
  display.setTextSize(2);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 90);
  display.print(card.name);  //value 1
  display.setCursor(0, 180);
  display.print(card.barcode);  // value 2
}

void displayCard(Card card){
  // display.clearScreen();
  display.fillScreen(GxEPD_WHITE);
  switch(card.type){
    case FORMAT_EAN_13:
      displayEAN13(BARCODE_INDEX, BAR_WIDTH, card.barcode);
    break;
    case FORMAT_UPC_A:
      displayUPC(BARCODE_INDEX, BAR_WIDTH, card.barcode);
    break;
    case FORMAT_CODE_39:
      displayCode39(BARCODE_INDEX, BAR_WIDTH, card.barcode);
    break;
    default:
      Serial.println("UNRECOGNIZED TYPE");
    break;
  }
  printText(card);
  display.display();
}


void loop() {
  if(digitalRead(BUTTON_PIN)){
    if(cardsIndex != 0){
      currentCard = (currentCard + 1) % cardsIndex;
    }
  }
  getCards();
  if(currentCard != prevCurrentCard){
    displayCard(cards[currentCard]);
    prevCurrentCard = currentCard;
    setupNFC();
  }
}