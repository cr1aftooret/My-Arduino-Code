#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int main_selection = 1, sub_selection = 1;
const byte BUTTON[] = { 39, 35, 36, 34 };  // 0 : UP, 1 : DOWN, 2 : ENTER, 3 : BACK
const byte LED[4] = { 4, 0, 2, 15 };

enum State {
  MAIN,
  SUB
};

State state = MAIN;

void display_main() {
  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15 * main_selection);
    u8g2.print(" >");
    u8g2.setCursor(110, 15 * main_selection);
    u8g2.print("<");
    for (int i = 1; i <= 4; i++) {
      u8g2.setCursor(42, 15 * i);
      u8g2.print("LED " + String(i));
    }
  } while (u8g2.nextPage());
}

void display_sub() {
  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15);
    u8g2.print("< LED " + String(main_selection));
    u8g2.setCursor(0, 25 + 17 * sub_selection);
    u8g2.print(" >");
    u8g2.setCursor(110, 25 + 17 * sub_selection);
    u8g2.print("<");
    u8g2.setCursor(46, 42);
    u8g2.print("[ON]");
    u8g2.setCursor(42, 59);
    u8g2.print("[OFF]");
  } while (u8g2.nextPage());
}

void main_menu() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(BUTTON[i]) == false) {
      while (digitalRead(BUTTON[i]) == false)
        ;
      switch (i) {
        case 0:
          main_selection = (main_selection == 1) ? 4 : main_selection - 1;
          break;
        case 1:
          main_selection = (main_selection == 4) ? 1 : main_selection + 1;
          break;
        case 2:
          state = SUB;
          break;
      }

      switch (state) {
        case MAIN:
          display_main();
          break;
        case SUB:
          display_sub();
          break;
      }
    }
  }
}

void sub_menu() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTON[i]) == false) {
      while (digitalRead(BUTTON[i]) == false)
        ;
      switch (i) {
        case 0:
          sub_selection = (sub_selection == 1) ? 2 : sub_selection - 1;
          break;
        case 1:
          sub_selection = (sub_selection == 2) ? 1 : sub_selection + 1;
          break;
        case 2:
          sub_selection == 1 ? digitalWrite(LED[main_selection - 1], HIGH)
                             : digitalWrite(LED[main_selection - 1], LOW);
          break;
        case 3:
          state = MAIN;
          break;
      }

      switch (state) {
        case MAIN:
          display_main();
          break;
        case SUB:
          display_sub();
          break;
      }
    }
  }
}

void setup() {
  u8g2.begin();
  for (byte i : BUTTON) {
    pinMode(i, INPUT);
  }
  for (byte i : LED) {
    pinMode(i, OUTPUT);
  }
  display_main();
}

void loop() {
  switch (state) {
    case MAIN:
      main_menu();
      sub_selection = 1;
      break;
    case SUB:
      sub_menu();
      break;
  }
}