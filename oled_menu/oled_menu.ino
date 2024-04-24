#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int selection = 1;
const byte up = 39, down = 35;

void display_u8g2() {
  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15 * selection);
    u8g2.print(" >");
    for (int i = 1; i <= 4; i++) {
      u8g2.setCursor(36, 15 * i);
      u8g2.print("Option " + String(i));
    }
  } while (u8g2.nextPage());
}

void setup() {
  u8g2.begin();
  pinMode(up, INPUT);
  pinMode(down, INPUT);
}

void loop() {
  if (digitalRead(up) == false && selection > 1) {
    selection--;
  }

  if (digitalRead(down) == false && selection < 4) {
    selection++;
  }

  display_u8g2();
  delay(100);
}
