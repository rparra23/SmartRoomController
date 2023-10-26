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
#include <Encoder.h>
#include "neopixel.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

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

//Setting Ecoders Starting Position
int position = 0;
int maxPos = 10