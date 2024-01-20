#include <Wire.h>
#include <U8x8lib.h>

#define OLED_SDA 21  // SDA pin on ESP8266 NodeMCU
#define OLED_SCL 22  // SCL pin on ESP8266 NodeMCU

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(1, 1, "Hello!");
}

void loop() {
  // You can add other tasks here if needed
}

void updateOLED() {
  u8x8.clearDisplay();
}
