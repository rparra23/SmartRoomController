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
#include "Button.h"
#include "hue.h"
#include "Colors.h"
#include "wemo.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <Encoder.h>
#include "neopixel.h"

// OLED DEFINES
#define OLED_RESET -1
Adafruit_SSD1306 display1(OLED_RESET);
Adafruit_SSD1306 display2(OLED_RESET);

// Setting Bools
bool updown = false; // Initialize the LED state to off

// Declaring Pins for Encoder
const int REDLED = D18;
const int GREENLED = D17;
const int BLUELED = D5;
const int BUTTONPIN = D15;
int ENCODER_A_PIN = D8;
int ENCODER_B_PIN = D9;

// Declaring the State of Button
int BUTTONSTATE = 0; // Initialize BUTTONSTATE to off
int onOff;

// Setting Encoders Starting Position
int position = 0;
int maxPos = 100;

// Setting Variables for Buzzer
int buzzer = D16;
int frequency = 1000;
int duration = 1000;

// Setting HUE Lights Constants
const int BULB = 3;
int color;

// Setting NeoPixels
int numPixels = 12;
int start = 0;
int end = numPixels - 1; // Set the end index to the last pixel

// Declare the NeoPixel object
Adafruit_NeoPixel pixel(numPixels, SPI1, WS2812B);

// Setting Time Integers
int currentHour = 12;
int currentMinute = 0;
int alarmHour;
int alarmMinute;
int currentTime;

// Variable to keep track of button presses
int buttonPresses = 0;
int clickCount = 0; // Counter for button clicks
bool adjustingHour = true; // Flag to indicate whether we are adjusting the hour or minute
bool flashEnabled = false;

// Declare the Encoder object
Encoder myEnc(ENCODER_A_PIN, ENCODER_B_PIN);

//Setting Fucntions
    void flashOLED();
    void flashNeoPixel();

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    Serial.begin(9600);

    // Initialize NeoPixel
    pixel.begin();
    pixel.show();
    pixel.setBrightness(30);

    // Initialize OLED
    display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    display1.display();
    display2.display();
    display1.clearDisplay();
    display2.clearDisplay();
    display1.display();
    display2.display();

    // Initialize button pin
    pinMode(BUTTONPIN, INPUT_PULLUP);
}

void loop() {
    // Check if the encoder button is pressed
    int buttonState = digitalRead(BUTTONPIN);
    if (buttonState == LOW) {
        delay(50); // Debounce

        // Count button presses
        buttonPresses++;

        if (buttonPresses == 1) {
            // Single click: Enable flashing
            flashEnabled = true;
        } else if (buttonPresses == 2) {
            // Double click: Exit time adjustment mode
            flashEnabled = false;
        }

        delay(250); // Delay to prevent multiple increments
        buttonPresses = 0;
    }

    // Handle time adjustment when not in flashing mode
    if (!flashEnabled) {
        int encoderValue = myEnc.read();
        if (encoderValue > 0) {
            // Adjust hour
            currentHour = (currentHour + 1) % 24;
            myEnc.write(0); // Reset the encoder value
        } else if (encoderValue < 0) {
            // Adjust minute
            currentMinute = (currentMinute + 1) % 60;
            myEnc.write(0); // Reset the encoder value
        }
    }

    // Handle flashing NeoPixel and OLED screens
    if (flashEnabled) {
        flashNeoPixel();
        flashOLED();
    } else {
        // Format time as strings
        String hourStr = String(currentHour);
        String minuteStr = (currentMinute < 10 ? "0" : "") + String(currentMinute);

        // Clear the OLED screens
        display1.clearDisplay();
        display2.clearDisplay();

        // Setting the text size and color
        display1.setTextSize(2);
        display1.setTextColor(WHITE);
        display2.setTextSize(2);
        display2.setTextColor(WHITE);

        display1.setCursor(80, 0);
        display1.print(hourStr);
        display1.display();

        display2.setCursor(50, 0);
        display2.print(minuteStr);
        display2.display();
    }
    
}

void flashNeoPixel() {
    // NeoPixel flashing logic here
    for (int i = 0; i < numPixels; i++) {
        pixel.setPixelColor(i, 255, 0, 255);
        pixel.setBrightness(30);
        pixel.show();
        pixel.clear();
        delay(80);
    }
}

void flashOLED() {
    static bool oledOn = false;
    if (millis() % 1000 < 500) { // Flash the OLED screens every 1 second with 500ms on and 500ms off
        if (!oledOn) {
            display1.clearDisplay();
            display2.clearDisplay();
            display1.display();
            display2.display();
            oledOn = true;
        }
    } else {
        if (oledOn) {
            display1.clearDisplay();
            display2.clearDisplay();
            display1.display();
            display2.display();
            oledOn = false;
        }
    }
}
