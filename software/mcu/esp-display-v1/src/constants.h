#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>
namespace CONST
{
  namespace UART
  {
    constexpr uint32_t BAUD = 115200;
  }
  namespace SPI
  {
    constexpr uint8_t SD_CS_PIN = 5;
    constexpr uint8_t SD_SCK_PIN = 18;
    constexpr uint8_t SD_MISO_PIN = 19;
    constexpr uint8_t SD_MOSI_PIN = 23;
  }
  namespace ASCII
  {
    constexpr uint8_t NEW_LINE    = 0x0A;
    constexpr uint8_t WHITE_SPACE = 0x0A;
    constexpr uint8_t ZER0        = 0x30;
  }
  namespace IMG_LANDSCAPE
  {
    const char * BABOON  = "/img_test/baboon-landscape.bmp";
    const char * FRUITS  = "/img_test/fruits-landscape.bmp";
    const char * LENNA   = "/img_test/lenna-landscape.bmp";
    const char * PEPPERS = "/img_test/peppers-landscape.bmp";
    const char * TULIPS  = "/img_test/tulips-landscape.bmp";
  }
  namespace IMG_PORTRAIT
  {
    const char * BABOON  = "/img_test/baboon-portrait.bmp";
    const char * FRUITS  = "/img_test/fruits-portrait.bmp";
    const char * LENNA   = "/img_test/lenna-portrait.bmp";
    const char * PEPPERS = "/img_test/peppers-portrait.bmp";
    const char * TULIPS  = "/img_test/tulips-portrait.bmp";
  }

}



#endif