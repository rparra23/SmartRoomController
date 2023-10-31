/* 
 * Project Smart Alarm Clock (Smart Room Controller Midterm Assignment)
 * Author: Rudy Parra
 * Date: Oct-25th-2023
 * Description: This program keeps track of time, sets an alarm, and implements a chime to help the user wake up.
 *              It also connects to the Hue Lights and turns them to full brightness when the alarm goes off.
 *              Turns on Wemo switches when the user presses the snooze button. 
 *              Uses the Encoder to set the time, set the alarm, and manually control lights and Wemo switches by a series of clicks.
 */

#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include <Encoder.h>
#include "IoTClassroom_CNM.h"
#include <neopixel.h>

#define OLED_RESET    -1
#define ENCODER_A_PIN D8
#define ENCODER_B_PIN D9



Adafruit_SSD1306 display1(OLED_RESET);
Adafruit_SSD1306 display2(OLED_RESET);
Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);
Servo rightLeg;
Servo leftLeg;

//Setting Int for Time
int hour = 1;   // Initialize the hour variable to 1
int minute1 = 1; // Initialize the minute variable to 1


//Setting Constants
    //HUE Bulb
const int BULB = (3);
int TOTALBULB = 6;
int color;
int clickCount = 0;
    // Wemos
  const int MYWEMO1 = 3;
  const int MYWEMO2 = 0;
  bool ONOFF;
  bool wemoOn = false;
  bool buttonPressed = false;
    //Encoder
const int BUTTONPIN = D15;
int BUTTONSTATE;
int lastButtonState  = HIGH;
bool onOff = false;
const int REDLED = D18;
const int GREENLED = D17;
const int BLUELED = D5;
int ENCODER_BUTTON_PIN;
const int buzzer = D16;


    //NeoPixels
int numPixels = 12;
int start = 0;
int end = numPixels -1;
int i;
Adafruit_NeoPixel pixel(numPixels, SPI1, WS2812B);



Button button(BUTTONPIN);

//Setting Connection Protocols to Manual
SYSTEM_MODE(MANUAL);

    void setup() {
        //Serial Monitor
        Serial.begin(9600);
        waitFor(Serial.isConnected,15000);

        //WiFi Connection Protocols
        WiFi.on();
        WiFi.clearCredentials();
        WiFi.setCredentials("IoTNetwork");
        WiFi.connect();
        while(WiFi.connecting()) {
            Serial.printf(".");
        }
        Serial.printf("\n\n");

        //Setting Encoder Inputs and Outputs
        pinMode(BUTTONPIN, INPUT);
        pinMode(GREENLED, OUTPUT);
        pinMode(REDLED, OUTPUT);
        pinMode(BLUELED, OUTPUT);


        //Setting NeoPixel Commands
        pixel.begin();
        pixel.show();
        pixel.setBrightness(30);

        //Servo Variable
        rightLeg.attach(A2);
        leftLeg.attach(A5);

        //Servo Setup
        pinMode(buzzer, OUTPUT);

        //Setting OLED DISPLAY
        display1.begin(SSD1306_SWITCHCAPVCC, 0x03C);
        display2.begin(SSD1306_SWITCHCAPVCC, 0x03D);

        display1.clearDisplay();
        display2.clearDisplay();

        display1.setTextSize(7);
        display1.setTextColor(WHITE);
        //display1.setCursor(0, 0);
        display2.setTextSize(7);
        display2.setTextColor(WHITE);
        //display2.setCursor(30, 0);

        display1.display();
        display2.display();
    }

    void loop() {
        //NEOPixel
        for (i = 0; i<16; i++) {
            pixel.setPixelColor(i, 255, 255, 255);
            pixel.setBrightness(30);
            pixel.show();
            pixel.clear();
            delay(70);
        }

        //ButtonLED
        digitalWrite(GREENLED, LOW); // Set the green LED to ON
        digitalWrite(REDLED, HIGH); 
        digitalWrite(BLUELED, HIGH); 

        // Increment the minute
        minute1++;
        delay(1000);

        // Check if the minute exceeds 59 and reset it to 1
        if (minute1 > 59) {
            minute1 = 1;
            // Increment the hour when the minute resets to 1
            hour++;
            // Check if the hour exceeds 12, and if so, reset it to 1
            if (hour > 12) {
                hour = 1;
            }
             tone(buzzer, 1000);
             delay(800);
             noTone(buzzer);
        }

        // Display the current hour on display1 and minute on display2
            display1.clearDisplay();
            display1.setCursor(30, 0);
            display1.print(hour);
            display1.display();
            

            // Display the current minute on display2
            display2.clearDisplay();
            display2.setCursor(30, 0);
            display2.print(minute1);
            display2.display();

        //HUE LOOP
        if (button.isClicked()) {
            clickCount++;

            if (clickCount == 1) {
                // First click, set color to HueViolet
                Serial.printf("Setting color of bulb %i to HueViolet\n", BULB);
                onOff = !onOff;
                for (i=0; i<=TOTALBULB; i++){
                setHue(i, onOff, HueGreen, random(32, 255), 255);
                }
            } else if (clickCount == 2) {
                // Second click, turn off the Hue light
                Serial.printf("Turning off Hue light for bulb %i\n", BULB);
                onOff = false;
                for (i=0; i<=TOTALBULB; i++){
                setHue(i, onOff, 0, 0, 0); // Assuming 0, 0, 0 means turning off the light
                }
            } else if (clickCount ==3) {
                wemoWrite(MYWEMO1,HIGH);
                wemoWrite(MYWEMO2,HIGH);

            } else if (clickCount ==4) {
                wemoWrite(MYWEMO1,LOW);
                wemoWrite(MYWEMO2,LOW);
                clickCount = 0; // Reset click count to start over
            }
        }
        //Servo Loop
        rightLeg.write(0);
        leftLeg.write(0);
    }

