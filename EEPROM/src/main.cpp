#include <EEPROM.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const byte BUTTON[] = {39, 35};
int num = 0;

void display(int num);

void setup() {
  EEPROM.begin(512);
  u8g2.begin();
  for(byte i : BUTTON) {
    pinMode(i, INPUT);
  }

  EEPROM.get(0, num);
}

void loop() {
  for (int i = 0; i < 2; i++) {
    if (digitalRead(BUTTON[i]) == false) {
      //while (digitalRead(BUTTON[i]) == false);
      switch (i) {
        case 0:
          num++;
          break;
        case 1:
          num--;
          break;
      }
      EEPROM.put(0, num);
      EEPROM.commit();
    }
  }

  display(num);
}

void display(int num) {
  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.firstPage();
  do {
    u8g2.setCursor(64, 32);
    u8g2.print(num);
    
  } while (u8g2.nextPage());
}