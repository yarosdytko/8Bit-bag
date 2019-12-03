//parametros internos de los efectos
// bola, bolas
#define BALL_SPEED 270    // velocidad base de bola y bolas

// bola
#define BALL_SIZE 2       // tamaño de la bola
#define RANDOM_COLOR 2    // color random para semilla

// bolas
#define BALLS_AMOUNT 3    // cantidad de bolas
#define CLEAR_PATH 0      // limpiar el camino
#define BALL_TRACK 1      // cola de la bola
#define TRACK_STEP 120    // longitud de la cola, mayor numero -> menor cola

// nieve
#define SNOW_DENSE 8        // densidad de la nieve, menor numero -> mas denso, mayor numero -> menos denso
#define SNOW_SPEED 350      // velocidad de la nieve
#define SNOW_COLOR 0xE0FFFF // color por de la nieve

// fuego
#define SPARKLES 1        // cenizas encendido -1 / apagado - 0
#define HUE_ADD 10        // correccion de color del fuego (valores de 0 - 230)

// arcoiris diagonal
int maxDim = max(WIDTH, HEIGHT);

// lluvia de estrellas
#define TAIL_STEP 120     // longitud de estrellas
#define SATURATION 220    // saturacion
#define STAR_DENSE 40     // densidad de estrellas

// destellos
#define DENSE 3           // densidad de destellos
#define BRIGHT_STEP 70    // intensidad de apagado de destello 

/*************************************************************************************************************/
byte hue = 1;
boolean colorChanged = false;
int snowDense = SNOW_DENSE;
uint32_t snowColor = SNOW_COLOR;
int starDense = STAR_DENSE;
int sparklesFade = BRIGHT_STEP;

/*******************************algunas funciones de uso interno**********************************************/
void effectsRoutine() {

  if (effectUpdate) {
    FastLED.clear();
    effectUpdate = false;
    colorChanged = false;
    colorFlag = false;
    loadingFlag = true;
    snowColor = SNOW_COLOR;
    hue = 1;
  }
  switch (currentEffect) {
    case 0:
      clearRoutine();
    case 1:
      snowRoutine();
      break;
    case 2:
      ballRoutine();
      break;
    case 3:
      ballsRoutine();
      break;
    case 4:
      rainbowRoutine();
      break;
    case 5:
      rainbowDiagonalRoutine();
      break;
    case 6:
      fireRoutine();
      break;
    case 7:
      matrixRoutine();
      break;
    case 8:
      sparklesRoutine();
      break;
    case 9:
      starfallRoutine();
      break;
  }
  FastLED.show();
}

void clearRoutine() {
  FastLED.clear();
  FastLED.show();
  delay(1);
}

/******************************************Efecto Nieve*****************************************************/
void snowRoutine() {
  if (extraFlag) {
    if (extraParameter > 1) {
      snowDense = extraParameter;
    } else {
      snowDense = SNOW_DENSE;
    }
    extraFlag = false;
  }
  if (colorFlag) {
    snowColor = globalColor;
    colorChanged = true;
  } else {
    snowColor = SNOW_COLOR;
    colorChanged = false;
  }

  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT - 1; y++) {
      drawPixelXY(x, y, getPixColorXY(x, y + 1));
    }
  }

  for (byte x = 0; x < WIDTH; x++) {
    if (getPixColorXY(x, HEIGHT - 2) == 0 && (random(0, snowDense) == 0)) {
      if (colorChanged) {
        drawPixelXY(x, HEIGHT - 1, snowColor);
      } else {
        drawPixelXY(x, HEIGHT - 1, snowColor - 0x101010 * random(0, 4));
      }
    } else {
      drawPixelXY(x, HEIGHT - 1, 0x000000);
    }
  }

  /*  for (byte x = 0; x < WIDTH; x++) {
      if (getPixColorXY(x, HEIGHT - 2) == 0 && (random(0, snowDense) == 0))
        drawPixelXY(x, HEIGHT - 1, 0xE0FFFF - 0x101010 * random(0, 4));
      else
        drawPixelXY(x, HEIGHT - 1, 0x000000);
    }*/

  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Cubo herrante*********************************************/
int coordB[0];
int8_t vectorB[2];
CRGB ballColor;

