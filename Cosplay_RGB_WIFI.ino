#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
/*PIN ON NODE MCU*/
    #define D0 16
    #define D1 5  //ATTUALMENTE USO QUESTA
    #define D2 4
    #define D3 0
    #define D4 2
    #define D5 14
    #define D6 12
    #define D7 13

int NLed=1; //NUMERO DEI LED PRESENTI

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NLed, D1, NEO_GRB + NEO_KHZ800);
//ACCES POINT CONFIG
const char *ssid = "Cosplay_Led_Controller";
const char *password = "123456789"; //DA CAMBIARE

ESP8266WebServer server(80); //PORTA DEL WEBSERVER

char stato='Red';

//HTML PAGE CONFIG...

const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /> </head>";

const String HtmlTitle = "<h1>RGB LED Controller<br/><dir>For manage the color of led. Made by Borchi Marco and Elisa Barzaghi</dir></h1><br/>\n";   
const String HtmlRED = "<big><dir>Led is Red<b>Led_RED</dir></b></big><br/>\n";
const String HtmlGREEN = "<big><dir>Led is Green<b>Led_GREEN</dir></b></big><br/>\n";
const String HtmlBLUE ="<big><dir>Led is Blue<b>Led_BLUE</dir></b></big><br/>\n";
const String HtmlButtons = 
    "<a href=\"Led_Red\"><button style=\"display: block; width: 100%;\">Led Red</button></a><br/>"
    "<a href=\"Led_Green\"><button style=\"display: block; width: 100%;\">Led Green</button></a><br/>"
    "<a href=\"Led_Blue\"><button style=\"display: block; width: 100%;\">Led Blue</button></a><br/>";
const String HtmlHtmlClose = "</html>"; //chiude il tag html della pagina


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
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
