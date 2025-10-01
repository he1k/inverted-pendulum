#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <TFT_eSPI.h>
#define SD_CS_PIN 5
#define SD_SCK_PIN 18
#define SD_MISO_PIN 19
#define SD_MOSI_PIN 23
#define PRINT_DEBUG 1
const char* lenna_landscape = "/img_test/lenna-landscape.bmp";
const char* lenna_portrait = "/img_test/lenna-portrait.bmp";
File f_r;
TFT_eSPI tft = TFT_eSPI();



void bmpDraw(const char *filename, int16_t x, int16_t y);

uint16_t read16(File &f);

uint32_t read32(File &f);










void setup()
{
  Serial.begin(115200);
  digitalWrite(TFT_CS, HIGH);
  tft.init();
  tft.setRotation(1); // Rotate by 90 degrees
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Hello, World!", 10, 10);
  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  SPI.setDataMode(SPI_MODE0);
  if (!SD.begin(SD_CS_PIN)) {
    #ifdef PRINT_DEBUG
      Serial.println("Card Mount Failed");
    #endif
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    #ifdef PRINT_DEBUG
      Serial.println("No SD card attached");
    #endif
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  Serial.println("initialisation done.");
  // f_r = SD.open(lenna_landscape,"r",false);
  // if(!f_r)
  // {
  //   Serial.print("FAILED to open: "); Serial.println(lenna_landscape);
  //   while(1);
  // }else
  // {
  //   Serial.print("SUCCEEDED to open: "); Serial.println(lenna_landscape);
  // }
  bmpDraw("/img_test/parrot.bmp", 0, 0);
  // if (!bmpDraw(lenna_landscape, 0, 0)) {
  //   Serial.println("Failed to draw image.");
  // }
}
void loop()
{
  Serial.println("Hello");
  delay(1000);
}

void bmpDraw(const char *filename, int16_t x, int16_t y) {

  if ((x >= tft.width()) || (y >= tft.height())) return;

  fs::File bmpFS;

  // Open requested file on SD card
  bmpFS = SD.open(filename, "r");

  if (!bmpFS)
  {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t  r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42)
  {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
    {
      y += h - 1;

      bool oldSwapBytes = tft.getSwapBytes();
      tft.setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t*  bptr = lineBuffer;
        uint16_t* tptr = (uint16_t*)lineBuffer;
        // Convert 24 to 16-bit colours
        for (uint16_t col = 0; col < w; col++)
        {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        tft.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
      }
      tft.setSwapBytes(oldSwapBytes);
      Serial.print("Loaded in "); Serial.print(millis() - startTime);
      Serial.println(" ms");
    }
    else Serial.println("BMP format not recognized.");
  }
  bmpFS.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

// bool bmpDraw(const char *filename, int16_t x, int16_t y) {
//   File bmpFile;
//   int bmpWidth, bmpHeight;   // W+H in pixels
//   uint8_t bmpDepth;          // Bit depth (must be 24)
//   uint32_t bmpImageoffset;   // Start of image data
//   uint32_t rowSize;
//   uint8_t sdbuffer[3*20];    // 20 pixels buffer
//   uint8_t buffidx = sizeof(sdbuffer);
//   bool goodBmp = false;
//   bool flip = true;        // BMP is stored bottom-to-top
//   int w, h, row, col;
//   uint8_t r, g, b;
//   uint32_t pos = 0;

//   bmpFile = SD.open(filename);
//   if (!bmpFile) {
//     Serial.println("File not found");
//     return false;
//   }

//   if (read16(bmpFile) == 0x4D42) { // BMP signature
//     read32(bmpFile); // File size
//     read32(bmpFile); // Creator bytes
//     bmpImageoffset = read32(bmpFile); // Start of image data
//     read32(bmpFile); // DIB header size
//     bmpWidth = read32(bmpFile);
//     bmpHeight = read32(bmpFile);
//     if (read16(bmpFile) == 1) { // # planes
//       bmpDepth = read16(bmpFile); // bits per pixel
//       if (bmpDepth == 24) {
//         goodBmp = true;
//         rowSize = (bmpWidth * 3 + 3) & ~3;
//         if (bmpHeight < 0) {
//           bmpHeight = -bmpHeight;
//           flip = false;
//         }
//         w = bmpWidth;
//         h = bmpHeight;

//         tft.setSwapBytes(true);
//         tft.startWrite();
//         tft.setAddrWindow(x, y, w, h);

//         for (row = 0; row < h; row++) {
//           if (flip)
//             pos = bmpImageoffset + (h - 1 - row) * rowSize;
//           else
//             pos = bmpImageoffset + row * rowSize;

//           bmpFile.seek(pos);
//           for (col = 0; col < w; col++) {
//             b = bmpFile.read();
//             g = bmpFile.read();
//             r = bmpFile.read();
//             uint16_t color = tft.color565(b, g, r);//tft.color565(r, g, b);
//             tft.pushColor(color);
//           }
//         }
//         tft.endWrite();
//       }
//     }
//   }
//   bmpFile.close();
//   return goodBmp;
// }

// uint16_t read16(File &f) {
//   uint16_t result;
//   ((uint8_t *)&result)[0] = f.read(); // LSB
//   ((uint8_t *)&result)[1] = f.read(); // MSB
//   return result;
// }

// uint32_t read32(File &f) {
//   uint32_t result;
//   ((uint8_t *)&result)[0] = f.read(); 
//   ((uint8_t *)&result)[1] = f.read(); 
//   ((uint8_t *)&result)[2] = f.read(); 
//   ((uint8_t *)&result)[3] = f.read(); 
//   return result;
// }