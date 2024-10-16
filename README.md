# Retail Shelf Pricechecker
Arduino sketch files and enclosure 3d models for a retail shelf price checker.\
\
The price checker consists of an ESP 8266 along with a barcode scanner module and an I2C/SPI SSD1309 OLED display.\
\
The code is designed to work with the Woocommerce API but can be adjusted to use any API.
# Usage
Copy `Tests/Woocommerce_API_Request/creds.h.sample` to `src/Price_Checker/creds.h`. Edit the file accordingly. This is the file for credentials used for the ESP; API and WiFi access.\
\
Load the `src/Price_Checker/Price_Checker.ino` sketch into the Arduino IDE . Compile and upload!
# Parts used
- ESP 8266 (LoLin V3 NodeMcu Board)
- EVAWGIB EV-ER08: 1D Barcode scanner. Have to use ribbon cables and adapter PCB.
- SSD1309 2.42" OLED Display
- Mini-360 Buck converter: Convert the shelf led lighting which is 12V  to 5V for the ESP
- Buzzer: Connected to the barcode scanner to eliminate delay in buzz.

Pinout for barcode scanner:\
![EVAWGIB EV-ER08 Pinout](https://raw.githubusercontent.com/CounterCookie/price-checker/refs/heads/main/Design/Barcode%20Scanner%20Pinout.jpg)

# Current implementation
### REST API
The current method uses the Woocommerce REST API to gather product prices using the scanned barcode.\
It's sort of an gimmick to get barcode searching to work with Woocommerce since it needs to be an additional metadata and the response is quite huge and unnecessary.\
And sometimes, the response can be extremely huge and the ESP8266 crashes. Using an external preprocessor such as a Woocommerce plugin might come in handy to make this process a bit more stable.
### Display
The OLED display is using the I2C protocol since it only requires 2 pins. I have not noticed a delay in the display of prices; therefore haven't switched to using SPI. the 8266 does not have enough pins to easily accommodate SPI without pull up/down resistors.\
\
Most ESP 8266 pins cause bootup failures if not used carefully. Use the pins defined in the sketch to be safe. [List of safe pins to use.](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/#table) Avoid any pins that cause failures at boot if possible.\
\
The I2C address of the OLED display might need to be changed. if the preset doesn't work, use an I2C scanner sketch to find the right address.
# Future Improvements
Use SKUs in place of barcodes, which first needs to be done on the Woocommerce shop.
In the future there will be a secondary API to shorten and cache prices periodically in order to make the response short and faster.\
It currently takes about 2 seconds for a price to appear on the screen. Most of the delay is in the HTTP request.
