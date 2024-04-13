#include <Keypad.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <vector>
using namespace std;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', '+' },
  { '4', '5', '6', '-' },
  { '7', '8', '9', '*' },
  { 'C', '0', '=', '/' }
};

byte rowPins[ROWS] = { 32, 33, 25, 26 };
byte colPins[COLS] = { 27, 14, 12, 13 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

vector<double> nums;
vector<char> ops;
String display, str_result;
double result, temp;
int i;

void setup() {
  u8g2.begin();
  init();
}

void init() {
  nums.clear();
  ops.clear();
  display = "";
  str_result = "";
  result = 0;
  temp = 0;
  i = 0;
}

void display_u8g2() {
  if (display.length() > 13) {
    display = display.substring(display.length() - 13);
  }
  u8g2.setFont(u8g2_font_10x20_mf);
  u8g2.firstPage();
  do {
    if (display != "") {
      u8g2.setCursor(0, 40);
      u8g2.print(display);
      u8g2.setCursor(0, 60);
      u8g2.print("=");
      u8g2.setCursor(10, 60);
      u8g2.print(str_result);
    }
  } while (u8g2.nextPage());
}

void calc(vector<double> nums, vector<char> ops) {
  if (nums.size() < 2 or nums.size() - ops.size() != 1) {
    return;
  }

  while (ops.size() != 0) {
    int location = 0;
    for (int i = 0; i < ops.size(); i++) {
      if (ops[i] == '*' or ops[i] == '/') {
        location = i;
        break;
      }
    }

    double a = nums[location];
    double b = nums[location + 1];
    char op = ops[location];

    switch (op) {
      case '+':
        a += b;
        break;
      case '-':
        a -= b;
        break;
      case '*':
        a *= b;
        break;
      case '/':
        a /= b;
        break;
    }

    nums.erase(nums.begin() + location, nums.begin() + location + 2);
    ops.erase(ops.begin() + location);
    nums.insert(nums.begin() + location, a);
  }

  result = nums[0];

  if (int(result) == result) {
    str_result = String(int(result));
  } else {
    format_result(result);
  }
}

void format_result(double r) {
  int int_length = String(int(r)).length();

  int dec_length = 0;
  while (int(r) != r) {
    dec_length++;
    r *= 10;

    if (int_length + dec_length >= 10) {
      break;
    }
  }
  char buffer[15];
  dtostrf(result, int_length + dec_length, dec_length, buffer);
  str_result = String(buffer);
}


void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key - 48 >= 0 && key - 48 <= 9) {
      temp = temp * 10 + (key - 48);
      if(nums.size() == i + 1){
        nums.pop_back();
        nums.push_back(temp);
      }else{
        nums.push_back(temp);
      }
      display += key;
    } else if (key == 'C') {
      init();
    } else if (key == '=') { 
      temp = 0;
      i = 0;
      display = str_result;
      str_result = "";
      nums.clear();
      ops.clear();
      nums.push_back(result);
    } else {
      display += key;
      ops.push_back(key);
      i += 1;
      temp = 0;
    }
    calc(nums, ops);
    display_u8g2();
  }
}