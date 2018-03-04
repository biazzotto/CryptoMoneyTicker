/*
  /$$$$$$                                  /$$               /$$      /$$                                      /$$$$$$$$ /$$           /$$
 /$$__  $$                                | $$              | $$$    /$$$                                     |__  $$__/|__/          | $$
| $$  \__/  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$ | $$$$  /$$$$  /$$$$$$  /$$$$$$$   /$$$$$$  /$$   /$$| $$    /$$  /$$$$$$$| $$   /$$  /$$$$$$   /$$$$$$
| $$       /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$| $$ $$/$$ $$ /$$__  $$| $$__  $$ /$$__  $$| $$  | $$| $$   | $$ /$$_____/| $$  /$$/ /$$__  $$ /$$__  $$
| $$      | $$  \__/| $$  | $$| $$  \ $$  | $$    | $$  \ $$| $$  $$$| $$| $$  \ $$| $$  \ $$| $$$$$$$$| $$  | $$| $$   | $$| $$      | $$$$$$/ | $$$$$$$$| $$  \__/
| $$    $$| $$      | $$  | $$| $$  | $$  | $$ /$$| $$  | $$| $$\  $ | $$| $$  | $$| $$  | $$| $$_____/| $$  | $$| $$   | $$| $$      | $$_  $$ | $$_____/| $$
|  $$$$$$/| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$/| $$ \/  | $$|  $$$$$$/| $$  | $$|  $$$$$$$|  $$$$$$$| $$   | $$|  $$$$$$$| $$ \  $$|  $$$$$$$| $$
 \______/ |__/       \____  $$| $$____/    \___/   \______/ |__/     |__/ \______/ |__/  |__/ \_______/ \____  $$|__/   |__/ \_______/|__/  \__/ \_______/|__/
                     /$$  | $$| $$                                                                      /$$  | $$
                    |  $$$$$$/| $$                                                                     |  $$$$$$/
                     \______/ |__/                                                                      \______/

The MIT License (MIT)

Copyright (c) 2018 Médéric NETTO

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <TimeLib.h>

#define TFT_CS   17
#define TFT_DC   16
#define TFT_RST  5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";

const char host[] = "api.coinmarketcap.com";

 // Colors
 int ILI9341_COLOR;
 #define CUSTOM_DARK 0x3186 // Background color

 // Bitmap_WiFi
 //extern uint8_t wifi_1[];
 //extern uint8_t wifi_2[];
 //extern uint8_t wifi_3[];

 // Bitmap_CryptoMonnaies
 extern uint16_t bitcoin1[];
 extern uint16_t ethereum2[];
 extern uint16_t ripple3[];
 extern uint16_t litecoin4[];
 extern uint16_t cardano5[];

 unsigned long previousMillis = 0;
 long interval = 0;

 int coin = 0;
 String crypto[] = {"bitcoin", "ethereum", "ripple", "litecoin", "cardano"};
 String oldPrice[5];

 void setup() {

  //Serial.begin(115200);

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(CUSTOM_DARK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(0, 170);
  tft.setTextSize(2);

  tft.println(">>> Connecting to: ");
  tft.println(" ");
  tft.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(200);
      tft.drawBitmap(70, 50, wifi_1, 100, 100, ILI9341_WHITE);
      delay(200);
      tft.drawBitmap(70, 50, wifi_2, 100, 100, ILI9341_WHITE);
      delay(200);
      tft.drawBitmap(70, 50, wifi_3, 100, 100, ILI9341_WHITE);
      delay(200);
      tft.fillRect(70, 50, 100, 100, CUSTOM_DARK);
  }

  tft.println(" ");
  tft.print(">>> WiFi connected");
  //tft.print("IP address: ");
  //tft.println(WiFi.localIP());

  delay(1500);

  int h = 50, w = 50, row, col, buffidx=0;
  for (row=5; row < h; row++) { // For each scanline...
    for (col=5; col < w; col++) { // For each pixel...
      //To read from Flash Memory, pgm_read_XXX is required.
      //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
      tft.drawPixel(col, row, pgm_read_word(ethereum2 + buffidx));
      buffidx++;
    } // end pixel
  }
  
  tft.fillScreen(CUSTOM_DARK); // Clear Screen

  tft.setTextSize(3);
  tft.setTextColor(ILI9341_BLUE);
  tft.setCursor(5, 110);
  tft.println("Crypto");
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(75, 140);
  tft.println("Money");
  tft.setTextColor(ILI9341_RED);
  tft.setCursor(130, 170);
  tft.println("Ticker");

  tft.drawLine(0, 135, 130, 200, ILI9341_WHITE);
  tft.drawLine(130, 200, 240, 200, ILI9341_WHITE);

  tft.setTextSize(1);
  tft.setCursor(5, 307);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Data from: CoinMarketCap.com");
  */
  delay(5000);
}



