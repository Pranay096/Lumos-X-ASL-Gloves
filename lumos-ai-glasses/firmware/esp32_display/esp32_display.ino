#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_CS   5
#define OLED_DC   2
#define OLED_RST  4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RST, OLED_CS);

const char* ssid = "Your Wifi SSID";
const char* password = "Your Wifi Password";

WiFiServer server(1234);

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.begin();

  display.clearDisplay();
  display.println("READY");
  display.display();

  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String text = client.readStringUntil('\n');

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(text);
    display.display();

    client.stop();
  }
}