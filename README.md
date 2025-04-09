# FM_Sendir
 Blátannarsendir fyrir gamla bíla, ESP32 tekur við í gegnum Bluetooth lagagögnum og sendir í gegnum lághleypisíu í FM módúl sem sendir analog gögnin út á FM bandi. Á sama tíma er skjár sem sýnir titilinn á laginu og artistanum. Powera ESP32 með USB en FM módúlinn sér með batterí og 5V regulator.

 ![EFA48659-3412-4048-BF77-7EDBC063A13A_1_105_c](https://github.com/user-attachments/assets/7c75a64b-cfd7-4da7-982d-982c3cc3a813)

Íhlutir:
- ESP32 - Devkit V1
- Digital FM Transmitter Module Stereo FM Transmitter DSP PLL 76.0-108.0MHz Stereo Frequency Modulation with LCD Display Line/USB/Mic Input, DC 3.0V - 5.0V
- 2.4inch LCD Display Module 240x320 Resolution 65K RGB Colors Embedded ILI9341 Driver SPI Interface, TFT Display Screen Compatible with Raspberry Pi 5 Pi 4B 3B+ 3B Pi Zero W WH Zero 2W/Ard/STM32
- 5V voltage regulator
- Þétta og viðnám til að sía burt suð

Notaði kóða frá þessum repositories:
- https://github.com/pschatzmann/ESP32-A2DP/blob/main/README.md
- https://github.com/n-delic/ESP32-FM-Transmitter/blob/main/radio_internaldac/radio_internaldac.ino
- https://github.com/adafruit/Adafruit_ILI9341
- https://github.com/pschatzmann/arduino-audio-tools
- https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.h
- https://github.com/adafruit/Adafruit-GFX-Library


Wiring fyrir LCD display:

• VCC → 3.3V on the ESP32

• GND → GND on the ESP32

• DIN (Data In) → GPIO 23 (MOSI)

• CLK (Clock) → GPIO 18 (SCK)

• CS (Chip Select) → GPIO 5 (or any available GPIO pin)

• DC (Data/Command) → GPIO 21 (or any available GPIO pin)

• RST (Reset) → GPIO 22 (or any available GPIO pin)

• BL (Backlight) → 3.3V (or 3.3V pin on ESP32)

