/* 
 * Project Smart Alarm Clock (Smart Room Controller Midterm Assignment)
 * Author: Rudy Parra
 * Date: Oct-25th-2023
 * Description: This prgram keeps tracks of time, sets alarm and implements a chime to help the user wake up.
 *              It also connects to the Hue Lights and turns them to full brightness when the alarm goes off.
 *              Turns on wemo switches when user presses the snooze button. 
 *              Uses the Encoder to set the time, set alarm, and manually control lights and wemo switches by a series of clicks
 *              
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IOTClassroom_CNM.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include <Encoder.h>
#include "neopixel.h"

//OLED DEFINES
#define OLED_RESET D4
Adafruit_SSD1306 display1(OLED_RESET);
Adafruit_SSD1306 display2(OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char logo16_glcd_bmp[] =
{ 0B00000000, 0B11000000,
  0B00000001, 0B11000000,
  0B00000001, 0B11000000,
  0B00000011, 0B11100000,
  0B11110011, 0B11100000,
  0B11111110, 0B11111000,
  0B01111110, 0B11111111,
  0B00110011, 0B10011111,
  0B00011111, 0B11111100,
  0B00001101, 0B01110000,
  0B00011011, 0B10100000,
  0B00111111, 0B11100000,
  0B00111111, 0B11110000,
  0B01111100, 0B11110000,
  0B01110000, 0B01110000,
  0B00000000, 0B00110000 };

  //OLED SCREEN REALTED
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Setting Bools
bool updown = false; //Initialize the LED state to off

//Decalring Pins for Encoder
const int REDLED = D18;
const int GREENLED = D17;
const int BLUELED = D5;
const int BUTTONPIN = D15;
int PINA = D8;
int PINB = D9;

//Declaring the State of Button
int BUTTONSTATE = 0; //Initialize BUTTONSTATE to off
int onOff;

//Setting Ecoders Starting Position
int position = 0;
int maxPos = 100;

//Setting NeoPixels
int numPixels = 12;
int start = 0;
int end = numPixels -1; // Set the end index to the last pixel

//Setting Variables for Buzzer
int buzzer = D16;
int frequency = 1000;
int duration = 1000;

//Setting HUE Lights Constants
const int BULB =3;
int color;

//Setting Time Inegers
int hour = 1;
int minute = 1;
int alarmHour;
int alarmMinute;

//Setting i as and Integer for multiple use cases
int i;

//Declaring Multiple Objects (neopixels, buttons, encoders, etc.)
Adafruit_NeoPixel pixel(numPixels, SPI1, WS2812B); // Use D6 for NeoPixels (SPI1)
Button button(BUTTONPIN);
String ONOFF;
Encoder myEnc(PINA, PINB);
Servo rightLEG;
Servo leftLEG;
String DateTime , TimeOnly;


// Setting Particles Connection Protocols
SYSTEM_MODE(MANUAL);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

//Setting Encoder CLass for HUE Lights
//class Encoder myEnc(PINA,PINB);



    void setup() {
        // Initializing Monitor Setup
        Serial.begin (9600); //Enable Serial Monitor
        waitFor(Serial.isConnected,10000); //waits for Serial Monitor
        Serial.println("Ready to Go");

        //NeoPixel Setup
        pixel.begin();
        pixel.show();
        pixel.setBrightness(30);

        //Servo Setup
        rightLEG.attach(A2);
        leftLEG.attach(A5);

        //Encoder Setup
        pinMode(BUTTONPIN, INPUT);
        pinMode(GREENLED, OUTPUT);
        pinMode(REDLED, OUTPUT);
        pinMode(BLUELED, OUTPUT);

        //Buzzer Setup
        pinMode(buzzer, OUTPUT);

        //OLED Setup
        display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
        Serial.begin(9600);
        display1.setTextSize(2);
        display1.setTextColor(WHITE);
        display1.setCursor(0, 0);
        display1.setRotation(0);
        display1.display(); // show splash screen
        delay(2000);
        display1.clearDisplay(); // clears the screen and buffer

        display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
        Serial.begin(9600);
        display2.setTextSize(2);
        display2.setTextColor(WHITE);
        display2.setCursor(0, 0);
        display2.setRotation(0);
        display2.display(); // show splash screen
        delay(2000);
        display2.clearDisplay(); // clears the screen and buffer
    
        //WIFI Connection Protocols (Purpose is fo Hue Lights and Wemo Outlet)
        //WiFi Connection Protocols
        WiFi.on();
        WiFi.clearCredentials();
        WiFi.setCredentials("IoTNetwork");
        WiFi.connect();
        while(WiFi.connecting()) {
            Serial.printf(".");
        }
        Serial.printf("\n\n");

        //Setting Various Variables
        pinMode(BUTTONPIN, INPUT);
        onOff = false;
    }

    void loop() { //Test
        for (i=0; i = numPixels; i++){
            pixel.setPixelColor(i++, 255,255,0);
            pixel.setBrightness(30);
            pixel.show();
            pixel.clear();
            delay(80);
        }

        // Servo Loop
        rightLEG.write(0);
        leftLEG.write(0);
        

        //Encoder Loop
        digitalWrite(GREENLED, HIGH); // Set the green LED to ON
        digitalWrite(REDLED, LOW);  // Set the red LED to OFF
        digitalWrite(BLUELED, HIGH);  // Set the red LED to OFF

        //Buzzer Loop
        tone(buzzer, frequency, duration);

        //OLED Loop
            // Displaying name with a Spanish honorific
            display1.printf("1");
            display1.display();
            delay(2000);
            display1.clearDisplay();

            display2.printf("2");
            display2.display();
            delay(2000);
            display2.clearDisplay();
            

        //Hue Lights Loop
        if (button.isClicked()){
            Serial.printf("Setting color of bulb &i to color %06i\n", BULB, HueRainbow[color%7]);
            onOff = !onOff;
            setHue(BULB,onOff, HueBlue, random(32,255),255);
            color++;
        }
    }

