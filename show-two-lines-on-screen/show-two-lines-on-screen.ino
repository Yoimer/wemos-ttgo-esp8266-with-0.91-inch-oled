//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                                          ////
////  TTGO ESP8266 0.91 inch OLED demo w/                                                                 ////
////  requires U8g2 library                                                                                   ////
////  Select "NodeMCU 1.0 (ESP-12 Module)" board                                                              ////
////  More examples at http://simplestuffmatters.com                                                          ////
////  USB drivers: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers  ////
////                                                                                                          ////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <U8g2lib.h>      // make sure to add U8g2 library and restart Arduino IDE  
#include <SPI.h>
#include <Wire.h>

#define OLED_SDA  2
#define OLED_SCL 14
#define OLED_RST  4
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA , OLED_RST);
const char *text = "Alexa is Alive";  // scroll this text from right to left

void setup() {
  // Serial.begin(115200);
  u8g2.begin();
}
 
void loop() {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_8x13B_mf); // choose a suitable font
  u8g2.drawStr(0,10,text);  // write something to the internal memory
  String sFullip = "PRUEBA";
  u8g2.drawStr(0,28,sFullip.c_str());  // write something to the internal memory
    
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
}