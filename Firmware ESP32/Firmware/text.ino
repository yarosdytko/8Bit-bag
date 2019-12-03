//parametros para trabajar con texto
#define TEXT_DIRECTION 1  // 1 - texto en horizontal, 0 - texto en vertical
#define MIRR_V 0          // texto espejo en verical (0 / 1)
#define MIRR_H 0          // texto espejo en horizontal (0 / 1)

#define TEXT_HEIGHT 4     // margen de texto desde la base(abajo)
#define LET_WIDTH 5       // ancho de cada letra
#define LET_HEIGHT 8      // alto de cada letra
#define SPACE 1           // espacio

int offset = WIDTH;
boolean fullText;

// rutina de texto

void textRoutine(String text, uint32_t color) {
  if (loadingFlag) {
    offset = WIDTH;
    fullText = false;
    loadingFlag = false;
  }

  if (scrollTimer.isReady()) {
    FastLED.clear();
    byte i = 0, j = 0;
    while (text[i] != '\0') {
      if ((byte)text[i] > 191) {    // soporte para alfabeto cirilico
        i++;
      } else {
        drawLetter(j, text[i], offset + j * (LET_WIDTH + SPACE), color);
        i++;
        j++;
      }
    }
    fullText = false;

    offset--;
    if (offset < -j * (LET_WIDTH + SPACE)) {    // fin de linea
      offset = WIDTH + 3;
      fullText = true; // significa que se ha reproducido toda la linea
    }
    FastLED.show();
  }
}

void drawLetter(uint8_t index, uint8_t letter, int16_t offset, uint32_t color) {
  int8_t start_pos = 0, finish_pos = LET_WIDTH;
  CRGB letterColor;
  if (color == 1) letterColor = CHSV(byte(offset * 10), 255, 255);
  else if (color == 2) letterColor = CHSV(byte(index * 30), 255, 255);
  else letterColor = color;

  if (offset < -LET_WIDTH || offset > WIDTH) return;
  if (offset < 0) start_pos = -offset;
  if (offset > (WIDTH - LET_WIDTH)) finish_pos = WIDTH - offset;

  for (byte i = start_pos; i < finish_pos; i++) {
    int thisByte;
    if (MIRR_V) thisByte = getFont((byte)letter, LET_WIDTH - 1 - i);
    else thisByte = getFont((byte)letter, i);

    for (byte j = 0; j < LET_HEIGHT; j++) {
      boolean thisBit;

      if (MIRR_H) thisBit = thisByte & (1 << j);
      else thisBit = thisByte & (1 << (LET_HEIGHT - 1 - j));

      // dibujo columna (i - pos horizontal, j - pos vertical)
      if (TEXT_DIRECTION) {
        if (thisBit) leds[getPixelNumber(offset + i, TEXT_HEIGHT + j)] = letterColor;
        else drawPixelXY(offset + i, TEXT_HEIGHT + j, 0x000000);
      } else {
        if (thisBit) leds[getPixelNumber(i, offset + TEXT_HEIGHT + j)] = letterColor;
        else drawPixelXY(i, offset + TEXT_HEIGHT + j, 0x000000);
      }
    }
  }
}

uint8_t getFont(uint8_t font, uint8_t row) {
  font = font - '0' + 16;   // traduccion de simbolo ASCII a numero de array
  if (font <= 90) return pgm_read_byte(&(fontHEX[font][row]));     // simbolos en latin
  else if (font >= 112 && font <= 159) {    // simbolos en cirilico
    return pgm_read_byte(&(fontHEX[font - 17][row]));
  } else if (font >= 96 && font <= 111) {
    return pgm_read_byte(&(fontHEX[font + 47][row]));
  }
}
