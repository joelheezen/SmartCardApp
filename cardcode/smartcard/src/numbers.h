#ifndef NUMBERS_H
#define NUMBERS_H
#include <string.h>
char numbers[10][10] = {
  "0001101",  //0
  "0011001",  //1
  "0010011",  //2
  "0111101",  //3
  "0100011",  //4
  "0110001",  //5
  "0101111",  //6
  "0111011",  //7
  "0110111",  //8
  "0001011"   //9
};


String getCode39(char str) {
  if (str == '@') return String("010010100");
  if (str == '0') return String("000110100");
  if (str == '1') return String("100100001");
  if (str == '2') return String("001100001");
  if (str == '3') return String("101100000");
  if (str == '4') return String("000110001");
  if (str == '5') return String("100110000");
  if (str == '6') return String("001110000");
  if (str == '7') return String("000100101");
  if (str == '8') return String("100100100");
  if (str == '9') return String("001100100");
  if (str == 'A') return String("100001001");
  if (str == 'B') return String("001001001");
  if (str == 'C') return String("101001000");
  if (str == 'D') return String("000011001");
  if (str == 'E') return String("100011000");
  if (str == 'F') return String("001011000");
  if (str == 'G') return String("000001101");
  if (str == 'H') return String("100001100");
  if (str == 'I') return String("001001100");
  if (str == 'J') return String("000011100");
  if (str == 'K') return String("100000011");
  if (str == 'L') return String("001000011");
  if (str == 'M') return String("101000010");
  if (str == 'N') return String("000010011");
  if (str == 'O') return String("100010010");
  if (str == 'P') return String("001010010");
  if (str == 'Q') return String("000000111");
  if (str == 'R') return String("100000110");
  if (str == 'S') return String("001000110");
  if (str == 'T') return String("000010110");
  if (str == 'U') return String("110000001");
  if (str == 'V') return String("011000001");
  if (str == 'W') return String("111000000");
  if (str == 'X') return String("010010001");
  if (str == 'Y') return String("110010000");
  if (str == 'Z') return String("011010000");
  if (str == '-') return String("010000101");
  if (str == '.') return String("110000100");
  if (str == ' ') return String("011000100");
  if (str == '*') return String("010010100");
  if (str == '$') return String("010101000");
  if (str == '/') return String("010100010");
  if (str == '+') return String("010001010");
  if (str == '%') return String("000101010");
  if (str == '#') return String("010010100");
  return String("");
}

String getUPCLeft(char str) {
  if (str == '0') return String("0001101");
  if (str == '1') return String("0011001");
  if (str == '2') return String("0010011");
  if (str == '3') return String("0111101");
  if (str == '4') return String("0100011");
  if (str == '5') return String("0110001");
  if (str == '6') return String("0101111");
  if (str == '7') return String("0111011");
  if (str == '8') return String("0110111");
  if (str == '9') return String("0001011");
  return String("");
}
String GetUPCRight(char str) {
  if (str == '0') return String("1110010");
  if (str == '1') return String("1100110");
  if (str == '2') return String("1101100");
  if (str == '3') return String("1000010");
  if (str == '4') return String("1011100");
  if (str == '5') return String("1001110");
  if (str == '6') return String("1010000");
  if (str == '7') return String("1000100");
  if (str == '8') return String("1001000");
  if (str == '9') return String("1110100");
  return String("");
}
String GetUPCMid(char str) {
  if (str == '1') return String("01010");
    return String("");
}
 //als de eerste digit van de str gelijk is aan 0
  String getEAN13LeftL(char str) {
      switch (str) {
      case '0': return String("0001101");
      case '1': return String("0011001");
      case '2': return String("0010011");
      case '3': return String("0111101");
      case '4': return String("0100011");
      case '5': return String("0110001");
      case '6': return String("0101111");
      case '7': return String("0111011");
      case '8': return String("0110111");
      case '9': return String("0001011");
      default: return String("");
    }
    }
    //als de eerste digit van de str gelijk is aan 1
      String getEAN13LeftG(char str) {
      switch (str) {
      case '0': return String("0100111");
      case '1': return String("0110011");
      case '2': return String("0011011");
      case '3': return String("0100001");
      case '4': return String("0011101");
      case '5': return String("0111001");
      case '6': return String("0000101");
      case '7': return String("0010001");
      case '8': return String("0001001");
      case '9': return String("0010111");
      default: return String("");
    }
    }
    
    


String getEAN13Right(char str) {
  switch (str) {
    case '0': return String("1110010");
    case '1': return String("1100110");
    case '2': return String("1101100");
    case '3': return String("1000010");
    case '4': return String("1011100");
    case '5': return String("1001110");
    case '6': return String("1010000");
    case '7': return String("1000100");
    case '8': return String("1001000");
    case '9': return String("1110100");
    default: return String("");
  }
}



#endif