void ballRoutine() {
  if (loadingFlag) {
    for (byte i = 0; i < 2; i++) {
      coordB[i] = WIDTH / 2 * 10;
      vectorB[i] = random(8, 20);
      ballColor = CHSV(random(0, 9) * 28, 255, 255);
    }
    loadingFlag = false;
  }
  for (byte i = 0; i < 2; i++) {
    coordB[i] += vectorB[i];
    if (coordB[i] < 0) {
      coordB[i] = 0;
      vectorB[i] = -vectorB[i];
      if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255, 255);
      //vectorB[i] += random(0, 6) - 3;
    }
  }
  if (coordB[0] > (WIDTH - BALL_SIZE) * 10) {
    coordB[0] = (WIDTH - BALL_SIZE) * 10;
    vectorB[0] = -vectorB[0];
    if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255, 255);
    //vectorB[0] += random(0, 6) - 3;
  }
  if (coordB[1] > (HEIGHT - BALL_SIZE) * 10) {
    coordB[1] = (HEIGHT - BALL_SIZE) * 10;
    vectorB[1] = -vectorB[1];
    if (RANDOM_COLOR) ballColor = CHSV(random(0, 9) * 28, 255, 255);
    //vectorB[1] += random(0, 6) - 3;
  }
  FastLED.clear();
  for (byte i = 0; i < BALL_SIZE; i++) {
    for (byte j = 0; j < BALL_SIZE; j++) {
      leds[getPixelNumber(coordB[0] / 10 + i, coordB[1] / 10 + j)] = ballColor;
    }
  }

  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Bolas*****************************************************/
int coord[BALLS_AMOUNT][2];
int8_t vector[BALLS_AMOUNT][2];
CRGB ballColors[BALLS_AMOUNT];

void ballsRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    for (byte j = 0; j < BALLS_AMOUNT; j++) {
      int sign;
      coord[j][0] = WIDTH / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][0] = random(4, 15) * sign;
      coord[j][1] = HEIGHT / 2 * 10;
      random(0, 2) ? sign = 1 : sign = -1;
      vector[j][1] = random(4, 15) * sign;
      ballColors[j] = CHSV(random(0, 9) * 28, 255, 255);
    }
  }

  if (!BALL_TRACK)
    FastLED.clear();
  else {
    fader(TRACK_STEP);
  }

  for (byte j = 0; j < BALLS_AMOUNT; j++) {

    for (byte i = 0; i < 2; i++) {
      coord[j][i] += vector[j][i];
      if (coord[j][i] < 0) {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }

    if (coord[j][0] > (WIDTH - 1) * 10) {
      coord[j][0] = (WIDTH - 1) * 10;
      vector[j][0] = -vector[j][0];
    }
    if (coord[j][1] > (HEIGHT - 1) * 10) {
      coord[j][1] = (HEIGHT - 1) * 10;
      vector[j][1] = -vector[j][1];
    }
    leds[getPixelNumber(coord[j][0] / 10, coord[j][1] / 10)] =  ballColors[j];
  }

  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Fuego*****************************************************/
unsigned char matrixValue[8][16];
unsigned char line[WIDTH];
int pcnt = 0;

const unsigned char valueMask[8][16] PROGMEM = {
  {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};

const unsigned char hueMask[8][16] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};

void fireRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    FastLED.clear();
    generateLine();
    memset(matrixValue, 0, sizeof(matrixValue));
  }
  if (pcnt >= 100) {
    shiftUp();
    generateLine();
    pcnt = 0;
  }
  drawFrame(pcnt);
  pcnt += 30;

  if (currentSpeed >= 194) {
    delay(2);
  }
}

void generateLine() {
  for (uint8_t x = 0; x < WIDTH; x++) {
    line[x] = random(64, 255);
  }
}

void shiftUp() {
  for (uint8_t y = HEIGHT - 1; y > 0; y--) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y > 7) continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    matrixValue[0][newX] = line[newX];
  }
}

