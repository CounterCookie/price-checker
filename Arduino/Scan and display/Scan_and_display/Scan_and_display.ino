/*
 * Simple barcode scanner bit. just to confirm that barcode scanner works.
 * Connect TX, RX to D3 and D4 respectively
 * 
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Fonts/FreeMono9pt7b.h>


#define BARCODE_TX D6
#define BARCODE_RX D7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET D3
#define I2C_ADDR 0x3C


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial barcode(BARCODE_RX, BARCODE_TX);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello dolly!");
  barcode.begin(9600);
  
  while ( !display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDR) ){
      // Loop forever
      Serial.println("Display allocation failed.");
      delay(1000);
  }

  display.clearDisplay();
  
  display.drawPixel(10, 10, WHITE);

  display.display();

  

}

void loop(){
  if (barcode.available()) {
    display.clearDisplay();
    
    display.setTextColor(WHITE);
    display.setCursor(0, 25);
    display.setFont(&FreeMono9pt7b);
    display.setTextSize(1);
    
    while (barcode.available()) {
      
      char input = barcode.read();
      Serial.print(input);

      
      display.print(input);
      display.display();
      
    }
    Serial.println();
    display.clearDisplay();
  }
}
