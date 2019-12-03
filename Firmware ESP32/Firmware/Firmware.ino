#include <FastLED.h>
#include "BluetoothSerial.h"
#include "fonts.h"
#include "timerMinim.h"

//matriz
#define WIDTH 16 // ancho de la matriz
#define HEIGHT 16 // alto

//leds
#define NUM_LEDS WIDTH * HEIGHT // numero de leds total en la matriz
#define DATA_PIN 12  // pin de datos para la comunicacion con los leds
#define COLOR_ORDER GRB
#define CURRENT_LIMIT 2000 // limite de corriente en miliamperios, autoajuste de brillo

//array de leds
CRGB leds[NUM_LEDS];

//parametros para efectos
#define EFFECT_SPEED 100  // velocidad estandar para efectoss
#define BRIGHTNESS 100    // brillo estandar, valores entre 0 y 255
#define TEXT_SPEED 100    // velocidad de texto , en ms 

uint32_t globalColor = 0x00ff00;
int currentMode = 255; //modo de funcionamiento 0 - demo, 1 - efectos, 2 - texto
int currentBrightness = BRIGHTNESS;
int currentSpeed = EFFECT_SPEED;
int currentEffect = 255;
int extraParameter = 0;
boolean effectUpdate = false;
boolean effectStart = false;
boolean effectIsRunning = false;
boolean loadingFlag = true;
boolean extraFlag = false;
boolean colorFlag = false;
String text = "";

//objeto timer
timerMinim effectTimer(currentSpeed);
timerMinim scrollTimer(currentSpeed);


//objeto bluetooth
BluetoothSerial SerialBT;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
  } else if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected");
  }
}

void setup() {
  Serial.begin(115200);
  //bluetooth setup
  SerialBT.register_callback(callback);
  if (!SerialBT.begin("8BitBag")) {
    Serial.println("Error bluetooth initializing");
  } else {
    Serial.println("All fine, bluetooth initialized");
  }

  //leds setup
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) {
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  }
  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(0) + analogRead(1)); // generar numeros aleatorios
}

void loop() {
  parser();
  if (effectStart) {
    if (currentMode == 1) {
      if (effectTimer.isReady()) {
        effectsRoutine();
      }
    }
    if (currentMode == 2) {
      textRoutine(text, globalColor);
    }
  } else {
    clearRoutine();
    effectIsRunning = false;
  }
}
