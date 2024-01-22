// #include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (5)
#define PN532_RESET (9)
#define PN532DEBUG 1

Adafruit_PN532 nfc(4);

uint8_t SELECT_APDU[] = {
  0x00, /* CLA */
  0xA4, /* INS */
  0x04, /* P1  */
  0x00, /* P2  */
  0x07, /* Length of AID  */
  0xF0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, /* AID  */
  0x00  /* Le  */
};

// uint8_t SELECT_APDU[] = { 
//   0x00, /* CLA */
//   0xA4, /* INS */
//   0x04, /* P1  */
//   0x00, /* P2  */
//   0x05, /* Length of AID  */
//   0xF2, 0x22, 0x22, 0x22, 0x22,
//   0x00  /* Le  */
// };

void setup() {
  Serial.begin(115200);

  Serial.println("HELLO");
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    Serial.flush();
    while (1); // halt
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  nfc.begin();
}

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  uint8_t responseLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("Found an NFC card!");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC);Serial.println(" Bytes");
    Serial.print("UID value: ");
    for(uint8_t i = 0; i < uidLength; i++){
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println();
    uint8_t response[] = { 0xAA, 0xBB, 0xCC, 0xDD};
    responseLength = sizeof(response);
    nfc.inDataExchange(uid, uidLength, response, &responseLength);
  }
}


void parseData(String data){
  Serial.println(data);

}

