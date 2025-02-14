#ifndef PAINTER_H
#define PAINTER_H
#include <Arduino.h>
#include <TFT_eSPI.h>
namespace PAINTER
{
  constexpr uint16_t WIDTH  = 320;
  constexpr uint16_t HEIGHT = 240;
  constexpr uint8_t  BYTES_PER_PIXEL = 3;
  constexpr uint32_t BFR_SIZE = WIDTH * HEIGHT * BYTES_PER_PIXEL;
}
class Painter
{
  private:
    uint8_t bfr[PAINTER::BFR_SIZE];
    TFT_eSPI *tft;
  public:
    Painter();
    void begin(TFT_eSPI *tft_in);
};
#endif