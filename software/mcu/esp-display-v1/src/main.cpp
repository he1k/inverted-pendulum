#include <Arduino.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    tft.begin();
    tft.setRotation(1);  // Try 0, 1, 2, or 3 for different orientations
}

void loop()
{
  tft.fillScreen(TFT_RED);  // Should turn the screen red
  delay(1000);
  tft.fillScreen(TFT_GREEN); // Should turn the screen green
  delay(1000);
  tft.fillScreen(TFT_BLUE);  // Should turn the screen blue
  delay(1000);
}
// #include <FS.h>
// #include <SPI.h>
// #include <SD.h>

// TFT_eSPI tft = TFT_eSPI(); 

// #define SD_CS 5 // Chip Select pin for SD card
// #define TFT_WIDTH  240
// #define TFT_HEIGHT 320


// uint16_t read16(File &f);

// uint32_t read32(File &f);

// void drawBMP(const char *filename, int16_t x, int16_t y);

// void setup() {
//     Serial.begin(115200);
//     digitalWrite(22, HIGH); // Touch controller chip select (if used)
//     digitalWrite(15, HIGH); // TFT screen chip select
//     digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)
//     delay(1000);
//     tft.begin();
//     if (!SD.begin(SD_CS)) {
//         Serial.println("SD Card initialization failed!");
//         return;
//     }
//     Serial.println("SD Card initialized.");
//     delay(4000);
//     tft.setRotation(1);  // Adjust as needed
//     tft.fillScreen(TFT_BLACK);
//     drawBMP("/img_test/baboon-landscape.bmp", 0, 0);
// }

// void loop() {
//     // Nothing to do in loop
// }

// void drawBMP(const char *filename, int16_t x, int16_t y) {
//     File bmpFile;
//     int bmpWidth, bmpHeight;
//     uint16_t bmpDepth;
//     uint32_t bmpImageoffset;
//     uint32_t rowSize;
//     uint8_t sdbuffer[3 * 20];  // 20-pixel buffer
//     uint16_t lcdbuffer[20];
//     uint8_t buffidx = sizeof(sdbuffer);

//     bmpFile = SD.open(filename);
//     if (!bmpFile) {
//         Serial.println("BMP file not found!");
//         return;
//     }

//     if (read16(bmpFile) != 0x4D42) { // BMP signature check
//         Serial.println("Not a BMP file!");
//         bmpFile.close();
//         return;
//     }

//     read32(bmpFile); // File size
//     read32(bmpFile); // Reserved
//     bmpImageoffset = read32(bmpFile);
//     read32(bmpFile); // Header size
//     bmpWidth  = read32(bmpFile);
//     bmpHeight = read32(bmpFile);
    
//     if (read16(bmpFile) != 1) { // Planes check
//         bmpFile.close();
//         return;
//     }
    
//     bmpDepth = read16(bmpFile);
//     if (bmpDepth != 24) { // Only 24-bit BMP supported
//         Serial.println("Only 24-bit BMP files are supported.");
//         bmpFile.close();
//         return;
//     }

//     read32(bmpFile); // Compression (should be 0 for uncompressed BMP)

//     rowSize = (bmpWidth * 3 + 3) & ~3; // Row size (aligned to 4-byte boundary)
//     bmpHeight = abs(bmpHeight);

//     tft.startWrite();
//     for (int row = 0; row < bmpHeight; row++) {
//         bmpFile.seek(bmpImageoffset + (bmpHeight - 1 - row) * rowSize);
//         for (int col = 0; col < bmpWidth; col++) {
//             if (buffidx >= sizeof(sdbuffer)) {
//                 bmpFile.read(sdbuffer, sizeof(sdbuffer));
//                 buffidx = 0;
//             }
            
//             uint8_t b = sdbuffer[buffidx++];
//             uint8_t g = sdbuffer[buffidx++];
//             uint8_t r = sdbuffer[buffidx++];
//             lcdbuffer[col % 20] = tft.color565(r, g, b);

//             if ((col % 20) == 19 || col == (bmpWidth - 1)) {
//                 tft.pushImage(x + col - (col % 20), y + row, 20, 1, lcdbuffer);
//             }
//         }
//     }
//     tft.endWrite();
//     bmpFile.close();
// }

// // Read 16-bit value from file
// uint16_t read16(File &f) {
//     uint16_t result;
//     f.read((uint8_t *)&result, 2);
//     return result;
// }

