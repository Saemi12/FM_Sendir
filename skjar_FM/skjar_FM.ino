#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <BluetoothA2DPSink.h>
#include "IcelandicCharConverter.h"  // Include the header file

#define TFT_CS     5
#define TFT_RST    22
#define TFT_DC     21

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
BluetoothA2DPSink a2dp_sink;

char songTitle[64] = "";
char songArtist[64] = "";
bool wasConnected = false;

void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
  if (data1 == 0x01) {
    strncpy(songTitle, (char*)data2, sizeof(songTitle) - 1);
    songTitle[sizeof(songTitle) - 1] = '\0';
    songTitle[63] = '\0';  // Ensure null termination
    Serial.printf("Title: %s\n", songTitle);
  } else if (data1 == 0x02) {
    strncpy(songArtist, (char*)data2, sizeof(songArtist) - 1);
    songArtist[sizeof(songArtist) - 1] = '\0';
    songArtist[63] = '\0';  // Ensure null termination
    Serial.printf("Artist: %s\n", songArtist);
  }
}

// Helper to center text and truncate if too long
void drawCenteredText(const char* text, int y, uint8_t textSize) {
  tft.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);

  // Truncate if it doesn't fit
  String str = text;
  while (w > 240 && str.length() > 0) {
    str.remove(str.length() - 1);
    tft.getTextBounds(str + "...", 0, y, &x1, &y1, &w, &h);
  }

  if (str != text) {
    str += "...";  // Add ellipsis
  }

  int16_t x = (240 - w) / 2;
  tft.setCursor(x, y);
  tft.println(str);
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);

  // Initial wait message
  tft.setTextSize(2);
  drawCenteredText("Waiting for BT...", 100, 2);

  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.start("ESP_FM_Transmitter");
}

void loop() {
  bool connected = a2dp_sink.is_connected();

  if (connected && !wasConnected) {
    tft.fillScreen(ILI9341_BLACK);
    wasConnected = true;
  } else if (!connected && wasConnected) {
    tft.fillScreen(ILI9341_BLACK);
    drawCenteredText("Waiting for BT...", 100, 2);
    wasConnected = false;
  }

  if (connected) {
    // Convert song title and artist to simpler characters
    String convertedTitle = convertIcelandicChars(songTitle);
    String convertedArtist = convertIcelandicChars(songArtist);

    // Show title and artist
    tft.fillRect(0, 60, 240, 60, ILI9341_BLACK);  // Clear display area
    drawCenteredText(convertedTitle.c_str(), 60, 2);
    drawCenteredText(convertedArtist.c_str(), 90, 2);
  }

  delay(1000);
}