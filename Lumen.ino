//These are needed to make Node MCU work
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

//Libraries
#include <FastLED.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//These are the number of LEDs in each panel
#define NUM_LEDS 16

//These all set config data used in the "setupWifi" function
#define STASSID "Lumen"
const char* ssid = STASSID;
IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

//These are declarations for LED control - don't worry about them too much
SimpleTimer timer;
CRGB leds[NUM_LEDS];

//Global variables
const int ledPin = 12; //marked as D6 on the ESP dev board
int totalMinutes = 0;
int ledsRemaining = 0;
int mappedValue = 0;
int mappedTimer = 10000;
int timerNumber = 1;
bool boot = true;

//Function for starting the soft access point connection
void setupWifi(){
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid) ? "Ready" : "Failed!");

  //This just lets us know what IP address the chip has, the softAPIP method returns this value
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

//Function that just removes one LED from the LED chain
void subtractInterval()
{
  ledsRemaining--;
}

//I have no idea what this does, I think it controls animations?
void ledTimer()
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int colorLED = map(ledsRemaining, 0, NUM_LEDS, 0, 96);
  for(int i = 0; i < ledsRemaining; i++) 
  {
    leds[i] = CHSV (colorLED,255,192);
  }
}

//Setup function
void setup() {
  Serial.begin(9600);
  setupWifi();
  pinMode(14, HIGH); //5V power to LEDs, labeled as D5 on board
  FastLED.addLeds<WS2812B, ledPin, RGB>(leds, NUM_LEDS);
}

void loop() {
  ledTimer();
  timer.reset();
  FastLED.show();
}
