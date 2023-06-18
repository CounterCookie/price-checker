/*
 * Simple barcode scanner bit. just to confirm that barcode scanner works.
 * Connect TX, RX to D3 and D4 respectively
 * 
 */
#include <HyperDisplay_SSD1309.h>
#include <SoftwareSerial.h>

#define BARCODE_TX D6
#define BARCODE_RX D7
SoftwareSerial barcode(BARCODE_RX, BARCODE_TX);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello dolly!");
  barcode.begin(9600);
}

void loop(){


  if (barcode.available())
  {
    Serial.println("data");
    while (barcode.available())
    {
      char input = barcode.read();
      Serial.print(input);
      delay(5);
    }
    Serial.println();
  }
}
