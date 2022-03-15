/*
 *  This sketch displays the numeral 12 with Seahawks colors fading.
 * 
 *  Hardware:  Arduino Uno, Neopixel 8x8 matrix with ws2812b
 *  Neopixel signal connected to pin 13 on Arduino
 * 
 */

#include <Adafruit_NeoPixel.h>

#define LED_PIN    13
#define LED_COUNT 64
static const uint32_t SeaHawksBlue =  ((uint32_t)0 << 16) | ((uint32_t)34 <<  8) | 68;
static const uint32_t SeaHawksGreen = ((uint32_t)105 << 16) | ((uint32_t)190 <<  8) | 40;
static const uint32_t SeaHawksGrey = ((uint32_t)165 << 16) | ((uint32_t)172 <<  8) | 175;
int backgroundBrightness = 25;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

boolean const static display12[64] = 
  {0, 0, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 1, 1, 1, 1,
  1, 0, 0, 1, 0, 0, 0, 1,
  1, 0, 0, 1, 0, 0, 0, 1,
  1, 0, 0, 0, 1, 1, 1, 1};

void setup() {
  // put your setup code here, to run once:
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  //Serial.begin(9600);
  show12(SeaHawksGrey, SeaHawksGreen, backgroundBrightness);
  delay(2000);
}

void loop() {
  fade(SeaHawksGreen, SeaHawksBlue, backgroundBrightness, 50);
  show12(SeaHawksGrey, SeaHawksBlue, backgroundBrightness);
  delay(2000);
  fade(SeaHawksBlue, SeaHawksGreen, backgroundBrightness, 50);
  show12(SeaHawksGrey, SeaHawksGreen, backgroundBrightness);
  delay(2000);
}


void show12(uint32_t color1, uint32_t color2, int brightness) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    if(display12[i]) {
        strip.setPixelColor(i, color1, 255);         //  Set pixel's color (in RAM)
    }
    else {
      strip.setPixelColor(i, color2, brightness);
    }
    strip.show();
  }
}

//Algorithm code credit: http://members.chello.at/easyfilter/bresenham.html

void fade(uint32_t color0, uint32_t color1, int brightness, int wait)
{
   //unpack colors
    int r0 = (uint8_t)(color0 >> 16);
    int g0 = (uint8_t)(color0 >>  8);
    int b0 = (uint8_t)color0;
    int r1 = (uint8_t)(color1 >> 16);
    int g1 = (uint8_t)(color1 >>  8);
    int b1 = (uint8_t)color1;
    
    int dr = abs(r1-r0), sr = r0<r1 ? 1 : -1;
    int dg = abs(g1-g0), sg = g0<g1 ? 1 : -1; 
    int db = abs(b1-b0), sb = b0<b1 ? 1 : -1; 
    int dm = max(max(dr,dg),db), i = dm; /* maximum difference */
    r1 = g1 = b1 = dm/2; /* error offset */

    int r0_scaled = -1, g0_scaled = -1, b0_scaled = -1;
    int r0_scaled_old, g0_scaled_old, b0_scaled_old;

    boolean changed = true;
    
    for(;;) {  /* loop */    
      r0_scaled_old = r0_scaled;
      b0_scaled_old = b0_scaled;
      g0_scaled_old = g0_scaled;
        
      r0_scaled = (r0 * brightness) >> 8;
      g0_scaled = (g0 * brightness) >> 8;
      b0_scaled = (b0 * brightness) >> 8;
      
      if(r0_scaled_old == r0_scaled && b0_scaled_old == b0_scaled && g0_scaled_old == g0_scaled) { changed = false; }
      else { changed = true; }
      
      /*Serial.print("Raw: "); Serial.print(r0); Serial.print("  "); Serial.print(g0); Serial.print("  "); Serial.print(b0);
      Serial.print("  Scaled   "); Serial.print(r0_scaled); Serial.print("  "); Serial.print(g0_scaled); Serial.print("  "); Serial.print(b0_scaled); 
      Serial.print("  Changed?  "); Serial.println(changed);*/
      
      if(changed) {
        uint32_t transitionColor = ((uint32_t)r0_scaled << 16) | ((uint32_t)g0_scaled <<  8) | b0_scaled;
   
        for(int j=0; j<strip.numPixels(); j++) { // For each pixel in strip...
          if(!display12[j]) {
          
          strip.setPixelColor(j, transitionColor);         //  Set pixel's color (in RAM)
          }
          strip.show();
        }
        changed = false;
        delay(wait);
      }
      
      if (i-- == 0) break;
      r1 -= dr; if (r1 < 0) { r1 += dm; r0 += sr;} 
      g1 -= dg; if (g1 < 0) { g1 += dm; g0 += sg;} 
      b1 -= db; if (b1 < 0) { b1 += dm; b0 += sb;} 
   }
}
