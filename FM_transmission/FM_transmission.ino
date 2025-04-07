#include <BluetoothA2DPSink.h>
#include <AudioTools.h>

// Output to DAC1 (GPIO 25)
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluetooth A2DP sink");

  // Start Bluetooth and route to DAC
  a2dp_sink.set_auto_reconnect(true);
  a2dp_sink.start("ESP_FM_TRANS"); // Name that appears on your phone
}

void loop() {
  // Nothing needed here; everything happens in background
}