void drawFrame(int pcnt) {
  int nextv;
  for (unsigned char y = HEIGHT - 1; y > 0; y--) {
    for (unsigned char x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y < 8) {
        nextv =
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&(valueMask[y][newX]));

        CRGB color = CHSV(
                       HUE_ADD + pgm_read_byte(&(hueMask[y][newX])), // H
                       255, // S
                       (uint8_t)max(0, nextv) // V
                     );

        leds[getPixelNumber(x, y)] = color;
      } else if (y == 8 && SPARKLES) {
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0) drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);
      } else if (SPARKLES) {
        if (getPixColorXY(x, y - 1) > 0)
          drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);

      }
    }
  }
  for (unsigned char x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    CRGB color = CHSV(
                   HUE_ADD + pgm_read_byte(&(hueMask[0][newX])), // H
                   255,           // S
                   (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
                 );
    leds[getPixelNumber(newX, 0)] = color;
  }
}

/******************************************Efecto Arcoiris*****************************************************/
void rainbowRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    hue = 1;
  }
  FastLED.setBrightness(currentBrightness / 2);
  hue += 3;
  for (byte i = 0; i < WIDTH; i++) {
    CHSV thisColor = CHSV((byte)(hue + i * float(255 / WIDTH)), 255, 255);
    for (byte j = 0; j < HEIGHT; j++)
      leds[getPixelNumber(i, j)] = thisColor;
  }

  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Arcoiris diagonal********************************************/
void rainbowDiagonalRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    hue = 1;
  }
  FastLED.setBrightness(currentBrightness / 2);
  hue += 3;
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) {
      CHSV thisColor = CHSV((byte)(hue + (float)(WIDTH / HEIGHT * x + y) * (float)(255 / WIDTH)), 255, 255);
      //drawPixelXY(x, y, thisColor);
      leds[getPixelNumber(x, y)] = thisColor;
    }
  }

  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Matrix(como en la pelicula)**********************************/
void matrixRoutine() {

  for (byte x = 0; x < WIDTH; x++) {
    uint32_t thisColor = getPixColorXY(x, HEIGHT - 1);
    if (thisColor == 0)
      drawPixelXY(x, HEIGHT - 1, 0x00FF00 * (random(0, 10) == 0));
    else if (thisColor < 0x002000)
      drawPixelXY(x, HEIGHT - 1, 0);
    else
      drawPixelXY(x, HEIGHT - 1, thisColor - 0x002000);
  }
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT - 1; y++) {
      drawPixelXY(x, y, getPixColorXY(x, y + 1));
    }
  }
  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Lluvia de estrellas******************************************/
void starfallRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
  }

  if (extraFlag) {
    if (extraParameter > 9) {
      starDense = 60 - extraParameter;
    } else {
      starDense = STAR_DENSE;
    }
  }

  for (byte i = HEIGHT / 2; i < HEIGHT; i++) {
    if (getPixColorXY(0, i) == 0
        && (random(0, starDense) == 0)
        && getPixColorXY(0, i + 1) == 0
        && getPixColorXY(0, i - 1) == 0)
      leds[getPixelNumber(0, i)] = CHSV(random(0, 200), SATURATION, 255);
  }

  for (byte i = 0; i < WIDTH / 2; i++) {
    if (getPixColorXY(i, HEIGHT - 1) == 0
        && (random(0, starDense) == 0)
        && getPixColorXY(i + 1, HEIGHT - 1) == 0
        && getPixColorXY(i - 1, HEIGHT - 1) == 0)
      leds[getPixelNumber(i, HEIGHT - 1)] = CHSV(random(0, 200), SATURATION, 255);
  }

  for (byte y = 0; y < HEIGHT - 1; y++) {
    for (byte x = WIDTH - 1; x > 0; x--) {
      drawPixelXY(x, y, getPixColorXY(x - 1, y + 1));
    }
  }

  for (byte i = HEIGHT / 2; i < HEIGHT; i++) {
    fadePixel(0, i, TAIL_STEP);
  }
  for (byte i = 0; i < WIDTH / 2; i++) {
    fadePixel(i, HEIGHT - 1, TAIL_STEP);
  }

  if (currentSpeed >= 194) {
    delay(2);
  }
}

/******************************************Efecto Destello*****************************************************/
void sparklesRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
  }

  for (byte i = 0; i < DENSE; i++) {
    byte x = random(0, WIDTH);
    byte y = random(0, HEIGHT);
    if (getPixColorXY(x, y) == 0)
      leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
  }
  fader(sparklesFade);

  if (currentSpeed >= 194) {
    delay(2);
  }
}
