#include <Arduino.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
void setup()
{
  Serial.begin(115200);
  digitalWrite(TFT_CS, HIGH);
  tft.init();
  tft.setRotation(1); // Rotate by 90 degrees
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Hello, World!", 10, 10);
}
void loop()
{
  Serial.println("Hello");
  delay(1000);
}