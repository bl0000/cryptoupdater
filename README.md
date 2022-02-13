# cryptoupdater
Checks prices of cryptocurrencies every minute and displays them on an OLED display.

Inspired by https://github.com/witnessmenow/arduino-sample-api-request

Created for the ESP32 using the Adafruit SSD1306 OLED screen in the Arduino Language.

Please note - I have very little experience with the Arduino Language, hence why the code is horrendous. I may revist the code and tidy it up at a later date. 

Also, there is a bug where after a few loops of the HTTP request, it fails to connect to the API again. I am yet to fix this.

I have created this program to present cryptocurrency data (the price and percentage changed in the last 24 hours) on the SSD1306, whilst also lighting an LED bulb green if it has increased in the last 24 hours (and flash if it has increased more than one percent) and vice-versa with a red LED bulb for decreasing market value.

