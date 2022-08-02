#include "FastLED.h"

#define NUM_LEDS_FAFA 197
#define NUM_LEDS_EDGE 104
#define DATA_PIN_FAFA 2
#define DATA_PIN_EDGE 5
#define F1_START 0
#define A1_START 46
#define F2_START 98
#define A2_START 148

// Define the arrays of leds
CRGB leds_fafa[NUM_LEDS_FAFA];
CRGB leds_edge[NUM_LEDS_EDGE];

static const CRGBPalette16 palette_party = PartyColors_p;
static const CRGBPalette16 palette_lava = LavaColors_p;
static const CRGBPalette16 palette_forest = ForestColors_p;
static const CRGBPalette16 palette_rainbow = RainbowColors_p;

enum Mode {
  LAS_VEGAS = 0,
  FOREST = 1,
  CARNIVAL = 2,
  LETTERS = 3
};

Mode mode = LAS_VEGAS;

// Fill the whole pattern
void Fill(CRGB color, CRGB* leds, int size) {
  for (int i = 0; i < size; i++) {
    leds[i] = color;
  }
}

// Zero the whole pattern
void Blank(CRGB* leds, int size) {
  Fill(CRGB::Black, leds, size);
}

// Fill the letters
void FillF1(CRGB color) {
  for (int i = F1_START; i < A1_START; i++) {
    leds_fafa[i] = color;
  }
}
void FillA1(CRGB color) {
  for (int i = A1_START; i < F2_START; i++) {
    leds_fafa[i] = color;
  }
}
void FillF2(CRGB color) {
  for (int i = F2_START; i < A2_START; i++) {
    leds_fafa[i] = color;
  }
}
void FillA2(CRGB color) {
  for (int i = A2_START; i < NUM_LEDS_FAFA; i++) {
    leds_fafa[i] = color;
  }
}

// Pick a random palette
CRGBPalette16 RandomPalette() {
  CRGBPalette16 palette;
  switch (random(8)) {
    case 0:
      palette = CloudColors_p;
      break;
    case 1:
      palette = LavaColors_p;
      break;
    case 2:
      palette = OceanColors_p;
      break;
    case 3:
      palette = ForestColors_p;
      break;
    case 4:
      palette = RainbowColors_p;
      break;
    case 5:
      palette = RainbowStripeColors_p;
      break;
    case 6:
      palette = PartyColors_p;
      break;
    case 7:
      palette = HeatColors_p;
      break;
  }
  return palette;
}

void setup() {
  // Setup the random number generator using a read on an unconnected pin.
  randomSeed(analogRead(0));
  // Initialize the library
  Blank(leds_fafa, NUM_LEDS_FAFA);
  Blank(leds_edge, NUM_LEDS_EDGE);
  FastLED.addLeds<WS2811, DATA_PIN_FAFA, RGB>(leds_fafa, NUM_LEDS_FAFA);  
  FastLED.addLeds<WS2811, DATA_PIN_EDGE, RGB>(leds_edge, NUM_LEDS_EDGE);  
}

void loop() {
  static int j = 0;
  if (j++ > 10000) {
    mode = (Mode) ((mode + 1) % 4);
    j = 0;
  }  
  
  switch (mode) {
    case LETTERS:
      FillF1(ColorFromPalette(palette_rainbow, (j + 0) % 255, 255, LINEARBLEND));
      FillA1(ColorFromPalette(palette_rainbow, (j + 64) % 255, 255, LINEARBLEND));
      FillF2(ColorFromPalette(palette_rainbow, (j + 128) % 255, 255, LINEARBLEND));
      FillA2(ColorFromPalette(palette_rainbow, (j + 192) % 255, 255, LINEARBLEND));
      for (int i = 0; i < NUM_LEDS_EDGE; i++) {
        leds_edge[i] = 0;
      }
      for (int i = 0; i < NUM_LEDS_EDGE; i++) {      
        if ((i+j) % NUM_LEDS_EDGE < 10) {
          leds_edge[i] = ColorFromPalette(palette_rainbow, j % 255, 255, LINEARBLEND);
          leds_edge[NUM_LEDS_EDGE - i - 1] = ColorFromPalette(palette_rainbow, j % 255, 255, LINEARBLEND);
        }
      }
      break;
    case FOREST:
      for (int i = 0; i < NUM_LEDS_FAFA; i++) {
        leds_fafa[i] = ColorFromPalette(palette_forest, (i+j) % NUM_LEDS_FAFA, NUM_LEDS_FAFA, LINEARBLEND);
      }
      for (int i = 0; i < NUM_LEDS_EDGE; i++) {
        leds_edge[i] = ColorFromPalette(palette_rainbow, j % 255, 255, LINEARBLEND);
      }
      break;
    case CARNIVAL:
      if ( j % 5 == 0) {
        for (int i = 0; i < NUM_LEDS_FAFA; i++) {
          leds_fafa[i] = ColorFromPalette(palette_party, random(255), 255, LINEARBLEND);
        }
        for (int i = 0; i < NUM_LEDS_EDGE; i++) {
          leds_edge[i] = ColorFromPalette(palette_party, random(255), 255, LINEARBLEND);
        }
      }
      break;
    case LAS_VEGAS:    
      for (int i = 0; i < NUM_LEDS_FAFA; i++) {
        leds_fafa[i] = ColorFromPalette(palette_lava, (i+j) % NUM_LEDS_FAFA, NUM_LEDS_FAFA, LINEARBLEND);
      }
      for (int i = 0; i < NUM_LEDS_EDGE; i++) {
        leds_edge[i] = ((i+j/5) % 4 == 0) ? CRGB::Black : CRGB::White;
      }
      break;
  }

  FastLED.show();
}