void loop() {

tft.setTextColor(ILI9341_WHITE);
tft.setCursor(1, 10);

unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
  interval = 60000;

if(coin == 5) {
  coin = 0;
}

//Serial.print(">>> Connecting to ");
//Serial.println(host);

WiFiClientSecure client;

const int httpsPort = 443;
if (!client.connect(host, httpsPort)) {
  tft.fillScreen(CUSTOM_DARK);
  tft.println(">>> Connection failed");
  return;
}

//Serial.print("Requesting URL: ");
//Serial.println("Connected to server!");

client.println("GET /v1/ticker/" + crypto[coin] + "/ HTTP/1.1");
client.println("Host: api.coinmarketcap.com");
client.println("Connection: close");
client.println();

unsigned long timeout = millis();
while (client.available() == 0) {
  if (millis() - timeout > 5000) {
    tft.fillScreen(CUSTOM_DARK);
    tft.println(">>> Client Timeout!");
    client.stop();
    return;
  }
}

String data;
while(client.available()) {
  data = client.readStringUntil('\r');
  //Serial.println(data);
}

data.replace('[', ' ');
data.replace(']', ' ');

char buffer[data.length() + 1];
data.toCharArray(buffer, sizeof(buffer));
buffer[data.length() + 1] = '\0';

const size_t bufferSize = JSON_OBJECT_SIZE(15) + 110;
DynamicJsonBuffer jsonBuffer(bufferSize);

JsonObject& root = jsonBuffer.parseObject(buffer);

if (!root.success()) {
  tft.println("parseObject() failed");
  return;
}

String name = root["name"]; // "Bitcoin"
String symbol = root["symbol"]; // "BTC"
String price_usd = root["price_usd"]; // "573.137"
String percent_change_1h = root["percent_change_1h"]; // "0.04"
String last_updated = root["last_updated"]; // "1472762067" <-- Unix Time Stamp
String error = root["error"]; // id not found

printTransition();
printLogo();
printName(name, symbol);
printPrice(price_usd);
printChange(percent_change_1h);
printTime(last_updated);
printPagination();
printError(error);

oldPrice[coin] = price_usd;
coin++;
  }
}

void printLogo() {

if (crypto[coin] == "bitcoin") {
    tft.drawBitmap(5, 5, bitcoin, 45, 45, ILI9341_ORANGE);
}else if (crypto[coin] == "ethereum") {
    tft.drawBitmap(5, 5, ethereum, 45, 45, ILI9341_BLACK);
}else if (crypto[coin] == "ripple") {
    tft.drawBitmap(5, 5, ripple, 45, 45, ILI9341_NAVY);
}else if (crypto[coin] == "litecoin") {
    tft.drawBitmap(5, 5, litecoin, 45, 45, ILI9341_LIGHTGREY);
}else if (crypto[coin] == "cardano") {
  tft.drawBitmap(5, 5, cardano, 45, 45, ILI9341_CYAN);
}else{
  tft.drawRect(5, 5, 45, 45, ILI9341_WHITE);
  tft.drawLine(5, 49, 49, 5, ILI9341_WHITE);
  tft.drawLine(5, 5, 49, 49, ILI9341_WHITE);
  }
}

