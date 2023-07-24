#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Fonts/FreeMono9pt7b.h>

#include "creds.h"


#define BARCODE_TX D6
#define BARCODE_RX D7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET D3
#define I2C_ADDR 0x3C

const char* ssid = STASSID;
const char* password = STAPSK;

// API secrets
const char* key = KEY;
const char* secret = SECRET;

String host = HOST;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial barcodeReader(BARCODE_RX, BARCODE_TX);
WiFiClientSecure client;
HTTPClient http;

void setup() {
  // Initialize both Serial ports. change to preferred baud rate if necessary.
  Serial.begin(115200);
  barcodeReader.begin(9600);
  
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

  while ( !display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDR) ){
    // Loop forever
    Serial.println("Display allocation failed.");
    delay(1000);
  }

  // Clear buffer and display nothing.
  display.clearDisplay();
  display.display();


}


void loop() {
  // Display setup
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(1);
  display.display();
  
  String barcode;
  
  while ( barcodeReader.available() > 0 || barcode.length() < 1 ) {
    barcode = barcodeReader.readStringUntil('\n');
  }

  barcode.trim();

  Serial.println(barcode);
  
  String host = HOST;
  String url = "/wp-json/wc/v3/products?search=" + barcode + "&consumer_key=" + key + "&consumer_secret=" + secret;
  String full_address = "https://" + host + url;


  Serial.print("Requesting URL: ");
  Serial.println(full_address);

  Serial.println("http begin");
  http.useHTTP10(true);
  http.begin(client, full_address);
  Serial.println(http.GET());
  DynamicJsonDocument product(10000);
  deserializeJson(product, http.getStream());
  const char* price = product[0]["price"];

  Serial.println(price);
  
  display.print(price);
 
  display.display();
  delay(2000);
}
