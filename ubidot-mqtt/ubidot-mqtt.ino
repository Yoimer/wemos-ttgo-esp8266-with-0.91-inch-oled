/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include <U8g2lib.h>      // make sure to add U8g2 library and restart Arduino IDE  
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "A1E-i5m3qmzERF7kb414GGBkmLjRVtfrXH" // Your Ubidots TOKEN
#define WIFINAME "Casa" //Your SSID
#define WIFIPASS "remioy2006202" // Your Wifi Pass
#define OLED_SDA  2
#define OLED_SCL 14
#define OLED_RST  4
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA , OLED_RST);
const char *text = "Alexa is Alive";  // scroll this text from right to left
String show_on_display = "";
Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  show_on_display = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    show_on_display += (char)payload[i];
  }
  Serial.println();
  updateScreen();
  
}

void updateScreen() {
        // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://things.ubidots.com/api/v1.6/devices/oled/web_value/values?token=A1E-i5m3qmzERF7kb414GGBkmLjRVtfrXH"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    } else {
        USE_SERIAL.println("There is a network problem");
    }
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  u8g2.begin();
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe("oled","web_value"); //Insert the dataSource and Variable's Labels

  // for GET requests
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFINAME, WIFIPASS);

  }

void loop() {
  // put your main code here, to run repeatedly:
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_8x13B_mf); // choose a suitable font
  u8g2.drawStr(0,10,text);  // write something to the internal memory
  u8g2.drawStr(0,28,show_on_display.c_str());  // write something to the internal memory
  u8g2.sendBuffer();

  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe("oled","web_value"); //Insert the dataSource and Variable's Labels
      }
  client.loop();
  }