// // Read 32-bit value from file
// uint32_t read32(File &f) {
//     uint32_t result;
//     f.read((uint8_t *)&result, 4);
//     return result;
// }

















// // This sketch if for an ESP32, it draws Jpeg images pulled from an SD Card
// // onto the TFT.

// // As well as the TFT_eSPI library you will need the JPEG Decoder library.
// // A copy can be downloaded here, it is based on the library by Makoto Kurauchi.
// // https://github.com/Bodmer/JPEGDecoder

// // Images on SD Card must be put in the root folder (top level) to be found
// // Use the SD library examples to verify your SD Card interface works!

// // The example images used to test this sketch can be found in the library
// // JPEGDecoder/extras folder
// //----------------------------------------------------------------------------------------------------

// #include <SPI.h>

// #include <FS.h>
// #include <SD.h>

// #include <TFT_eSPI.h>
// #include "constants.h"
// #include "colors.h"
// #include "ascii.h"
// #include "storage.h"
// TFT_eSPI tft = TFT_eSPI();
// Storage mem;
// #define PRINT_DEBUG
// #define IMG_PATH "/img/lenna.ppm"

// // JPEG decoder library

// //####################################################################################################
// // Setup
// //####################################################################################################
// #define TFT_GREY 0x7BEF

// unsigned long runTime = 0;

// float sx = 0, sy = 0;
// uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;

// unsigned int rainbow(int value);
// uint16_t to_num(uint8_t D1, uint8_t D2, uint8_t D3);
// uint16_t read_line();
// uint16_t get_pixel();
// File file;
// void setup() {
//   Serial.begin(CONST::UART::BAUD);

//   // Set all chip selects high to avoid bus contention during initialisation of each peripheral
//   digitalWrite(22, HIGH); // Touch controller chip select (if used)
//   digitalWrite(15, HIGH); // TFT screen chip select
//   digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)
//   delay(1000);
//   tft.begin();
//   if(mem.begin(CONST::SPI::SD_CS_PIN))
//   {
//     Serial.println("SD Card: SUCCESS");
//   }else
//   {
//     Serial.println("SD Card: FAILURE");
//     while(1){};
//   }
//   mem.print_card_details();
//   while(1){};
// //   SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
// //   SPI.setDataMode(SPI_MODE0);
// //   if (!SD.begin(SD_CS_PIN)) {
// //     #ifdef PRINT_DEBUG
// //       Serial.println("Card Mount Failed");
// //     #endif
// //     return;
// //   }
  
// //   Serial.println("initialisation done.");
// //   file = SD.open(IMG_PATH);
// //   if(!file){
// //     Serial.println("Failed open file");
// //   }else{
// //     Serial.println("Success");  
// //   }
// //   char format[2];
// //   format[0] = (char) file.read();
// //   format[1] = (char) file.read();
// //   if(file.read() != NEW_LINE){
// //     Serial.println("Missing new line");
// //   }
// //   if(file.read() == 35){
// //     Serial.println("Comment included");
// //   }else{
// //     Serial.println("No comment included");
// //   }
// //   uint8_t cnt = 0;
// //   while(file.read() != NEW_LINE){
// //     cnt++;
// //     if(cnt == 255){
// //       Serial.println("Too long comment");
// //       break;
// //     }
// //   }
// //   Serial.print("Comment size: ");
// //   Serial.println(cnt);
// //   uint8_t b = file.read();
// //   uint16_t width = 0, height = 0;
// //   uint8_t d[3];
// //   uint8_t idx = 0;
// //   while(1){
// //     Serial.println(idx);
// //     if(b == WHITE_SPACE)
// //     {
// //       width = to_num(d[0], d[1], d[2]);
// //       Serial.print("Width = ");
// //       Serial.println(width);
// //       idx = 0;
// //     }else if(b == NEW_LINE)
// //     {
// //       height = to_num(d[0], d[1], d[2]);
// //       Serial.print("Height = ");
// //       Serial.println(height);
// //       break;
// //     }else{
// //       d[idx] = b;
// //       idx++;
// //     }
// //     b = file.read();
// //   }
//   delay(4000);
//   tft.setRotation(3);
// }

// void loop()
// {
//   runTime = millis();

