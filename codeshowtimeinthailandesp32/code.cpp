#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// Display pins
#define TFT_CS     5  // Chip select
#define TFT_RST    17 // Reset pin
#define TFT_DC     16 // Data/command
#define TFT_SCLK   18 // SCK
#define TFT_MOSI   23 // SDA

// WiFi credentials
const char* ssid = "FeeMing_2.4G";  // Replace with your SSID
const char* password = "feekritsadayu48";  // Replace with your WiFi password

// NTP Client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200); // 25200 seconds offset for UTC+7

// Initialize Adafruit ST7735
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  tft.initR(INITR_BLACKTAB);  // Initialize the display
  tft.fillScreen(ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  timeClient.begin();  // Start NTP Client
}

void displayTime() {
  if (timeClient.update()) {
    String formattedTime = timeClient.getFormattedTime();
  
    tft.fillScreen(ST77XX_GREEN); // Clear the screen
    tft.setTextSize(1);
    tft.setCursor(0, 10);
    tft.println("Current Time Now:");

    tft.setTextSize(2); // Larger text for time
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(formattedTime, 0, 0, &x1, &y1, &w, &h); // Get width and height of the text
    tft.setCursor((tft.width() - w) / 2, (tft.height() - h) / 2); // Center text horizontally and vertically
    tft.println(formattedTime);
  } else {
    timeClient.forceUpdate();  // Force an update if failed
  }
}


void loop() {
  static unsigned long lastTimeUpdate = 0;
  if (millis() - lastTimeUpdate > 60000) { // Update the time every min
    lastTimeUpdate = millis();
    displayTime();
  }
}