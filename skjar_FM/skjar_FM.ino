#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <BluetoothA2DPSink.h>
#include "IcelandicCharConverter.h"  // Your unchanged header

#define TFT_CS     5
#define TFT_RST    22
#define TFT_DC     21

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
BluetoothA2DPSink a2dp_sink;

String songTitle = "";
String songArtist = "";
String lastSongTitle = "";
String lastSongArtist = "";
bool wasConnected = false;
bool UpdateBlue = false;

void drawCenteredText(String text, int y, uint8_t textSize) {
  tft.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  String original = text;

  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);

  // Truncate with "..." if it’s too wide
  while (w > 310 && text.length() > 0) {
    text.remove(text.length() - 1);
    tft.getTextBounds(text + "...", 0, y, &x1, &y1, &w, &h);
  }

  // Only add "..." if we actually had to truncate
  if (text != original) {
    text += "...";
  }

  int16_t x = (320 - w) / 2;
  tft.setCursor(x, y);
  tft.println(text);
}

void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
  String raw = String((char*)data2);
  if (data1 == 0x01) {
    songTitle = convertIcelandicChars(raw);
    Serial.printf("Title: %s\n", songTitle.c_str());
  } else if (data1 == 0x02) {
    songArtist = convertIcelandicChars(raw);
    Serial.printf("Artist: %s\n", songArtist.c_str());
  }
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  drawCenteredText("Waiting for Bluetooth", 100, 2);

  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.start("ESP_FM_Transmitter");
}

void loop() {
  bool connected = a2dp_sink.is_connected();

  if (connected != wasConnected) {
    tft.fillScreen(ILI9341_BLACK);
    if (connected) {
      drawCenteredText("Bluetooth Connected!", 100, 2);
      UpdateBlue = true;
    } else {
      drawCenteredText("Waiting for Bluetooth", 100, 2);
    }
    wasConnected = connected;
  }

  if (connected) {
    if (songTitle != lastSongTitle || songArtist != lastSongArtist || UpdateBlue) {
      tft.fillRect(0, 60, 320, 60, ILI9341_BLACK);  // Clear song area
      drawCenteredText(songTitle, 90, 2);
      drawCenteredText(songArtist, 120, 2);

      lastSongTitle = songTitle;
      lastSongArtist = songArtist;
      UpdateBlue = false;
    }
  }

  delay(1000);
}