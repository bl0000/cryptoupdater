// ----------------------------
// Standard Libraries
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define GLED 23
#define RLED 19

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//------- Replace the following! ------
char ssid[] = "TP-Link_E06C";       // your network SSID (name)
char password[] = "?0DoWd@BCfC7!";  // your network key


// HTTPS 
WiFiClientSecure client;


// base URL 
#define TEST_HOST "api.coingecko.com"


// Root cert of server we are connecting to
// Baltimore CyberTrust Root - Expires 12 May 2025
const char *server_cert = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
                                  "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
                                  "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
                                  "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
                                  "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
                                  "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
                                  "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
                                  "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
                                  "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
                                  "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
                                  "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
                                  "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
                                  "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
                                  "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
                                  "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
                                  "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
                                  "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
                                  "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
                                  "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
                                  "-----END CERTIFICATE-----\n";


void setup() {

  Serial.begin(115200);

  // Connect to the WiFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(GLED, OUTPUT); 
  pinMode(RLED, OUTPUT);
  //--------

  // Checking the cert is the best way on an ESP32
  // This will verify the server is trusted.
  client.setCACert(server_cert);

  static const char* test = "test123";
  Serial.println(test);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed."));
    for(;;);
  }
  display.display();
  delay(2000);

}

void makeHTTPRequest() {

  // Opening connection to server (Use 80 as port if HTTP)
  if (!client.connect(TEST_HOST, 443))
  {
    Serial.println(F("Connection failed"));
    return;
  }

  // give the esp a breather
  yield();

  // Send HTTP request
  client.print(F("GET "));
  // This is the second half of a request (everything that comes after the base URL)
  client.print("/api/v3/simple/price?ids=bitcoin%2Cethereum%2Ccardano%2Ctezos%2Clitecoin&vs_currencies=gbp&include_market_cap=false&include_24hr_vol=false&include_24hr_change=true&include_last_updated_at=false"); // %2C == ,
  client.println(F(" HTTP/1.1"));

  //Headers
  client.print(F("Host: "));
  client.println(TEST_HOST);

  client.println(F("Cache-Control: no-cache"));

  if (client.println() == 0)
  {
    Serial.println(F("Failed to send request"));
    return;
  }
  
  delay(100);
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
  {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    return;
  }

  // This is probably not needed for most, but I had issues
  // with the Tindie api where sometimes there were random
  // characters coming back before the body of the response.
  // This will cause no hard to leave it in
  // peek() will look at the character, but not take it off the queue
  while (client.available() && client.peek() != '{')
  {
    char c = 0;
    client.readBytes(&c, 1);
    Serial.print(c);
    Serial.println("BAD");
  }

  DynamicJsonDocument doc(384);

  DeserializationError error = deserializeJson(doc, client);
    
  if (!error) {
    float cardano_gbp = doc["cardano"]["gbp"]; // 1.77
    double cardano_gbp_24h_change = doc["cardano"]["gbp_24h_change"]; // -13.720802371967345
    
    long bitcoin_gbp = doc["bitcoin"]["gbp"]; // 34081
    double bitcoin_gbp_24h_change = doc["bitcoin"]["gbp_24h_change"]; // -10.232719575972183
    
    float ethereum_gbp = doc["ethereum"]["gbp"]; // 2496.2
    double ethereum_gbp_24h_change = doc["ethereum"]["gbp_24h_change"]; // -12.42264314784384
    
    float litecoin_gbp = doc["litecoin"]["gbp"]; // 128.87
    double litecoin_gbp_24h_change = doc["litecoin"]["gbp_24h_change"]; // -19.92999017540627
    
    float tezos_gbp = doc["tezos"]["gbp"]; // 3.21
    double tezos_gbp_24h_change = doc["tezos"]["gbp_24h_change"]; // -21.080447561274884

    client.println("Connection: close");
    client.println(); // end HTTP header
    
    Serial.print("bitcoin_gbp: ");
    Serial.println(bitcoin_gbp);
    Serial.print("bitcoin change: ");
    Serial.println(bitcoin_gbp_24h_change);

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Bitcoin"));
    display.setCursor(10, 10);
    display.println(bitcoin_gbp);
    display.setCursor(45, 10);
    display.println("GBP");
    display.setCursor(10, 20);
    display.println(bitcoin_gbp_24h_change,1);
    if(bitcoin_gbp_24h_change < 0 || bitcoin_gbp_24h_change >= 10) { // due to screen limitations
      display.setCursor(36, 20);
      display.println(F("% change in 24h"));
    } else if(bitcoin_gbp_24h_change <= -10) {
      display.setCursor(30,20);
      display.println(F("% change in 24")); }
    else {
      display.setCursor(32,20);
      display.println(F("% change in 24h")); }
    display.display();      // Show initial text
    if (bitcoin_gbp_24h_change > 1) { // LED configuration
      for (int i = 0; i < 30; i++) {
        digitalWrite(GLED, HIGH);
        
        delay(500);
        digitalWrite(GLED, LOW);
        
        delay(500); }
    } else if (bitcoin_gbp_24h_change > 0) {
      digitalWrite(GLED, HIGH);
      
      delay(30000);
      digitalWrite(GLED, LOW);
      
    } else if (bitcoin_gbp_24h_change < 0 && bitcoin_gbp_24h_change > -1) {
      digitalWrite(RLED, HIGH);
      
      delay(30000);
      digitalWrite(RLED, LOW);
      
    } else if (bitcoin_gbp_24h_change <= -1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(RLED, HIGH);
        
        delay(500);
        digitalWrite(RLED, LOW);
        
        delay(500); }
    }

    display.clearDisplay();
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Ethereum"));
    display.setCursor(10, 10);
    display.println(ethereum_gbp);
    display.setCursor(55, 10);
    display.println("GBP");
    display.setCursor(10, 20);
    display.println(ethereum_gbp_24h_change,1);
    if(ethereum_gbp_24h_change < 0 || ethereum_gbp_24h_change >= 10) {
      display.setCursor(36, 20);
      display.println(F("% change in 24h"));
    } else if(ethereum_gbp_24h_change <= -10) {
      display.setCursor(30,20);
      display.println(F("% change in 24")); }
    else {
      display.setCursor(32,20);
      display.println(F("% change in 24h")); }
    display.display(); // Show initial text
    if (ethereum_gbp_24h_change > 1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(GLED, HIGH);
        delay(500);
        digitalWrite(GLED, LOW);
        delay(500); }
    } else if (ethereum_gbp_24h_change > 0) {
      digitalWrite(GLED, HIGH);
      delay(30000);
      digitalWrite(GLED, LOW);
    } else if (ethereum_gbp_24h_change < 0 && ethereum_gbp_24h_change > -1) {
      digitalWrite(RLED, HIGH);
      delay(30000);
      digitalWrite(RLED, LOW);
    } else if (ethereum_gbp_24h_change <= -1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(RLED, HIGH);
        delay(500);
        digitalWrite(RLED, LOW);
        delay(500); } }
        
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Litecoin"));
    display.setCursor(10, 10);
    display.println(litecoin_gbp);
    display.setCursor(50, 10);
    display.println("GBP");
    display.setCursor(10, 20);
    display.println(litecoin_gbp_24h_change,1);
    if(litecoin_gbp_24h_change < 0 || litecoin_gbp_24h_change >= 10) {
      display.setCursor(36, 20);
      display.println(F("% change in 24h")); }
      else if(litecoin_gbp_24h_change <= -10) {
      display.setCursor(30,20);
      display.println(F("% change in 24")); }
      else { 
    display.setCursor(32,20); 
    display.println(F("% change in 24h")); }
    display.display();      // Show initial text
    
    if (litecoin_gbp_24h_change > 1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(GLED, HIGH);    
        delay(500);
        digitalWrite(GLED, LOW);
        delay(500); }
    } else if (litecoin_gbp_24h_change > 0) {
      digitalWrite(GLED, HIGH);
      delay(30000);
      digitalWrite(GLED, LOW);
    } else if (litecoin_gbp_24h_change < 0 && litecoin_gbp_24h_change > -1) {
      digitalWrite(RLED, HIGH);
      delay(30000);
      digitalWrite(RLED, LOW);
    } else if (litecoin_gbp_24h_change <= -1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(RLED, HIGH);
        delay(500);
        digitalWrite(RLED, LOW);
        delay(500); }
    }

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Cardano"));
    display.setCursor(10, 10);
    display.println(cardano_gbp);
    display.setCursor(40, 10);
    display.println("GBP");
    display.setCursor(10, 20);
    display.println(cardano_gbp_24h_change,1);
    if(cardano_gbp_24h_change < 0 || cardano_gbp_24h_change >= 10) {
      display.setCursor(36, 20);
      display.println(F("% change in 24h"));
    } else if(cardano_gbp_24h_change <= -10) {
      display.setCursor(30,20);
      display.println(F("% change in 24")); }
      else { 
    display.setCursor(32,20); 
    display.println(F("% change in 24h")); }
    display.display();      // Show initial text
    
    if (cardano_gbp_24h_change > 1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(GLED, HIGH);
        delay(500);
        digitalWrite(GLED, LOW);
        delay(500); }
    } else if (cardano_gbp_24h_change > 0) {
      digitalWrite(GLED, HIGH);
      delay(30000);
      digitalWrite(GLED, LOW);
    } else if (cardano_gbp_24h_change < 0 && cardano_gbp_24h_change > -1) {
      digitalWrite(RLED, HIGH);
      delay(30000);
      digitalWrite(RLED, LOW);
    } else if (cardano_gbp_24h_change <= -1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(RLED, HIGH);
        delay(500);
        digitalWrite(RLED, LOW);
        delay(500); }
    }

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Tezos"));
    display.setCursor(10, 10);
    display.println(tezos_gbp);
    display.setCursor(40, 10);
    display.println("GBP");
    display.setCursor(10, 20);
    display.println(tezos_gbp_24h_change,1);
    if(tezos_gbp_24h_change < 0 || tezos_gbp_24h_change >= 10) {
      display.setCursor(36, 20);
      display.println(F("% change in 24h"));
    } else if(tezos_gbp_24h_change <= -10) {
      display.setCursor(30,20);
      display.println(F("% change in 24")); }
      else { 
    display.setCursor(32,20);
    display.println(F("% change in 24h")); }
    display.display();      // Show initial text
    if (tezos_gbp_24h_change > 1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(GLED, HIGH);
        delay(500);
        digitalWrite(GLED, LOW);
        delay(500); }
    } else if (tezos_gbp_24h_change > 0) {
      digitalWrite(GLED, HIGH);
      delay(30000);
      digitalWrite(GLED, LOW);
    } else if (tezos_gbp_24h_change < 0 && tezos_gbp_24h_change > -1) {
      digitalWrite(RLED, HIGH);
      delay(30000);
      digitalWrite(RLED, LOW);
    } else if (tezos_gbp_24h_change <= -1) {
      for (int i = 0; i < 30; i++) {
        digitalWrite(RLED, HIGH);
        delay(500);
        digitalWrite(RLED, LOW);
        delay(500); }
    }
    
  } else {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
}
  
void loop() {
  Serial.println(F("Looping"));
  delay(5000);
  makeHTTPRequest();
}
