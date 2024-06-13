#include <EEPROM.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int ll = 0;
byte data;
unsigned long long int num = 0;

void display();

void setup() {
  EEPROM.begin(257);
  u8g2.begin();

  ll = EEPROM.read(256);
}

void loop() {
  display();

  for (int i = 0; i <= ll; i++) {
    data = EEPROM.read(i);

    if (data < 255) {
      EEPROM.write(i, data + 1);
      break;
    }

    if (i == ll) {
      EEPROM.write(i, 0);
      ll++;
      EEPROM.write(256, ll);
      EEPROM.write(ll, 1);
      break;
    } else {
      EEPROM.write(i, 0);
    }
  }
  EEPROM.commit();
  delay(1000);
}

void display() {
  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.firstPage();

  num = 0;
  for (int i = 0; i <= ll; i++) {
    num += EEPROM.read(i) * pow(256, i);
  }

  do {
    u8g2.setCursor(64, 32);
    u8g2.print(String(num));
  } while (u8g2.nextPage());
}