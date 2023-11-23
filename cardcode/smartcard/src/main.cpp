#include <Arduino.h>

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "numbers.h"

// select the display class and display driver class in the following file (new style):
#include "GxEPD2_display_selection_new_style.h"


void helloWorld();
void displayBarcode();
void displayUPC();
void displayNumber(int index, int number);
void displayCode39(int index, int width, String str);
String getBinCode(char str);
void drawCode39(int x, int y, int width, int height, int pitch,  String data);

void setup(){
    Serial.begin(115200);
    display.init(115200, true, 2, false);
    // displayUPC();

    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    displayCode39(10, 1, "*0966307*");
    display.display();

    display.hibernate();
}

const char HelloWorld[] = "Hello World!";

#define BARCODE_HEIGHT 75
#define BARCODE_Y_START 10

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

void displayNumber(int index, int number){
    Serial.println(numbers[number]);
    for(int i = 0; i < 7; i++){
        if(numbers[number][i] == '0'){
            display.fillRect(index, BARCODE_Y_START, 1, BARCODE_HEIGHT, GxEPD_WHITE); //OO
        }else{
            display.fillRect(index, BARCODE_Y_START, 1, BARCODE_HEIGHT, GxEPD_BLACK); //II
        }
        index++;
    }
}

void displayUPC(){
    int startingPosition = 10;
    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    display.fillRect(startingPosition, BARCODE_Y_START, 2, BARCODE_HEIGHT, GxEPD_WHITE); //OO
    display.fillRect(startingPosition + 2, BARCODE_Y_START, 2, BARCODE_HEIGHT, GxEPD_BLACK); //II
    display.fillRect(startingPosition + 4, BARCODE_Y_START, 2, BARCODE_HEIGHT, GxEPD_WHITE); //OO
    display.fillRect(startingPosition + 6, BARCODE_Y_START, 1, BARCODE_HEIGHT, GxEPD_BLACK); //I
    display.display();

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

void loop(){

}