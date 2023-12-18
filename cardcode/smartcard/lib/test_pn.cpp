// #include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (5)
#define PN532_RESET (9)

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
  // Serial.println("Listening...");
  if (nfc.inListPassiveTarget()) {
    Serial.println("Something's there...");

    uint8_t response[255];
    uint8_t responseLength = sizeof(response);

    bool success = nfc.inDataExchange(SELECT_APDU, sizeof(SELECT_APDU), response, &responseLength);

    if (success) {
      Serial.println("Sent");
      nfc.PrintHexChar(response, responseLength);
    } else {
      Serial.println("Not sent");
    }
  }
}

