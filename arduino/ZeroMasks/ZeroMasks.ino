#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 0
#define BRIGHTNESS 5

#define IS_BEAR true

#define EYE_LEFT_BEGIN 0
#define EYE_RIGHT_BEGIN 16
#define EAR_RIGHT_BEGIN 32

#if IS_BEAR
#define EAR_LEFT_BEGIN 44
#define NUM_LEDS 56
#else
#define EAR_LEFT_BEGIN 40
#define NUM_LEDS 48
#endif


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


boolean useEyes = false;

int partOffset[] = {
  EYE_LEFT_BEGIN,
  EYE_RIGHT_BEGIN,
  EAR_RIGHT_BEGIN,
  EAR_LEFT_BEGIN
};

int partLength[] = {
  EYE_RIGHT_BEGIN - EYE_LEFT_BEGIN,
  EAR_RIGHT_BEGIN - EYE_RIGHT_BEGIN,
  EAR_LEFT_BEGIN - EAR_RIGHT_BEGIN,
  NUM_LEDS - EAR_LEFT_BEGIN
};

int partEnd[4];

void setup() {
  for (int i = 0; i < 4; i++) {
    partEnd[i] = partOffset[i] + partLength[i];
  }

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();

  int entropy = analogRead(0);

  if ((entropy & 1) == 0) {
    useEyes = true;
  } else {
    useEyes = false;
  }

//  Serial.begin(9600);
}

void loop() {
  rainbow(20);
  pulseWhite(); 
  spin();

}

// Fill the dots one after the other with a color
void spin() {
  long veryBeginTime = millis();

  while (millis() - veryBeginTime < 120000) {
    int spinSpeed = random(50, 75);
    int partCurrentIdx[4];
    int partDir[4];
    long partColor;
    float fadePercent = random(2);
  
    for (int i = 0; i < 4; i++) {
      partCurrentIdx[i] = random(partLength[i]);
      partDir[i] = random(2)*2-1;
    }
  
  #if !IS_BEAR
    partCurrentIdx[2] = 0;
    partCurrentIdx[3] = 0;
    partDir[2] = 1;
    partDir[3] = 1;
  #endif
  
    switch(random(7)) {
    case 0:
      partColor = strip.Color(255, 0, 0);
      break;
    case 1:
      partColor = strip.Color(0, 255, 0);
      break;
    case 2:
      partColor = strip.Color(0, 0, 255);
      break;
    case 3:
      partColor = strip.Color(255, 0, 255);
      break;
    case 4:
      partColor = strip.Color(255, 255, 0);
      break;
    case 5:
      partColor = strip.Color(0, 255, 255);
      break;
    case 6:
      partColor = strip.Color(0, 0, 0, 255);
      break;
    }
  
    long timeBegin = millis();
  
    while (millis() - timeBegin < 2000) {
      for (int part = 0; part < 4; part++) {
        if (!useEyes && part < 2) {
          continue;
        }
        
        strip.setPixelColor(partOffset[part] + partCurrentIdx[part], fade(partColor, fadePercent * 0.5));
        partCurrentIdx[part] = (partCurrentIdx[part] + partLength[part] + partDir[part]) % partLength[part];
        strip.setPixelColor(partOffset[part] + partCurrentIdx[part], partColor);
      }
      Serial.println();
      strip.show();
      delay(spinSpeed);
    }
  }
}

void pulseWhite() {
  long veryBeginTime = millis();

  while (millis() - veryBeginTime < 60000) {
    int spinSpeed = random(50, 75);
    int partCurrentIdx[4];
    int partDir[4];
    long partColor;
    float fadePercent = random(2);
  
    for (int i = 0; i < 4; i++) {
      partCurrentIdx[i] = random(partLength[i]);
      partDir[i] = random(2)*2-1;
    }
  
  #if !IS_BEAR
    partCurrentIdx[2] = 0;
    partCurrentIdx[3] = 0;
    partDir[2] = 1;
    partDir[3] = 1;
  #endif


  partColor = strip.Color(0, 0, 0, 128);

    long timeBegin = millis();
  
    while (millis() - timeBegin < 2000) {
      for (int part = 0; part < 4; part++) {
        if (!useEyes && part < 2) {
          continue;
        }
        
        strip.setPixelColor(partOffset[part] + partCurrentIdx[part], fade(partColor, 0.5));
        partCurrentIdx[part] = (partCurrentIdx[part] + partLength[part] + partDir[part]) % partLength[part];
        strip.setPixelColor(partOffset[part] + partCurrentIdx[part], partColor);
      }
      Serial.println();
      strip.show();
      delay(spinSpeed);
    }
  }
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        strip.show();
        delay(wait);
    }
  
  }



  delay(500);


  for(int k = 0 ; k < whiteLoops ; k ++){

    for(int j = 0; j < 256 ; j++){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
          }
          strip.show();
        }

        delay(2000);
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, neopix_gamma[j] ) );
          }
          strip.show();
        }
  }

  delay(500);


}

void rainbow(uint8_t wait) {

  long veryBeginTime = millis();

  while (millis() - veryBeginTime < 120000) {
  
    for(int j=0; j<256; j++) {
      int i = 0;
  
      if (!useEyes) {
        i = EAR_RIGHT_BEGIN;
      }
  
      for(; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }

      strip.show();
      delay(wait);
    }
  }
}
void fullWhite() {
  
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
    }
      strip.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t white(uint32_t c) {
  return (c >> 24);
}
uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

long fade(long c, float fadePercent) {
  return strip.Color(
    red(c) * fadePercent,
    green(c) * fadePercent,
    blue(c) * fadePercent,
    white(c) * fadePercent
  );
}

