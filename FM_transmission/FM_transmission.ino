#include <BluetoothA2DPSink.h>
#include <AudioTools.h>

// Output to DAC1 (GPIO 25)
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

// Button GPIOs
const int btnPrev = 12;
const int btnPlayPause = 14;
const int btnNext = 27;

// Store last states to detect press
bool lastPrevState = HIGH;
bool lastPlayPauseState = HIGH;
bool lastNextState = HIGH;

// Track playback state
bool isPlaying = false;  // Initially assume paused

// Callback for playback status
void playback_status_callback(esp_avrc_playback_stat_t status) {
  if (status == ESP_AVRC_PLAYBACK_PLAYING) {
    isPlaying = true;
  } else {
    isPlaying = false;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluetooth A2DP sink");

  // Start Bluetooth
  a2dp_sink.set_auto_reconnect(true);
  a2dp_sink.start("ESP_FM_TRANS");

  // Set the playback status callback
  a2dp_sink.set_avrc_rn_playstatus_callback(playback_status_callback);

  // Setup buttons with internal pull-up
  pinMode(btnPrev, INPUT_PULLUP);
  pinMode(btnPlayPause, INPUT_PULLUP);
  pinMode(btnNext, INPUT_PULLUP);
}

void loop() {
  // Read current states (LOW means pressed)
  bool currentPrev = digitalRead(btnPrev);
  bool currentPlayPause = digitalRead(btnPlayPause);
  bool currentNext = digitalRead(btnNext);

  // Previous button
  if (currentPrev == LOW && lastPrevState == HIGH) {
    Serial.println("Previous track");
    a2dp_sink.previous();
    delay(300); // Debounce delay
  }

  // Play/Pause toggle
  if (currentPlayPause == LOW && lastPlayPauseState == HIGH) {
    // Sync play/pause with the current playback state
    if (isPlaying) {
      Serial.println("Pausing playback");
      a2dp_sink.pause();
    } else {
      Serial.println("Playing playback");
      a2dp_sink.play();
    }
    delay(300);  // Debounce delay
  }

  // Next button
  if (currentNext == LOW && lastNextState == HIGH) {
    Serial.println("Next track");
    a2dp_sink.next();
    delay(300);  // Debounce delay
  }

  // Update last states
  lastPrevState = currentPrev;
  lastPlayPauseState = currentPlayPause;
  lastNextState = currentNext;
}
