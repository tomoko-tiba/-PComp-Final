
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST77xx.h>

#include <Adafruit_GFX.h>         // Core graphics library
#include <SdFat.h>                // SD card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

// Comment out the next line to load from SPI/QSPI flash instead of SD card:
#define USE_SD_CARD

// TFT display and SD card share the hardware SPI interface, using
// 'select' pins for each to identify the active device on the bus.

#define SD_CS    7 // SD card select pin
#define TFT_CS  10 // TFT select pin
#define TFT_DC   9 // TFT display/command pin
#define TFT_RST  8 // Or set to -1 and connect to Arduino RESET pin

#if defined(USE_SD_CARD)
SdFat                SD;         // SD card filesystem
Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
#else
// SPI or QSPI flash filesystem (i.e. CIRCUITPY drive)
#if defined(__SAMD51__) || defined(NRF52840_XXAA)
Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS,
    PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
#else
#if (SPI_INTERFACES_COUNT == 1)
Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
#else
Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
#endif
#endif
Adafruit_SPIFlash    flash(&flashTransport);
FatFileSystem        filesys;
Adafruit_ImageReader reader(filesys); // Image-reader, pass in flash filesys
#endif

Adafruit_ST7789      tft    = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Adafruit_Image       img;        // An image loaded into RAM
int32_t              width  = 0, // BMP image dimensions
                     height = 0;
int inByte = 0;
           
void setup() {
  Serial.begin(9600);
  ImageReturnCode stat; // Status from image-reading functions
  tft.init(240, 320);           // Init ST7789 240x320
  SD.begin(SD_CS, SD_SCK_MHZ(10));
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);               // 设置要显示的文字坐标
  tft.setTextColor(ST77XX_GREEN);    // 设置文字颜色
  tft.setTextSize(4);                // 设置文字尺寸
  tft.println("Virtual");            // 显示内容
  tft.println("Flower");
  tft.println("Planting");
  tft.println("Experiment");
  delay(1000);
  
}

void loop() {
  if (Serial.available() > 0) {
    //Serial.println("read");
    inByte = Serial.read();
    //Serial.println(inByte);
  }
  inputEmotion(inByte);
}


void inputEmotion(int b){
  ImageReturnCode stat;
  tft.setRotation(2);
  switch(b) {
    case 1:
      stat = reader.drawBMP("/1.bmp", tft, 0, 0);
      delay(1000);
      break;
    case 2:
      stat = reader.drawBMP("/2.bmp", tft, 0, 0);
    delay(1000);
      break;
    case 3:
      stat = reader.drawBMP("/3.bmp", tft, 0, 0);
    delay(1000);
      break;
    case 4:
      stat = reader.drawBMP("/4.bmp", tft, 0, 0);
    delay(1000);
      break;
    case 5:
      stat = reader.drawBMP("/5.bmp", tft, 0, 0);
    delay(1000);
      break;
    case 6:
      stat = reader.drawBMP("/6.bmp", tft, 0, 0);
    delay(1000);
      break;
    case 7:
      stat = reader.drawBMP("/7.bmp", tft, 0, 0);
    delay(1000);
      break; 
  }
}
