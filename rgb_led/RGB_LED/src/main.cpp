#include <Adafruit_NeoPixel.h>

#define PIN 32
#define NUMPIXLS 8
int di = 0;

Adafruit_NeoPixel pixels(NUMPIXLS, PIN, NEO_GRB + NEO_KHZ800);

byte colors[8][3] = {
    {255, 0, 0},
    {255, 255, 0},
    {0, 255, 0},
    {0, 0, 255},
    {128, 0, 255},
    {255, 255, 255},
    {128, 64, 0},
    {255, 128, 255},
};

void setup()
{
  pixels.begin();
}

void loop()
{
  pixels.clear();
  pixels.setBrightness(10);

  for (int i = 0; i < NUMPIXLS; i++)
  {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(colors[i][0], colors[i][1], colors[i][2]));
    delay(500);
    pixels.show();
  }

  while (true)
  {
    for (int i = 0; i < NUMPIXLS; i++)
    {
      pixels.setPixelColor(((i + di) % 8), pixels.Color(colors[i][0], colors[i][1], colors[i][2]));
    }
    pixels.show();
    di = (di + 1) % 8;
    delay(250);
  }
}
