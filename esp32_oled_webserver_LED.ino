#include <Wire.h>
#include <U8x8lib.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define OLED_SDA 21  // SDA pin on ESP32
#define OLED_SCL 22  // SCL pin on ESP32
#define LED_PIN 13   // GPIO pin for the LED on ESP32 WROVER

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(OLED_SCL, OLED_SDA, U8X8_PIN_NONE);

const char *ssid = "Your wifi ssid";
const char *password = "Your wifi password";

bool ledState = false;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  // Initialize the OLED display
  Wire.begin(OLED_SDA, OLED_SCL);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, " Welcome!");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure LED pin
  pinMode(LED_PIN, OUTPUT);

  // Print the local IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Serve the enhanced web page with dynamic updates
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><head><style>";
    html += "body {font-family: Arial, sans-serif; text-align: center;}";
    html += "h1 {color: #333;} button {padding: 10px; font-size: 16px;}";
    html += "</style>";
    html += "<script>";
    html += "function updateStatus(status) { document.getElementById('status').innerText = status; }";
    html += "function turnOn() { fetch('/on').then(response => response.text()).then(text => { updateStatus(text); }); }";
    html += "function turnOff() { fetch('/off').then(response => response.text()).then(text => { updateStatus(text); }); }";
    html += "</script></head><body>";
    html += "<h1>ESP32 OLED Control</h1>";
    html += "<button onclick='turnOn()'>Turn On</button>";
    html += "<button onclick='turnOff()'>Turn Off</button>";
    html += "<p>Status: <span id='status'>Unknown</span></p>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    ledState = true;
    updateOLED();
    request->send(200, "text/plain", "LED turned on");
  });
    
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    ledState = false;
    updateOLED();
    request->send(200, "text/plain", "LED turned off");
  });

  server.begin();
}

void loop() {
  // You can add other tasks here if needed
}

void updateOLED() {
  u8x8.clearDisplay();
  u8x8.drawString(0, 0, ledState ? " Mode: ON" : " Mode: OFF");

  // Update the physical LED state based on ledState
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}
