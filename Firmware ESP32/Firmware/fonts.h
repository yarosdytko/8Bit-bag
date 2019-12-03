// fuente para efecto texto
const uint8_t fontHEX[][5] PROGMEM = {
  {0x00, 0x00, 0x00, 0x00, 0x00}, //   0x20 32
  {0x00, 0x00, 0x6f, 0x00, 0x00}, // ! 0x21 33
  {0x00, 0x07, 0x00, 0x07, 0x00}, // " 0x22 34
  {0x14, 0x7f, 0x14, 0x7f, 0x14}, // # 0x23 35
  {0x00, 0x07, 0x04, 0x1e, 0x00}, // $ 0x24 36
  {0x23, 0x13, 0x08, 0x64, 0x62}, // % 0x25 37
  {0x36, 0x49, 0x56, 0x20, 0x50}, // & 0x26 38
  {0x00, 0x00, 0x07, 0x00, 0x00}, // ' 0x27 39
  {0x00, 0x1c, 0x22, 0x41, 0x00}, // ( 0x28 40
  {0x00, 0x41, 0x22, 0x1c, 0x00}, // ) 0x29 41
  {0x14, 0x08, 0x3e, 0x08, 0x14}, // * 0x2a 42
  {0x08, 0x08, 0x3e, 0x08, 0x08}, // + 0x2b 43
  {0x00, 0x50, 0x30, 0x00, 0x00}, // , 0x2c 44
  {0x08, 0x08, 0x08, 0x08, 0x08}, // - 0x2d 45
  {0x00, 0x60, 0x60, 0x00, 0x00}, // . 0x2e 46
  {0x20, 0x10, 0x08, 0x04, 0x02}, // / 0x2f 47
  {0x3e, 0x51, 0x49, 0x45, 0x3e}, // 0 0x30 48
  {0x00, 0x42, 0x7f, 0x40, 0x00}, // 1 0x31 49
  {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 0x32 50
  {0x21, 0x41, 0x45, 0x4b, 0x31}, // 3 0x33 51
  {0x18, 0x14, 0x12, 0x7f, 0x10}, // 4 0x34 52
  {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 0x35 53
  {0x3c, 0x4a, 0x49, 0x49, 0x30}, // 6 0x36 54
  {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 0x37 55
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 0x38 56
  {0x06, 0x49, 0x49, 0x29, 0x1e}, // 9 0x39 57
  {0x00, 0x36, 0x36, 0x00, 0x00}, // : 0x3a 58
  {0x00, 0x56, 0x36, 0x00, 0x00}, // ; 0x3b 59
  {0x08, 0x14, 0x22, 0x41, 0x00}, // < 0x3c 60
  {0x14, 0x14, 0x14, 0x14, 0x14}, // = 0x3d 61
  {0x00, 0x41, 0x22, 0x14, 0x08}, // > 0x3e 62
  {0x02, 0x01, 0x51, 0x09, 0x06}, // ? 0x3f 63
  {0x3e, 0x41, 0x5d, 0x49, 0x4e}, // @ 0x40 64
  {0x7e, 0x09, 0x09, 0x09, 0x7e}, // A 0x41 65
  {0x7f, 0x49, 0x49, 0x49, 0x36}, // B 0x42 66
  {0x3e, 0x41, 0x41, 0x41, 0x22}, // C 0x43 67
  {0x7f, 0x41, 0x41, 0x41, 0x3e}, // D 0x44 68
  {0x7f, 0x49, 0x49, 0x49, 0x41}, // E 0x45 69
  {0x7f, 0x09, 0x09, 0x09, 0x01}, // F 0x46 70
  {0x3e, 0x41, 0x49, 0x49, 0x7a}, // G 0x47 71
  {0x7f, 0x08, 0x08, 0x08, 0x7f}, // H 0x48 72
  {0x00, 0x41, 0x7f, 0x41, 0x00}, // I 0x49 73
  {0x20, 0x40, 0x41, 0x3f, 0x01}, // J 0x4a 74
  {0x7f, 0x08, 0x14, 0x22, 0x41}, // K 0x4b 75
  {0x7f, 0x40, 0x40, 0x40, 0x40}, // L 0x4c 76
  {0x7f, 0x02, 0x0c, 0x02, 0x7f}, // M 0x4d 77
  {0x7f, 0x04, 0x08, 0x10, 0x7f}, // N 0x4e 78
  {0x3e, 0x41, 0x41, 0x41, 0x3e}, // O 0x4f 79
  {0x7f, 0x09, 0x09, 0x09, 0x06}, // P 0x50 80
  {0x3e, 0x41, 0x51, 0x21, 0x5e}, // Q 0x51 81
  {0x7f, 0x09, 0x19, 0x29, 0x46}, // R 0x52 82
  {0x46, 0x49, 0x49, 0x49, 0x31}, // S 0x53 83
  {0x01, 0x01, 0x7f, 0x01, 0x01}, // T 0x54 84
  {0x3f, 0x40, 0x40, 0x40, 0x3f}, // U 0x55 85
  {0x0f, 0x30, 0x40, 0x30, 0x0f}, // V 0x56 86
  {0x3f, 0x40, 0x30, 0x40, 0x3f}, // W 0x57 87
  {0x63, 0x14, 0x08, 0x14, 0x63}, // X 0x58 88
  {0x07, 0x08, 0x70, 0x08, 0x07}, // Y 0x59 89
  {0x61, 0x51, 0x49, 0x45, 0x43}, // Z 0x5a 90
  {0x3c, 0x4a, 0x49, 0x29, 0x1e}, // [ 0x5b 91
  {0x02, 0x04, 0x08, 0x10, 0x20}, // \ 0x5c 92
  {0x00, 0x41, 0x7f, 0x00, 0x00}, // ] 0x5d 93
  {0x04, 0x02, 0x01, 0x02, 0x04}, // ^ 0x5e 94
  {0x40, 0x40, 0x40, 0x40, 0x40}, // _ 0x5f 95
  {0x00, 0x00, 0x03, 0x04, 0x00}, // ` 0x60 96
  {0x20, 0x54, 0x54, 0x54, 0x78}, // a 0x61 97
  {0x7f, 0x48, 0x44, 0x44, 0x38}, // b 0x62 98
  {0x38, 0x44, 0x44, 0x44, 0x20}, // c 0x63 99
  {0x38, 0x44, 0x44, 0x48, 0x7f}, // d 0x64 100
  {0x38, 0x54, 0x54, 0x54, 0x18}, // e 0x65 101
  {0x08, 0x7e, 0x09, 0x01, 0x02}, // f 0x66 102
  {0x0c, 0x52, 0x52, 0x52, 0x3e}, // g 0x67 103
  {0x7f, 0x08, 0x04, 0x04, 0x78}, // h 0x68 104
  {0x00, 0x44, 0x7d, 0x40, 0x00}, // i 0x69 105
  {0x20, 0x40, 0x44, 0x3d, 0x00}, // j 0x6a 106
  {0x00, 0x7f, 0x10, 0x28, 0x44}, // k 0x6b 107
  {0x00, 0x41, 0x7f, 0x40, 0x00}, // l 0x6c 108
  {0x7c, 0x04, 0x18, 0x04, 0x78}, // m 0x6d 109
  {0x7c, 0x08, 0x04, 0x04, 0x78}, // n 0x6e 110
  {0x38, 0x44, 0x44, 0x44, 0x38}, // o 0x6f 111
  {0x7c, 0x14, 0x14, 0x14, 0x08}, // p 0x70 112
  {0x08, 0x14, 0x14, 0x18, 0x7c}, // q 0x71 113
  {0x7c, 0x08, 0x04, 0x04, 0x08}, // r 0x72 114
  {0x48, 0x54, 0x54, 0x54, 0x20}, // s 0x73 115
  {0x04, 0x3f, 0x44, 0x40, 0x20}, // t 0x74 116
  {0x3c, 0x40, 0x40, 0x20, 0x7c}, // u 0x75 117
  {0x1c, 0x20, 0x40, 0x20, 0x1c}, // v 0x76 118
  {0x3c, 0x40, 0x30, 0x40, 0x3c}, // w 0x77 119
  {0x44, 0x28, 0x10, 0x28, 0x44}, // x 0x78 120
  {0x0c, 0x50, 0x50, 0x50, 0x3c}, // y 0x79 121
  {0x44, 0x64, 0x54, 0x4c, 0x44}, // z 0x7a 122
  {0x00, 0x08, 0x36, 0x41, 0x41}, // { 0x7b 123
  {0x00, 0x00, 0x7f, 0x00, 0x00}, // | 0x7c 124
  {0x41, 0x41, 0x36, 0x08, 0x00}, // } 0x7d 125
  {0x04, 0x02, 0x04, 0x08, 0x04}, // ~ 0x7e 126

  {0x7e, 0x09, 0x09, 0x09, 0x7e}, // А 192
  {0x7F, 0x49, 0x49, 0x49, 0x71}, // Б
  {0x7f, 0x49, 0x49, 0x49, 0x36}, // В
  {0x7F, 0x01, 0x01, 0x01, 0x01}, // Г
  {0x60, 0x3E, 0x21, 0x3F, 0x60}, // Д
  {0x7f, 0x49, 0x49, 0x49, 0x41}, // Е
  {0x76, 0x08, 0x7F, 0x08, 0x76}, // Ж
  {0x21, 0x41, 0x45, 0x4b, 0x31}, // З
  {0x7F, 0x20, 0x10, 0x08, 0x7F}, // И
  {0x7E, 0x20, 0x11, 0x08, 0x7E}, // Й
  {0x7f, 0x08, 0x14, 0x22, 0x41}, // К
  {0x70, 0x0E, 0x01, 0x01, 0x7F}, // Л
  {0x7f, 0x02, 0x0c, 0x02, 0x7f}, // М
  {0x7f, 0x08, 0x08, 0x08, 0x7f}, // Н
  {0x3e, 0x41, 0x41, 0x41, 0x3e}, // О
  {0x7F, 0x01, 0x01, 0x01, 0x7F}, // П
  {0x7f, 0x09, 0x09, 0x09, 0x06}, // Р
  {0x3e, 0x41, 0x41, 0x41, 0x22}, // С
  {0x01, 0x01, 0x7f, 0x01, 0x01}, // Т
  {0x07, 0x48, 0x48, 0x48, 0x7F}, // У
  {0x1C, 0x22, 0x7F, 0x22, 0x1C}, // Ф
  {0x63, 0x14, 0x08, 0x14, 0x63}, // Х
  {0x7F, 0x40, 0x40, 0x7F, 0xC0}, // Ц
  {0x07, 0x08, 0x08, 0x08, 0x7F}, // Ч
  {0x7F, 0x40, 0x7F, 0x40, 0x7F}, // Ш
  {0x7F, 0x40, 0x7F, 0x40, 0xFF}, // Щ
  {0x01, 0x7F, 0x48, 0x48, 0x70}, // Ъ
  {0x7F, 0x48, 0x70, 0x00, 0x7F}, // Ы
  {0x00, 0x7F, 0x48, 0x48, 0x70}, // Ь
  {0x22, 0x41, 0x49, 0x49, 0x3E}, // Э
  {0x7F, 0x08, 0x3E, 0x41, 0x3E}, // Ю
  {0x46, 0x29, 0x19, 0x09, 0x7F}, // Я 223

  {0x20, 0x54, 0x54, 0x54, 0x78}, //a 224
  {0x3c, 0x4a, 0x4a, 0x49, 0x31}, //б
  {0x7c, 0x54, 0x54, 0x28, 0x00}, //в
  {0x7c, 0x04, 0x04, 0x04, 0x0c}, //г
  {0xe0, 0x54, 0x4c, 0x44, 0xfc}, //д
  {0x38, 0x54, 0x54, 0x54, 0x18}, //e
  {0x6c, 0x10, 0x7c, 0x10, 0x6c}, //ж
  {0x44, 0x44, 0x54, 0x54, 0x28}, //з
  {0x7c, 0x20, 0x10, 0x08, 0x7c}, //и
  {0x7c, 0x41, 0x22, 0x11, 0x7c}, //й
  {0x7c, 0x10, 0x28, 0x44, 0x00}, //к
  {0x20, 0x44, 0x3c, 0x04, 0x7c}, //л
  {0x7c, 0x08, 0x10, 0x08, 0x7c}, //м
  {0x7c, 0x10, 0x10, 0x10, 0x7c}, //н
  {0x38, 0x44, 0x44, 0x44, 0x38}, //o
  {0x7c, 0x04, 0x04, 0x04, 0x7c}, //п
  {0x7C, 0x14, 0x14, 0x14, 0x08}, //p
  {0x38, 0x44, 0x44, 0x44, 0x20}, //c
  {0x04, 0x04, 0x7c, 0x04, 0x04}, //т
  {0x0C, 0x50, 0x50, 0x50, 0x3C}, //у
  {0x30, 0x48, 0xfc, 0x48, 0x30}, //ф
  {0x44, 0x28, 0x10, 0x28, 0x44}, //x
  {0x7c, 0x40, 0x40, 0x40, 0xfc}, //ц
  {0x0c, 0x10, 0x10, 0x10, 0x7c}, //ч
  {0x7c, 0x40, 0x7c, 0x40, 0x7c}, //ш
  {0x7c, 0x40, 0x7c, 0x40, 0xfc}, //щ
  {0x04, 0x7c, 0x50, 0x50, 0x20}, //ъ
  {0x7c, 0x50, 0x50, 0x20, 0x7c}, //ы
  {0x7c, 0x50, 0x50, 0x20, 0x00}, //ь
  {0x28, 0x44, 0x54, 0x54, 0x38}, //э
  {0x7c, 0x10, 0x38, 0x44, 0x38}, //ю
  {0x08, 0x54, 0x34, 0x14, 0x7c}, //я 255
};
