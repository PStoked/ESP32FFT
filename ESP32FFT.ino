// These import statements allow us to do more things more quickly and efficiently with our ESP32
#include <Arduino.h>
#include <ESP_I2S.h>
#include <FastLED.h>
#include <arduinoFFT.h>

// These are the initial settings for our LED strip, initializing
// the pin that controls the LED, the number of LEDs, and how the LEDs 
// be controlled. 

// You can change NUM_LEDS so that microcontroller knows 
// how to distribute the frequencies along the LED strip
#define LED_PIN     32
#define NUM_LEDS    150
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

//Microphone Settings
#define I2S_BCLK    26
#define I2S_LRCLK   25
#define I2S_DIN     33
I2SClass I2S;

// This first setting, FFT_GAIN, is what you're most likely to want to change
// Increase FFT_GAIN to make it more sensitive to noise; decrease if saturates too easily.
// The ideal value for FFT_GAIN will be different depending on your system
// as well as the environment in which you want to use it
static constexpr float FFT_GAIN = 1e-13f;
static constexpr float BRIGHTNESS_GAIN = 1.0f;

// These are the settings for the Fast Fourier Transform (FFT)
// This controls how the FFT algorithm processes the data it recieves.

// Number of samples per second
static constexpr uint32_t SAMPLE_RATE = 16000;
// Number of samples processed for one "frame" of the FFT
static constexpr uint16_t FFT_SIZE   = 1024;

//These constants determine the minimum and maximum values
//displayed on the LED strip.
// You can change these values to your preference!!
static constexpr float FREQ_MIN = 100.0f;
static constexpr float FREQ_MAX = 8000.0f;

// More FFT settings
double vReal[FFT_SIZE];
double vImag[FFT_SIZE];
ArduinoFFT<double> FFT(vReal, vImag, FFT_SIZE, SAMPLE_RATE);

// Settings for the LED display
// How quickly do the LEDs smoothly get dimmer when a frequency is no longer present.
static constexpr float SMOOTHING = 0.30f;
float smoothed[NUM_LEDS];

// This part determines what actually gets sent to the LED strip by the FFT
static constexpr float GATE_POWER    = 10000.0f; // raise for more "blank in silence"
static constexpr float SIGNAL_MARGIN = 1.5f;     // frame must exceed gate*margin

// Increase FFT_GAIN to make it more sensitive to noise; decrease if saturates too easily.
// The ideal value for FFT_GAIN will be different depending on your system
// as well as the environment in which you want to use it
static constexpr float FFT_GAIN = 1e-13f;
static constexpr float BRIGHTNESS_GAIN = 1.0f;

// I added a high pass filter so that slow, rumbling frequencies
// (like an AC unit) can be filtered out before the FFT is calculated
// Set cutoff frequency here (e.g. 60, 100, 150, 200...)
static constexpr float HPF_CUTOFF_HZ = 60.0f;

// 1st-order HPF state
static float hpf_x1 = 0.0f; // previous input
static float hpf_y1 = 0.0f; // previous output

// Compute HPF coefficient from cutoff
static inline float hpfAlpha(float cutoffHz) {
  // 1st order digital HPF (RC):
  // alpha = RC / (RC + dt), RC = 1/(2*pi*fc), dt = 1/fs
  const float dt = 1.0f / (float)SAMPLE_RATE;
  const float RC = 1.0f / (2.0f * (float)M_PI * cutoffHz);
  return RC / (RC + dt);
}

static inline float highPassSample(float x, float alpha) {
  // y[n] = alpha * ( y[n-1] + x[n] - x[n-1] )
  float y = alpha * (hpf_y1 + x - hpf_x1);
  hpf_x1 = x;
  hpf_y1 = y;
  return y;
}

// There are many ways of scaling the LED display, but I personally think the most interesting
// way to do it is with evenly spaced octaves. That way, if you mark out
// where the octaves lie like a piano, you can see where the overtones are
// in terms of the musical notes we hear, instead of with a linear frequency scale.
uint16_t binStart[NUM_LEDS];
uint16_t binEnd[NUM_LEDS];

