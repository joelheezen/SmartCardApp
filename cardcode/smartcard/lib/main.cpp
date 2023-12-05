#include <Arduino.h>

void timeoutSet();
void timeoutWait();
void timeoutSet(unsigned long x);
void printBarcode(const char *text, uint8_t type);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("BOOTED!");
}

void loop() {
  // put your main code here, to run repeatedly:

  printBarcode("0966307", 4);
  delay(1000);
}

#define BAUDRATE 115200 //!< How many bits per second the serial port should transfer
#define BYTE_TIME (((11L * 1000000L) + (BAUDRATE / 2)) / BAUDRATE)

// ASCII codes used by some of the printer config commands:
#define ASCII_TAB '\t' //!< Horizontal tab
#define ASCII_LF '\n'  //!< Line feed
#define ASCII_FF '\f'  //!< Form feed
#define ASCII_CR '\r'  //!< Carriage return
#define ASCII_DC2 18   //!< Device control 2
#define ASCII_ESC 27   //!< Escape
#define ASCII_FS 28    //!< Field separator
#define ASCII_GS 29    //!< Group separator

uint8_t prevByte = '\n';            // Treat as if prior line is blank
uint8_t column = 0;
uint8_t maxColumn = 32;
uint8_t charHeight = 24;
uint8_t lineSpacing = 6;
uint8_t barcodeHeight = 50;
uint8_t dtrPin = 0;
unsigned long resumeTime = 0;
unsigned long dotPrintTime = 0;
unsigned long dotFeedTime = 0;
boolean dtrEnabled = false;
uint16_t firmware = 0;

// stream(&mySerial);

// // Constructor
// Adafruit_Thermal::Adafruit_Thermal(Stream *s, uint8_t dtr)
//     : stream(s), dtrPin(dtr) {
//   dtrEnabled = false;
// }
size_t write(uint8_t c) {

  if (c != 13) { // Strip carriage returns
    timeoutWait();
    Serial.println(c);
    unsigned long d = BYTE_TIME;
    if ((c == '\n') || (column == maxColumn)) { // If newline or wrap
      d += (prevByte == '\n') ? ((charHeight + lineSpacing) * dotFeedTime)
                              : // Feed line
               ((charHeight * dotPrintTime) +
                (lineSpacing * dotFeedTime)); // Text line
      column = 0;
      c = '\n'; // Treat wrap as newline on next pass
    } else {
      column++;
    }
    timeoutSet(d);
    prevByte = c;
  }

  return 1;
}

// The next four helper methods are used when issuing configuration
// commands, printing bitmaps or barcodes, etc.  Not when printing text.

void writeBytes(uint8_t a) {
  timeoutWait();
  // Serial.println(a);
  Serial.print(a);
  timeoutSet(BYTE_TIME);
}

void writeBytes(uint8_t a, uint8_t b) {
  timeoutWait();
  Serial.println(a);
  Serial.println(b);
  timeoutSet(2 * BYTE_TIME);
}

void writeBytes(uint8_t a, uint8_t b, uint8_t c) {
  timeoutWait();
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  timeoutSet(3 * BYTE_TIME);
}

void writeBytes(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  timeoutWait();
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  Serial.println(d);
  timeoutSet(4 * BYTE_TIME);
}


// Feeds by the specified number of lines
void feed(uint8_t x) {
  if (firmware >= 264) {
    writeBytes(ASCII_ESC, 'd', x);
    timeoutSet(dotFeedTime * charHeight);
    prevByte = '\n';
    column = 0;
  } else {
    while (x--)
      write('\n'); // Feed manually; old firmware feeds excess lines
  }
}

// This method sets the estimated completion time for a just-issued task.
void timeoutSet(unsigned long x) {
  if (!dtrEnabled)
    resumeTime = micros() + x;
}

// This function waits (if necessary) for the prior task to complete.
void timeoutWait() {
  if (dtrEnabled) {
    while (digitalRead(dtrPin) == HIGH) {
      yield();
    };
  } else {
    while ((long)(micros() - resumeTime) < 0L) {
      yield();
    }; // (syntax is rollover-proof)
  }
}

void printBarcode(const char *text, uint8_t type) {
  feed(1); // Recent firmware can't print barcode w/o feed first???
  if (firmware >= 264)
    type += 65;
  writeBytes(ASCII_GS, 'H', 2);    // Print label below barcode
  writeBytes(ASCII_GS, 'w', 3);    // Barcode width 3 (0.375/1.0mm thin/thick)
  writeBytes(ASCII_GS, 'k', type); // Barcode type (listed in .h file)
  if (firmware >= 264) {
    int len = strlen(text);
    if (len > 255)
      len = 255;
    writeBytes(len); // Write length byte
    for (uint8_t i = 0; i < len; i++)
      writeBytes(text[i]); // Write string sans NUL
  } else {
    uint8_t c, i = 0;
    do { // Copy string + NUL terminator
      writeBytes(c = text[i++]);
    } while (c);
  }
  timeoutSet((barcodeHeight + 40) * dotPrintTime);
  prevByte = '\n';
}

void setBarcodeHeight(uint8_t val) { // Default is 50
  if (val < 1)
    val = 1;
  barcodeHeight = val;
  writeBytes(ASCII_GS, 'h', val);
}