void printName(String name, String symbol) {

int StringLength = name.length();

if (StringLength < 10){
  tft.setTextSize(3);
}else if (StringLength >= 10 && StringLength < 15) {
  tft.setTextSize(2);
}else if (StringLength >= 15 && StringLength < 30) {
  tft.setTextSize(1);
}else{
  tft.setTextSize(2);
  name = "Text too long!";
}

  tft.setCursor(65, 10);
  tft.println(name);

  tft.setTextSize(2);
  tft.setCursor(65, 33);
  tft.print(symbol);

  tft.drawLine(65, 54, 240, 54, ILI9341_WHITE);
}

void printPrice(String price_usd) {

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(5, 75);
  tft.print("Price:");

  if(price_usd != oldPrice[coin]){
    if(price_usd > oldPrice[coin]){

    ILI9341_COLOR = ILI9341_GREENYELLOW;
    }else{

    ILI9341_COLOR = ILI9341_RED;
    }
  }

  tft.setTextSize(3);
  tft.setTextColor(ILI9341_COLOR);
  tft.setCursor(40, 110);
  tft.print("$");
  tft.println(price_usd);
}

void printChange(String percent_change_1h) {

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(5, 150);
  tft.print("Change(1h):");

  if(percent_change_1h >= "0"){

    tft.setTextColor(ILI9341_GREENYELLOW);

    }else{

    tft.setTextColor(ILI9341_RED);
  }

  tft.setTextSize(3);
  tft.setCursor(40, 180);
  tft.print(percent_change_1h);
  tft.print("%");
}

void printTime(String last_updated) {

  long int timeData = last_updated.toInt();
  time_t t = timeData;
  //time_t Z = 1515675789;

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(5, 220);
  tft.print("Last Updated:");

  tft.setTextSize(3);
  tft.setCursor(40, 250);
  /*
  printDigits(day(t));
  tft.print("/");
  printDigits(month(t));
  tft.print("/");
  tft.print(year(t));
  tft.print(" ");
  */
  printDigits(hour(t) + 1); // +1 for the French time
  tft.print(":");
  printDigits(minute(t));
  //tft.print(":");
  //printDigits(second(t));
}

void printPagination() {

  tft.drawCircle( 98, 300, 4, ILI9341_WHITE);
  tft.drawCircle(108, 300, 4, ILI9341_WHITE);
  tft.drawCircle(118, 300, 4, ILI9341_WHITE);
  tft.drawCircle(128, 300, 4, ILI9341_WHITE);
  tft.drawCircle(138, 300, 4, ILI9341_WHITE);

if(coin == 0) {
  tft.fillCircle(98, 300, 4, ILI9341_WHITE);
}else if (coin == 1) {
  tft.fillCircle(108, 300, 4, ILI9341_WHITE);
}else if (coin == 2) {
  tft.fillCircle(118, 300, 4, ILI9341_WHITE);
}else if (coin == 3) {
  tft.fillCircle(128, 300, 4, ILI9341_WHITE);
}else if (coin == 4) {
  tft.fillCircle(138, 300, 4, ILI9341_WHITE);
  }
}

void printError(String error) {

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(65, 22);
  tft.println(error);
}

void printTransition(){

  tft.fillScreen(CUSTOM_DARK);
  delay(10);
  tft.fillScreen(ILI9341_RED);
  delay(10);
  tft.fillScreen(ILI9341_GREEN);
  delay(10);
  tft.fillScreen(ILI9341_BLUE);
  delay(10);
  tft.fillScreen(CUSTOM_DARK);
  delay(10);
}

void printDigits(int digits) {
 // utility function for digital clock display: prints preceding colon and leading 0
 if (digits < 10)
 tft.print('0');
 tft.print(digits);
}
