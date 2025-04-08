#include <BluetoothA2DPSink.h>
#include <AudioTools.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "IcelandicCharConverter.h"  // Your own character conversion header

// TFT display pins
#define TFT_CS     5
#define TFT_RST    22
#define TFT_DC     21
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Bluetooth and audio
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

// Buttons
const int btnPrev = 12;
const int btnPlayPause = 14;
const int btnNext = 27;

bool lastPrevState = HIGH;
bool lastPlayPauseState = HIGH;
bool lastNextState = HIGH;
bool isPlaying = false;

// Song info
String songTitle = "";
String songArtist = "";
String lastSongTitle = "";
String lastSongArtist = "";
bool wasConnected = false;
bool updateDisplay = false;

// Draw text in the center
void drawCenteredText(String text, int y, uint8_t textSize) {
  tft.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  String original = text;

  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  while (w > 310 && text.length() > 0) {
    text.remove(text.length() - 1);
    tft.getTextBounds(text + "...", 0, y, &x1, &y1, &w, &h);
  }

  if (text != original) text += "...";

  int16_t x = (320 - w) / 2;
  tft.setCursor(x, y);
  tft.println(text);
}

// Bluetooth metadata callback
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

// Playback status callback
void playback_status_callback(esp_avrc_playback_stat_t status) {
  isPlaying = (status == ESP_AVRC_PLAYBACK_PLAYING);
}

void setup() {
  Serial.begin(115200);

  // Display setup
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  drawCenteredText("Waiting for Bluetooth", 100, 2);

  // Buttons
  pinMode(btnPrev, INPUT_PULLUP);
  pinMode(btnPlayPause, INPUT_PULLUP);
  pinMode(btnNext, INPUT_PULLUP);

  // Bluetooth
  a2dp_sink.set_auto_reconnect(true);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.set_avrc_rn_playstatus_callback(playback_status_callback);
  a2dp_sink.start("ESP_FM_TRANS");
}

void loop() {
  // Button reading
  bool currentPrev = digitalRead(btnPrev);
  bool currentPlayPause = digitalRead(btnPlayPause);
  bool currentNext = digitalRead(btnNext);

  if (currentPrev == LOW && lastPrevState == HIGH) {
    Serial.println("Previous track");
    a2dp_sink.previous();
    delay(300);
  }

  if (currentPlayPause == LOW && lastPlayPauseState == HIGH) {
    if (isPlaying) {
      Serial.println("Pausing playback");
      a2dp_sink.pause();
    } else {
      Serial.println("Playing playback");
      a2dp_sink.play();
    }
    delay(300);
  }

  if (currentNext == LOW && lastNextState == HIGH) {
    Serial.println("Next track");
    a2dp_sink.next();
    delay(300);
  }

  // Update button states
  lastPrevState = currentPrev;
  lastPlayPauseState = currentPlayPause;
  lastNextState = currentNext;

  // Display connection status
  bool connected = a2dp_sink.is_connected();
  if (connected != wasConnected) {
    tft.fillScreen(ILI9341_BLACK);
    if (connected) {
      drawCenteredText("Bluetooth Connected!", 100, 2);
      updateDisplay = true;
    } else {
      drawCenteredText("Waiting for Bluetooth", 100, 2);
    }
    wasConnected = connected;
  }

  // Update song info if needed
  if (connected && (songTitle != lastSongTitle || songArtist != lastSongArtist || updateDisplay)) {
    tft.fillRect(0, 60, 320, 80, ILI9341_BLACK); // Clear old text
    drawCenteredText(songTitle, 90, 2);
    drawCenteredText(songArtist, 120, 2);

    lastSongTitle = songTitle;
    lastSongArtist = songArtist;
    updateDisplay = false;
  }

  delay(300);
}
