#include <Keypad.h>
#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}
};

byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins,colPins,ROWS,COLS);

String display, str_result;
bool first, npe;
double result, temp;
char op;

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  init();
}

void init(){
  display = "";
  str_result = "";
  first = true;
  npe = true;
  result = 0;
  temp = 0;
  op = 0;
}

void display_u8g2(){
  if(display.length() > 13){
    display = display.substring(display.length() - 13);
  }
  u8g2.setFont(u8g2_font_10x20_mf);
  u8g2.firstPage();
  do{
    if(display != ""){
      u8g2.setCursor(0,40);
      u8g2.print(display);
      u8g2.setCursor(0,60);
      u8g2.print("=");
      u8g2.setCursor(10,60);
      u8g2.print(str_result);
    }
  }while(u8g2.nextPage());
}

void calc(){
  switch (op){
    case '+':
      result += temp;
      break;
    case '-':
      result -= temp;
      break;
    case '*':
      result *= temp;
      break;
    case '/':
      result /= temp;
      break;
  }
}

void formatResult(double r) {  
  int int_length = String(int(r)).length();

  int dec_length = 0;
  while(int(r) != r){
    dec_length++;
    r *= 10;

    if(int_length + dec_length >= 10){
      break;
    }
  }
  char buffer[30];
  dtostrf(result, int_length + dec_length, dec_length, buffer);
  str_result = String(buffer);
}


void loop() {
  char key = keypad.getKey();

  if (key) {
    if(key - 48 >= 0 && key - 48 <= 9){
      temp = temp * 10 + (key - 48);
      display += key;
      
      npe = true;
    } else if (key == 'C'){
      init();
    } else if (key == '='){
      calc();

      /*
        這行居然不能用!?
        str_result = String(std::floor(result) == result ? int(result) : result);
      */

      if(std::floor(result) == result){
        str_result = String(int(result));
      } else {
        formatResult(result);
      }

      npe = false;
    } else {
      display += key;

      if(first){
        result = temp;
      }

      if(op != 0 && npe){
        calc();
      }

      op = key;
      temp = 0;
      first = false;
      npe = true;
    }
    display_u8g2();
  }
}