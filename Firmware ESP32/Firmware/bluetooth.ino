//parametros para parser
#define  PARSE_AMOUNT 4
#define starting '$'
#define divider ' '
#define ending ';'

//variables para parser
int pIndex;
int pEffect;
byte inData[PARSE_AMOUNT];
String pString = "";
String pStringText = "";
enum mode {NORMAL, COLOR, TEXT} pMode;
boolean pStart;
boolean parseStarted;
boolean dataEnd;
boolean matrixSwitch;
boolean pEffectStart;
uint32_t pColor;
int pBrightness;
int pSpeed;

//flags -> variables de control
boolean effectsFlag;
boolean runningTextFlag;

void parser() {

  //****tratamiento****
  if (dataEnd) {
    dataEnd = false;

    switch (inData[0]) {
      case 0:
        pEffect = inData[1];
        if (pEffect != currentEffect) {
          effectUpdate = true;
          currentEffect = pEffect;
          currentMode = 1;
          Serial.print("Modo: ");
          Serial.println(currentMode);
          Serial.print("Efecto: ");
          Serial.println(currentEffect);
        }
        break;
      case 1:
        pBrightness = inData[1];
        if (pBrightness != currentBrightness) {
          currentBrightness = pBrightness;
          FastLED.setBrightness(currentBrightness);
          Serial.print("Brillo: ");
          Serial.println(currentBrightness);
        }
        break;
      case 2:
        pSpeed = inData[1];
        if (pSpeed != currentSpeed) {
          currentSpeed = pSpeed;
          effectTimer.setInterval(205 - currentSpeed);
          scrollTimer.setInterval(205 - currentSpeed);
          Serial.print("Velocidad: ");
          Serial.println(currentSpeed);
        }
        break;
      case 3:
        if (pStringText != text) {
          text = pStringText;
          pStringText = "";
          currentMode = 2;
          Serial.print("Modo: ");
          Serial.println(currentMode);
          Serial.print("Texto: ");
          Serial.println(text);
          drawPixelXY(1,1,0x00ff00);
          FastLED.show();
          delay(5);
        }
        break;
      case 4:
        if (pColor != globalColor) {
          colorFlag = true;
          Serial.print("Color: ");
          Serial.println(pColor);
          setColor(pColor);
        }
        break;
      case 5:
        if (inData[1] != 0 ) {
          extraParameter = inData[1];
          extraFlag = true;
          Serial.print("Parametro extra: ");
          Serial.println(extraParameter);
        } else {
          Serial.println("Parametro extra no valido");
          extraFlag = false;
        }
        break;
      case 6:
        pEffectStart = inData[1];
        if (pEffectStart) {
          effectStart = pEffectStart;
          Serial.println("Efecto start");
        } else {
          effectStart = pEffectStart;
          Serial.println("Efecto stop");
        }
        break;
    }
  }

  //*******parsing*******
  if (SerialBT.available() > 0) {
    char inByte;

    if (pMode == TEXT) {
      pStringText = SerialBT.readString();
      inByte = ending;
      pMode = NORMAL;
    } else {
      inByte = SerialBT.read();
    }

    if (pStart) {
      if (inByte != divider && inByte != ending) {
        pString += inByte;
      } else {
        if (pIndex == 0) {
          byte t = pString.toInt();
          if (t == 3) {
            pMode = TEXT;
          } else if (t == 4) {
            pMode = COLOR;
          } else {
            pMode = NORMAL;
          }
        }
        if (pIndex == 1) {
          if (pMode == NORMAL) inData[pIndex] = pString.toInt();
          if (pMode == COLOR) pColor = strtol(&pString[0], NULL, 16);
        } else {
          inData[pIndex] = pString.toInt();
        }
        pString = "";
        pIndex++;
      }
    }

    if (inByte == starting) {
      Serial.println("data start");
      pStart = true;
      pIndex = 0;
      pString = "";
    }
    if (inByte == ending) {
      Serial.println("data end");
      pMode = NORMAL;
      pStart = false;
      dataEnd = true;
    }
  }
}


// hex string to uint32_t
/*
  uint32_t HEXtoInt(String hexValue) {
  byte tens, ones, number1, number2, number3;
  tens = (hexValue[0] < '9') ? hexValue[0] - '0' : hexValue[0] - '7';
  ones = (hexValue[1] < '9') ? hexValue[1] - '0' : hexValue[1] - '7';
  number1 = (16 * tens) + ones;

  tens = (hexValue[2] < '9') ? hexValue[2] - '0' : hexValue[2] - '7';
  ones = (hexValue[3] < '9') ? hexValue[3] - '0' : hexValue[3] - '7';
  number2 = (16 * tens) + ones;

  tens = (hexValue[4] < '9') ? hexValue[4] - '0' : hexValue[4] - '7';
  ones = (hexValue[5] < '9') ? hexValue[5] - '0' : hexValue[5] - '7';
  number3 = (16 * tens) + ones;

  return ((uint32_t)number1 << 16 | (uint32_t)number2 << 8 | number3 << 0);
  }*/
