
#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#define BAUD_RATE 115200

#define TFT_BL 21
#define TFT_CS 15
#define TFT_DC 2
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_RST -1


SPIClass hspi = SPIClass(HSPI);

//Adafruit_ILI9341 tft = Adafruit_ILI9341(&hspi, TFT_DC, TFT_CS, TFT_RST);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);
void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  digitalWrite(TFT_CS, HIGH); // TFT screen chip select
  //digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)
  delay(1000);
  tft.begin();
  delay(2000);
  Serial.println("Display initialized, starting test sequence now...");
  //tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
  for(uint8_t i = 0; i < 4; i++)
  {
    Serial.print("Rotation: "); Serial.print(i); Serial.print(", color: "); Serial.println("Black");
    tft.setRotation(i);
    tft.fillScreen(ILI9341_RED);
    delay(3000);
    Serial.print("Rotation: "); Serial.print(i); Serial.print(", color: "); Serial.println("Black");
    tft.setRotation(i);
    tft.fillScreen(ILI9341_BLACK);
    delay(3000);
  }
  // tft.fillScreen(ILI9341_RED); // Should turn the screen red
  // delay(1000);
  // tft.fillScreen(ILI9341_GREEN); // Should turn the screen green
  // delay(1000);
  // tft.fillScreen(ILI9341_BLUE); // Should turn the screen blue
  // delay(1000);
}
