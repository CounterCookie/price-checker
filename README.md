# Retail Shelf Pricechecker
Arduino sketch files and enclosure 3d models for a retail shelf price checker.
The price checker consists of an ESP 8266 along with a barcode scanner module and an I2C/SPI SSD1309 OLED display.
The code is designed to work with the Woocommerce API but can be adjusted to use any API.

# Current implementation
The current method uses the Woocommerce REST API to gather product prices using the scanned barcode.
It's sort of an gimmick to get barcode searching to work with Woocommerce since it needs to be an additional metadata and the response is quite huge and unnecessary.
And sometimes, the response can be extremely huge and the ESP8266 crashes.

# Future Improvements
Use SKUs in place of barcodes, which first needs to be done on the woocommerce shop.
In the future there will be a secondary API to shorten and cache prices periodically in order to make the response short and faster.
It currently takes about 2 seconds for a price to appear on the screen.