//   tft.fillScreen(TFT_BLACK);
//   tft.startWrite();
//   uint32_t line = 1;
//   for (int px = 1; px < 320; px++)
//   {
//     for (int py = 0; py < 240; py++)
//     {
//       float x0 = (map(px, 0, 320, -250000/2, -242500/2)) / 100000.0; //scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.5, 1))
//       float yy0 = (map(py, 0, 240, -75000/4, -61000/4)) / 100000.0; //scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1, 1))
//       float xx = 0.0;
//       float yy = 0.0;
//       int iteration = 0;
//       int max_iteration = 128;
//       while ( ((xx * xx + yy * yy) < 4)  &&  (iteration < max_iteration) )
//       {
//         float xtemp = xx * xx - yy * yy + x0;
//         yy = 2 * xx * yy + yy0;
//         xx = xtemp;
//         iteration++;
//       }
//       int color = rainbow((3*iteration+64)%128);
//       color = COL_GREEN;
//       if(py > 30){
//         color = COL_MAGENTA;
//       }
//       if(py > 60){
//         color = COL_CYAN;
//       }
//       if(py > 90){
//         color = COL_BROWN;
//       }
//       color = get_pixel();
//       tft.drawPixel(px, py, color);
//       delay(50);
//       #ifdef PRINT_DEBUG
//         Serial.print("px = "); Serial.print(px); Serial.print("py = "); Serial.println(py);
//         Serial.print("line: "); Serial.print(line);
//       #endif
//       line++;
//     }
//   }
//   tft.endWrite();

//   Serial.println(millis()-runTime);
//   while(1) yield();
// }

// unsigned int rainbow(int value)
// {
//   // Value is expected to be in range 0-127
//   // The value is converted to a spectrum colour from 0 = blue through to red = blue

//   byte red = 0; // Red is the top 5 bits of a 16-bit colour value
//   byte green = 0;// Green is the middle 6 bits
//   byte blue = 0; // Blue is the bottom 5 bits

//   byte quadrant = value / 32;

//   if (quadrant == 0) {
//     blue = 31;
//     green = 2 * (value % 32);
//     red = 0;
//   }
//   if (quadrant == 1) {
//     blue = 31 - (value % 32);
//     green = 63;
//     red = 0;
//   }
//   if (quadrant == 2) {
//     blue = 0;
//     green = 63;
//     red = value % 32;
//   }
//   if (quadrant == 3) {
//     blue = 0;
//     green = 63 - 2 * (value % 32);
//     red = 31;
//   }
//   return (red << 11) + (green << 5) + blue;
// }
// uint16_t to_num(uint8_t D1, uint8_t D2, uint8_t D3)
// {
//   uint8_t D1_conv = D1 - ASCII_ZERO;
//   uint8_t D2_conv = D2 - ASCII_ZERO;
//   uint8_t D3_conv = D3 - ASCII_ZERO;
//   if(((D1_conv < 0) || (D1_conv > 9)) || ((D2_conv < 0) || (D2_conv > 9)) || ((D3_conv < 0) || (D3_conv > 9)))
//   {
//     return -1;
//   }else{
//     return D1_conv*100 + D2_conv*10 + D3_conv*1;
//   }
// }
// // 320x240
// uint16_t read_line(){
//   uint8_t d[4] = {ASCII_ZERO,ASCII_ZERO,ASCII_ZERO,ASCII_ZERO};
//   Serial.println("Read line start");
//   for(int i = 0; i < 4; i++){
//     Serial.print("i = "); Serial.println(i);
//     d[0] = d[1];
//     d[1] = d[2];
//     d[2] = d[3];
//     d[3] = file.read();
//     if(d[3] == NEW_LINE){
//       break;
//     }
//   }
//   Serial.println("Loop done");
//   uint16_t res = to_num(d[0], d[1], d[2]);
//   if(d[3] == NEW_LINE){
//     #ifdef PRINT_DEBUG
//       Serial.print("Read: "); Serial.println(res);
//     #endif
//   }else{
//     #ifdef PRINT_DEBUG
//       Serial.println("ERROR: Missing new line after 3 byte reads");
//     #endif
//   }
//   return res;
// }
// uint16_t get_pixel(){
//   uint8_t red = (0x00FF & read_line());
//   uint8_t green = (0x00FF & read_line());
//   uint8_t blue = (0x00FF & read_line());
//   return (red << 11) + (green << 5) + blue;
// }