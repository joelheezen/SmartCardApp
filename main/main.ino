#include <Arduino.h>
#include <SPI.h>


#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "numbers.h"

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"

#define BARCODE_HEIGHT 75
#define BARCODE_Y_START 10
#define RST_PIN 9
#define SS_PIN 4
String Barcode = "8720246983982";
String inputString = "naam|nummer|format";  //naam nummer format
String values[3];
int mid = 0;
void helloWorld();
void displayBarcode();
void displayUPC(int index, int width, String str);
void displayNumber(int index, int number);
void displayCode39(int index, int width, String str);
String getBinCode(char str);
void drawCode39(int x, int y, int width, int height, int pitch, String data);

void setup() {
  Serial.begin(115200);
  display.init(115200, true, 2, false);
  // display.setRotation(1);
  // display.fillScreen(GxEPD_WHITE);
  // displayCode39(10, 1, "0966307");
  // display.display();

  // display.setRotation(1);
  // display.fillScreen(GxEPD_WHITE);
  // StartAndEndUpc(10,1);
  // displayUPC(10, 1, "123456789012" );
  // StartAndEndUpc(10,1);
  // display.display();

  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  displayEAN13(13, 1.5, Barcode);
  printText();
  display.display();
}

void extractText() {
  int startIndex = 0;
  int endIndex = inputString.indexOf('|');
  int valueIndex = 0;

  while (endIndex != -1) {
    values[valueIndex] = inputString.substring(startIndex, endIndex);
    startIndex = endIndex + 1;
    endIndex = inputString.indexOf('|', startIndex);
    valueIndex++;
  }

  // Parse the last value
  values[valueIndex] = inputString.substring(startIndex);
}

void barcodeType() {
  // soorteer welke barcode het is met value3

  if (values[3] == "Code39") {
    // display.setRotation(1);
    // display.fillScreen(GxEPD_WHITE);
    // displayCode39(10, 1, values[2]);
    // display.display(); 

  } else if (values[3] == "UPC") {
    // display.setRotation(1);
    // display.fillScreen(GxEPD_WHITE);
    // StartAndEndUpc(10,1);
    // displayUPC(10, 1, values[2] );
    // StartAndEndUpc(10,1);
    // display.display();

  } else if (values[3] == "Ean13") {
    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    displayEAN13(13, 1.5, values[2]);
    printText();
    display.display();
  }
}

void displayCode39(int index, int width, String str) {
  Serial.println(str);
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
    display.fillRect(index, BARCODE_Y_START, width, BARCODE_HEIGHT, GxEPD_WHITE);  // might be optional
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
    Serial.print("L");
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

void printText() {
  //print bij elke barcode welke barcode er weergegeven wordt
  display.setTextSize(2);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 90);
  display.print("Albert heijn");  //value 1
  display.setCursor(0, 180);
  display.print(Barcode);  // value 2
}


void loop() {
}