#include "painter.h"

Painter::Painter()
{
  
}

void Painter::begin(TFT_eSPI *tft_in);
{
  for(uint32_t i = 0; i < PAINTER::BFR_SIZE; i++)
  {
    bfr[i] = 0;
  }
  tft = tft_in;
}




#include <TFT_eSPI.h>  // TFT display library
#include <SPI.h>
#include <SD.h>

TFT_eSPI tft = TFT_eSPI(); 

#define SD_CS 5 // Chip Select pin for SD card
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

void setup() {
    Serial.begin(115200);
    tft.begin();
    tft.setRotation(1);  // Adjust as needed
    tft.fillScreen(TFT_BLACK);

    if (!SD.begin(SD_CS)) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialized.");

    drawBMP("/image.bmp", 0, 0);
}

void loop() {
    // Nothing to do in loop
}

void drawBMP(const char *filename, int16_t x, int16_t y) {
    File bmpFile;
    int bmpWidth, bmpHeight;
    uint16_t bmpDepth;
    uint32_t bmpImageoffset;
    uint32_t rowSize;
    uint8_t sdbuffer[3 * 20];  // 20-pixel buffer
    uint16_t lcdbuffer[20];
    uint8_t buffidx = sizeof(sdbuffer);

    bmpFile = SD.open(filename);
    if (!bmpFile) {
        Serial.println("BMP file not found!");
        return;
    }

    if (read16(bmpFile) != 0x4D42) { // BMP signature check
        Serial.println("Not a BMP file!");
        bmpFile.close();
        return;
    }

    read32(bmpFile); // File size
    read32(bmpFile); // Reserved
    bmpImageoffset = read32(bmpFile);
    read32(bmpFile); // Header size
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    
    if (read16(bmpFile) != 1) { // Planes check
        bmpFile.close();
        return;
    }
    
    bmpDepth = read16(bmpFile);
    if (bmpDepth != 24) { // Only 24-bit BMP supported
        Serial.println("Only 24-bit BMP files are supported.");
        bmpFile.close();
        return;
    }

    read32(bmpFile); // Compression (should be 0 for uncompressed BMP)

    rowSize = (bmpWidth * 3 + 3) & ~3; // Row size (aligned to 4-byte boundary)
    bmpHeight = abs(bmpHeight);

    tft.startWrite();
    for (int row = 0; row < bmpHeight; row++) {
        bmpFile.seek(bmpImageoffset + (bmpHeight - 1 - row) * rowSize);
        for (int col = 0; col < bmpWidth; col++) {
            if (buffidx >= sizeof(sdbuffer)) {
                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                buffidx = 0;
            }
            
            uint8_t b = sdbuffer[buffidx++];
            uint8_t g = sdbuffer[buffidx++];
            uint8_t r = sdbuffer[buffidx++];
            lcdbuffer[col % 20] = tft.color565(r, g, b);

            if ((col % 20) == 19 || col == (bmpWidth - 1)) {
                tft.pushImage(x + col - (col % 20), y + row, 20, 1, lcdbuffer);
            }
        }
    }
    tft.endWrite();
    bmpFile.close();
}

// Read 16-bit value from file
uint16_t read16(File &f) {
    uint16_t result;
    f.read((uint8_t *)&result, 2);
    return result;
}

// Read 32-bit value from file
uint32_t read32(File &f) {
    uint32_t result;
    f.read((uint8_t *)&result, 4);
    return result;
}
