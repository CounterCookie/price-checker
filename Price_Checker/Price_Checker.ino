#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Fonts/FreeSans18pt7b.h>

#include "creds.h"


#define BARCODE_TX D4
#define BARCODE_RX D1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   D7
#define OLED_CLK   D5
#define OLED_DC    D2
#define OLED_CS    D8
#define OLED_RESET D3

const char* ssid = STASSID;
const char* password = STAPSK;

// API secrets
const char* key = KEY;
const char* secret = SECRET;

String host = HOST;
WiFiClientSecure client;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
SoftwareSerial barcodeReader(BARCODE_RX, BARCODE_TX);
HTTPClient http;

void setup() {

  // Initialize both Serial ports. change to preferred baud rate if necessary.
  Serial.begin(115200);
  barcodeReader.begin(9600);

  // Support streaming http 1.0 doesn't support reusing a single connection.
  http.useHTTP10(true);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Forget about sll verification.
  client.setInsecure();

  Serial.print("Connecting to ");
  Serial.println(host);

  if (!client.connect(host, 443)) {
    Serial.println("Connection failed");
    return;
  }

  while ( !display.begin(SSD1306_SWITCHCAPVCC) ){
    // Loop forever
    Serial.println("Display allocation failed.");
    delay(1000);
  }

  // Clear buffer and display nothing.
  display.print("");
  display.display();
  displaySetup();


}

void displaySetup(){
  // Display setup
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setFont(&FreeSans18pt7b);
  display.setTextSize(1);
  display.display();
}

void loop() {
  
  String barcode;
  
  while ( barcode.length() < 4 || barcodeReader.available() > 0 ) {
    barcode = barcodeReader.readStringUntil('\n');
  }
  
  displaySetup();

  // Remove the newline
  barcode.trim();


  Serial.println(barcode);
  
  String host = HOST;
  String url = "/wp-json/wc/v3/products?search=" + barcode + "&consumer_key=" + key + "&consumer_secret=" + secret;
  String full_address = "https://" + host + url;


  Serial.print("Requesting URL: ");
  Serial.println(full_address);

  Serial.println("http begin");
  http.begin(client, full_address);
  Serial.println(http.GET());
  // Just use a large chunk of ram until we can properly stream json.
  DynamicJsonDocument product(10000);
  deserializeJson(product, http.getStream());

  if (measureJson(product) < 100) {
    Serial.println("Cashier");
    display.setCursor(0, 40);
    display.print("Cashier");
    display.display();
    delay(3000);
    displaySetup();
    return;
  }
  
  const char* price = product[0]["price"];
  
  Serial.println(price);
  int priceLength = strlen(price);
  Serial.println(priceLength);

  // Adjust cursor position depending on price length.
  // This is only calibrated for the current font.
  if (priceLength == 4) {
    display.setCursor(32, 40);
  } else if (priceLength == 5) {
    display.setCursor(22, 40);
  } else if (priceLength == 6) {
    display.setCursor(12, 40);
  } else {
    display.setCursor(0, 40);
  }
  displaySetup();
  display.print(price);
  display.display();
  delay(3000);
  displaySetup();
}
