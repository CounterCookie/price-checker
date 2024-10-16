#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "creds.h"

// IMPORTED FROM 'creds.h'
//#ifndef STASSID
//#define STASSID "your-ssid"
//#define STAPSK "your-password"
//#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// API secrets
const char* key = KEY;
const char* secret = SECRET;

String host = HOST;


WiFiClientSecure client;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  Serial.println();
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

  client.setInsecure();

  Serial.print("Connecting to ");
  Serial.println(host);

  if (!client.connect(host, 443)) {
    Serial.println("Connection failed");
    return;
  }


}


void loop() {
  String barcode;
  while ( Serial.available() > 0 || barcode.length() < 1 ) {
    barcode = Serial.readStringUntil('\n');
  }

  String host = HOST;
  String url = "/wp-json/wc/v3/products?search=" + barcode + "&consumer_key=" + key + "&consumer_secret=" + secret;
  String full_address = "https://" + host + url;


  Serial.print("Requesting URL: ");
  Serial.println(url);

  Serial.println("http begin");
  http.useHTTP10(true);
  http.begin(client, full_address);


  http.GET();

  DynamicJsonDocument product(13500);

  deserializeJson(product, http.getStream());

  const char* price = product[0]["price"];

  Serial.println(price);
  

}
