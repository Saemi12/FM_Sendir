#include <BluetoothA2DPSink.h>

BluetoothA2DPSink a2dp_sink;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluetooth A2DP Sink...");

  // Start A2DP Sink with internal DAC output (GPIO 25 / DAC1)
  a2dp_sink.set_output(audio_output);
  a2dp_sink.start("ESP32_FM_Stream");
}

void loop() {
  // Nothing needed here, audio is handled by callbacks
}

void audio_output(const uint8_t *data, uint32_t len) {
  // Send audio data to DAC1 (GPIO25)
  for (uint32_t i = 0; i < len; i++) {
    dacWrite(25, data[i]); // scale or average if stereo
  }
}