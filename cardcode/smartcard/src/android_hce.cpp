#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
#include <Adafruit_PN532.h>

#define DELIMITER "|"
#define BUFFSIZE 1024
  
PN532_SPI pn532spi(SPI, 4);
PN532 nfc(pn532spi);


char* example_string = "[Ndfg|5802126620020|2, tese|2290660800074|32, ddddD|2290660800074|32, twst|2290660800074|32, makto|5802126620020|2, Nd|5802126690|2, joejeo|2290660800074|32, Namesd|5802126620020|2]DONE";

typedef struct {
  char name[100];
  char barcode[100];
  int type;
} Cards;
void printCards();
Cards getCard(String input);

int cards_index = 0;

Cards cards[100] = {};


void setup()
{    
    Serial.begin(115200);
    Serial.println("-------Peer to Peer HCE--------");
    
    nfc.begin();
    
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
    }
    
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    //nfc.setPassiveActivationRetries(0xFF);
    
    // configure board to read RFID tags
    nfc.SAMConfig();
}

void loop(){
  bool success;
  uint8_t responseLength = 128;
  Serial.println("Wating for ISO14443A card");
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
    Serial.println(respBuffer);
     
    success = nfc.inDataExchange(selectApdu, sizeof(selectApdu), response, &responseLength);
    if(success){
      Serial.print("responseLength: "); Serial.println(responseLength);
      
      nfc.PrintHexChar(response, responseLength);
      for (uint8_t i = 0; i < responseLength; i++) {
          char c = response[i];
          if (c <= 0x1f || c > 0x7f) {
              respBuffer += '.';
              // c[0] = '.';
              // strcat(respBuffer, c);
          } else {
              respBuffer += c;
              // strcat(respBuffer, c);
          }
      }

      Serial.print("OUTPUT:\t"); Serial.println(respBuffer);
    }
    do{
      uint8_t apdu[] = "";
      uint8_t output[200];
      uint8_t length = 200;

      success = nfc.inDataExchange(apdu, sizeof(apdu), output, &length);
      if(success){
        Serial.print("responseLength: "); Serial.println(length);
        
        nfc.PrintHexChar(output, length);
        for (uint8_t i = 0; i < length; i++) {
            char c= output[i];
            if (c <= 0x1f || c > 0x7f) {
                // c[0] = '.';
                // strcat(respBuffer, c);
                respBuffer += c;
            } else {
                // strcat(respBuffer, c);
                respBuffer += c;
            }
        }

        Serial.print("OUTPUT:\t"); Serial.println(respBuffer);

      }
    }while(success);

    if(respBuffer.indexOf("DONE") == -1){
      Serial.println("DATA TRANSFER NOT CORRECT!!!");
    }else{
      int startpos = respBuffer.indexOf('[') + 1;
      while(startpos <= respBuffer.lastIndexOf(',')){
        int entrypos = respBuffer.indexOf(',', startpos);
        String entry = respBuffer.substring(startpos, entrypos);
        Serial.println(entry);
        cards[cards_index++] = getCard(entry);
        startpos = entrypos + 2;
        entrypos = respBuffer.indexOf(',', entrypos + 1);
      }
      String entry = respBuffer.substring(startpos, respBuffer.indexOf(']'));
      Serial.println(entry);
      cards[cards_index++] = getCard(entry);
      printCards();
    }
  }
}

Cards getCard(String input){
  Cards retCard;  
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
  for(int i = 0; i < cards_index; i++){
    Serial.print(cards[i].name); Serial.print('\t'); Serial.print(cards[i].barcode);
    Serial.print('\t'); Serial.println(cards[i].type);
  }
}

void printResponse(uint8_t *response, uint8_t responseLength) {
  
   String respBuffer;

    for (int i = 0; i < responseLength; i++) {
      
      if (response[i] < 0x10) 
        respBuffer = respBuffer + "0"; //Adds leading zeros if hex value is smaller than 0x10
      
      respBuffer = respBuffer + String(response[i], HEX) + " ";                        
    }

    Serial.print("response: "); Serial.println(respBuffer);
}

void setupNFC() {
 
  nfc.begin();
    
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig(); 
}