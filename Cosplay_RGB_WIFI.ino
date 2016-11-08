#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
/*PIN ON NODE MCU*/
    #define D0 16
    #define D1 5  //USE THIS
    #define D2 4
    #define D3 0
    #define D4 2
    #define D5 14
    #define D6 12
    #define D7 13

int NLed=1; //NUMBERS OF LED


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NLed, D1, NEO_GRB + NEO_KHZ800);

//ACCES POINT CONFIG
const char *ssid = "Cosplay_Led_Controller"; //CHANGE THIS
const char *password = "123456789"; //CHANGE THIS

ESP8266WebServer server(80); //Web server Port

char stato='Red';

//HTML PAGE CONFIG...

const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /> </head>";

const String HtmlTitle = "<h1>RGB LED Controller for Cosplay<br/><dir>Made by Borchi Marco</dir></h1><br/>\n";   
const String HtmlRED = "<big><dir>Led is Red<b>Led_RED</dir></b></big><br/>\n";
const String HtmlGREEN = "<big><dir>Led is Green<b>Led_GREEN</dir></b></big><br/>\n";
const String HtmlBLUE ="<big><dir>Led is Blue<b>Led_BLUE</dir></b></big><br/>\n";
const String HtmlRAINBOW = "<big><dir>Led is on Rainbow mode<b>Led_Rainbow</dir></b></big><br/>\n";
const String HtmlRAINBOW2 = "<big><dir>Led is on Rainbow mode 2<b>Led_Rainbow 2</dir></b></big><br/>\n";
const String HtmlButtons = 
    "<a href=\"Led_Rainbow\"><button style=\"display: block; width: 100%;\">Led Rainbow effect (Beta)</button></a><br/>"
    "<a href=\"Led_Rainbow2\"><button style=\"display: block; width: 100%;\">Led Rainbow effect 2 (Beta)</button></a><br/>"
    "<a href=\"Led_Red\"><button style=\"display: block; width: 100%;\">Led Red</button></a><br/>"
    "<a href=\"Led_Green\"><button style=\"display: block; width: 100%;\">Led Green</button></a><br/>"
    "<a href=\"Led_Blue\"><button style=\"display: block; width: 100%;\">Led Blue</button></a><br/>";
const String HtmlHtmlClose = "</html>"; //chiude il tag html della pagina

void handleLedRainbow()
{
    stato='Rainbow';
     rainbow(50);
    Serial.write("LED IS NOW on rainbow mode");
    response();
}

void handleLedRainbow2()
{
    stato='Rainbow2';
     
     rainbowCycle(50);
    Serial.write("LED IS NOW on rainbow mode 2");
    response();
}
void handleLedRed()
{
    stato='Red';
     colorWipe(strip.Color(255, 0, 0), 50); // Red
    Serial.write("LED IS NOW Red");
    response();
}

void handleLedGreen()
{
    stato='Green';
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    Serial.write("LED IS NOW Green");
  response();
}
void handleLedBlue()
{
    stato='Blue';
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    Serial.write("LED IS NOW Blue");
    response();
}

void handleRoot()
{
    response();
}

void response()
{
    String htmlRes = HtmlHtml + HtmlTitle;
    if(stato == 'Red')
    {
        htmlRes +=HtmlRED;
    }
    else if (stato=='Green')
    {
        htmlRes +=HtmlGREEN;
    }
    else if (stato=='Blue')
    {
        htmlRes +=HtmlBLUE;
    }
    else if (stato=='Rainbow')
    {
        htmlRes+=HtmlRAINBOW;
    }
    else if (stato=='Rainbow2')
    {
        htmlRes+=HtmlRAINBOW2;
    }

     htmlRes += HtmlButtons; 
     htmlRes += HtmlHtmlClose;

     server.send(200, "text/html", htmlRes);
}

void setup() 
{
    delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
	Serial.print(" AP IP address: ");
	Serial.println(myIP);

	server.on("/", handleRoot);
    
    server.on("/Led_Red", handleLedRed);
    server.on("/Led_Green", handleLedGreen);
    server.on("/Led_Blue", handleLedBlue);
    server.on("/Led_Rainbow",handleLedRainbow);
    server.on("/Led_Rainbow2", handleLedRainbow2);
    

	server.begin();

	Serial.println("HTTP server started");
    Serial.println("Led Initializing...");
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
    server.handleClient();
}
/*
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW

*/
//SET LED TO ONE COLOR
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
//TO TEST - May causes the server crash
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
  Serial.write("End of rainbow");
}
//TO TEST - May causes the server crash
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  Serial.write("End of rainbowCycle");
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