static void buildLogBins() {
  const double binWidth = (double)SAMPLE_RATE / (double)FFT_SIZE;
  const double logFMin = log((double)FREQ_MIN);
  const double logFMax = log((double)FREQ_MAX);

  for (int led = 0; led < NUM_LEDS; led++) {
    double t1 = (double)led / (double)NUM_LEDS;
    double t2 = (double)(led + 1) / (double)NUM_LEDS;

    double f1 = exp(logFMin + (logFMax - logFMin) * t1);
    double f2 = exp(logFMin + (logFMax - logFMin) * t2);

    int b1 = (int)floor(f1 / binWidth);
    int b2 = (int)ceil (f2 / binWidth);

    if (b1 < 1) b1 = 1;
    int maxBin = (FFT_SIZE / 2) - 1;
    if (b2 > maxBin) b2 = maxBin;
    if (b2 < b1) b2 = b1;

    binStart[led] = (uint16_t)b1;
    binEnd[led]   = (uint16_t)b2;
  }
}

//This sets the color mapping of the device
//The default is blue moving from blue to purple to red with an increase in amplitude
static inline CRGB amplitudeToColor(uint8_t v) {
  uint8_t hue = (uint8_t)(160 - ((uint16_t)v * 160) / 255); // 160..0
  return CHSV(hue, 255, v);
}


// Setup protocols
 
void setup() {
  Serial.begin(115200);
  delay(200);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.clear(true);

  I2S.setPins(I2S_BCLK, I2S_LRCLK, -1, I2S_DIN);
  if (!I2S.begin(I2S_MODE_STD, SAMPLE_RATE,
                 I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_MONO)) {
    while (1) delay(1000);
  }

  buildLogBins();

  for (int i = 0; i < NUM_LEDS; i++) smoothed[i] = 0.0f;
}

// The main loop, which will run continuously until it's cut off

void loop() {

  const float alpha = hpfAlpha(HPF_CUTOFF_HZ);

  // capture
  // We still remove DC by mean subtraction, but HPF helps kill rumble/handling noise.
  double mean = 0.0;

  for (int i = 0; i < FFT_SIZE; i++) {
    int32_t s;
    while (I2S.available() < 4) {}
    I2S.readBytes((char*)&s, 4);
    s >>= 8; // ~24 useful bits

    // High-pass in time domain (float), then store into double FFT buffer
    float xf = (float)s;
    float yhpf = highPassSample(xf, alpha);

    vReal[i] = (double)yhpf;
    vImag[i] = 0.0;

    mean += vReal[i];
  }

  // DC removal
  mean *= (1.0 / (double)FFT_SIZE);
  for (int i = 0; i < FFT_SIZE; i++) vReal[i] -= mean;

  // FFT
  FFT.windowing(FFT_WIN_TYP_HANN, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  float ledP[NUM_LEDS];
  float maxP = 1e-9f;

  for (int led = 0; led < NUM_LEDS; led++) {
    uint16_t b1 = binStart[led];
    uint16_t b2 = binEnd[led];

    double sum = 0.0;
    int n = (int)(b2 - b1 + 1);

    for (uint16_t b = b1; b <= b2; b++) {
      double a = vReal[b];
      sum += a * a;
    }

    float p = (float)(sum / (double)n);
    ledP[led] = p;
    if (p > maxP) maxP = p;
  }

  // ---- noise gate decision (frame-level) ----
  const bool signalPresent = (maxP > (GATE_POWER * SIGNAL_MARGIN));

  // ---- render (no normalization, fixed gain mapping) ----
  for (int led = 0; led < NUM_LEDS; led++) {

    float gated = ledP[led] - GATE_POWER;
    if (!signalPresent || gated < 0.0f) gated = 0.0f;

    // Compressive mapping: level = 1 - exp(-gain * gated)
    float level = 1.0f - expf(-FFT_GAIN * gated);

    level *= BRIGHTNESS_GAIN;
    if (level > 1.0f) level = 1.0f;

    smoothed[led] = SMOOTHING * smoothed[led] + (1.0f - SMOOTHING) * level;
    if (smoothed[led] < 0.003f) smoothed[led] = 0.0f;

    uint8_t v = (uint8_t)(smoothed[led] * 255.0f);
    leds[led] = amplitudeToColor(v);
  }

  FastLED.show();
}
