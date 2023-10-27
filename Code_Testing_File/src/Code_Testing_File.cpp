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
#include "Button.h"
#include "hue.h"
#include "Colors.h"
#include "wemo.h"
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
  0B00000001, 0B11000000