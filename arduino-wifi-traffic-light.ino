#include "config.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Pins for ESP8266 via Wemos D1 Mini
#define D0 16
#define D1 5  // I2C Bus SCL (clock)
#define D2 4  // I2C Bus SDA (data)
#define D3 0
#define D4 2  // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3  // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#define RED D5
#define YELLOW D6
#define GREEN D7

// cp config.h.example config.h
// then edit the constants for config
const char* wifiSSID = WIFI_SSID;
const char* wifiPass = WIFI_PASS;
const char* httpUser = HTTP_USER;
const char* httpPass = HTTP_PASS;

int tries = 0;
ESP8266WebServer server(80);

uint8_t red = 0;
uint8_t yellow = 0;
uint8_t green = 1;

uint8_t flashRed = 0;
uint8_t flashYellow = 0;
uint8_t flashGreen = 0;


void setup() {
  // setup LEDs
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  setLights(LOW, HIGH, LOW);

  Serial.begin(115200);

  // setup wifi - STATUS blinking red
  WiFi.mode(WIFI_STA);
  setLights(HIGH, LOW, LOW);

  tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (tries % 10 == 0) {
      WiFi.begin(wifiSSID, wifiPass);
    }

    delay(1000);
    setLights(tries % 2 ? LOW : HIGH, LOW, LOW);
    tries++;
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  setLights(LOW, HIGH, LOW);

  // setup server - STATUS yellow
  server.on("/api/lights", lightsApi);

  server.begin();


  // done - STATUS green
  setLights(LOW, LOW, HIGH);
  delay(3000);
}


int loopCount = 0;
void loop() {
  if (flashRed && loopCount % flashRed == 0 ) {
    red = !red;
  }
  if (flashYellow && loopCount % flashYellow == 0 ) {
    yellow = !yellow;
  }
  if (flashGreen && loopCount % flashGreen == 0 ) {
    green = !green;
  }

  setLights(red, yellow, green);


  server.handleClient();

  delay(100);
  loopCount++;
}

void lightsApi() {
  if (!server.authenticate(httpUser, httpPass)) {
    return server.requestAuthentication();
  }

  String v = server.arg("red");
  red = v.toInt();
  v = server.arg("flashRed");
  flashRed = v.toInt();

  v = server.arg("yellow");
  yellow = v.toInt();
  v = server.arg("flashYellow");
  flashYellow = v.toInt();

  v = server.arg("green");
  green = v.toInt();
  v = server.arg("flashGreen");
  flashGreen = v.toInt();

  server.send(200, "text/plain", "OK");
}

void setLights(uint8_t red, uint8_t yellow, uint8_t green) {
  digitalWrite(RED, red);
  digitalWrite(YELLOW, yellow);
  digitalWrite(GREEN, green);
}
