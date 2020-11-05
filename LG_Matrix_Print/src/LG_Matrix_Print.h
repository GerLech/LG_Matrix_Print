/*
 * LG_Matrix_print.h
 * Version 1.0
 *      Author: Gerald Lechner
 */

/*
 * This is a driver for MAX7219 based LED matrix displays.
 * It is based on the library LEDMatrixDriver from Bartosz Bielawski
 * It was simplified but got extra functions to simple print text
 * on the MATRIX display. There is also a function to realize a ticker
 * this function needs only the message and the delay between scroll steps
 * The applied font has all standard ASCII characters and German umlauts
 *
 * The driver uses hardware SPI
 * up to 255 segments are supported.
 */
#ifndef LG_MATRIX_PRINT_H
#define LG_MATRIX_PRINT_H

#include <SPI.h>
#include <Arduino.h>

#ifdef ESP32
#include <cstring>
#endif

class LG_Matrix_Print
{
	//commands for MAX7219 defined in the datasheet
	const static uint16_t ENABLE =		0x0C00;
	const static uint16_t TEST =	 	0x0F00;
	const static uint16_t INTENSITY =	0x0A00;
	const static uint16_t SCAN_LIMIT =	0x0B00;
	const static uint16_t DECODE =		0x0900;

  const static uint8_t INVERT_SEGMENT_X = 1;
  const static uint8_t INVERT_DISPLAY_X = 2;
  const static uint8_t INVERT_Y = 4;


	public:
    //only number of segments N and CS Pin
    LG_Matrix_Print(uint8_t N, uint8_t csPin, uint8_t flags = 0);

    void setEnabled(bool enabled);
		//display brightness: 0 - 15
		void setIntensity(uint8_t level);
    void display();
    void setPixel(int16_t x, int16_t y, bool enabled);
    void clear() {memset(buffer, 0, 8*N);}
    void drawCharacter( uint8_t* bitmap, int x,int width );
    int pixelLength(String text);
    int printText(int start, String text, boolean isUTF8 = true);
    void ticker(String message, uint16_t wait, int16_t start);
    void ticker(String message, uint16_t wait);
    boolean updateTicker();
    void stopTicker();

  private:
    uint8_t* _getBufferPtr(int16_t x, int16_t y) const;
		void _sendCommand(uint16_t command);
		void _displayRow(uint8_t row);
    String _encodeUnicode(String text);

    const uint8_t N;
		SPISettings spiSettings;
		uint8_t csPin;
    uint8_t flags;
    uint8_t* buffer;
    boolean _tickerMode = false;
    int16_t _tickerPos = 0;
    uint16_t _wait = 100;
    uint32_t _last = 0;
    String _message;

    uint8_t font [103][9] {
                         {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,6}, // 32 Space
                         {0x80,0x80,0x80,0x80,0x80,0x00,0x80,0x00,2}, // 33 !
                         {0xa0,0xa0,0xa0,0x00,0x00,0x00,0x00,0x00,4}, // 34 "
                         {0x00,0x50,0xf8,0x50,0x50,0xf8,0x50,0x00,6}, // 35 #
                         {0x20,0x78,0xa0,0x70,0x28,0xf0,0x20,0x00,6}, // 36 $
                         {0xc8,0xc8,0x10,0x20,0x40,0x98,0x98,0x00,6}, // 37 %
                         {0x20,0x50,0x20,0x40,0xa8,0x90,0x68,0x00,6}, // 38 &
                         {0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,2}, // 39 '
                         {0x20,0x40,0x80,0x80,0x80,0x40,0x20,0x00,4}, // 40 (
                         {0x80,0x40,0x20,0x20,0x20,0x40,0x80,0x00,4}, // 41 )
                         {0x00,0xa8,0x70,0xf8,0x70,0xa8,0x00,0x00,6}, // 42 *
                         {0x00,0x20,0x20,0xf8,0x20,0x20,0x00,0x00,6}, // 43 +
                         {0x00,0x00,0x00,0x00,0x00,0x60,0x60,0xc0,4}, // 44 ,
                         {0x00,0x00,0x00,0xf8,0x00,0x00,0x00,0x00,6}, // 45 -
                         {0x00,0x00,0x00,0x00,0x00,0xc0,0xc0,0x00,3}, // 46 .
                         {0x08,0x08,0x10,0x20,0x40,0x80,0x80,0x00,6}, // 47 /
                         {0x20,0x50,0x88,0x88,0x88,0x50,0x20,0x00,6}, // 48 0
                         {0x30,0x50,0x90,0x10,0x10,0x10,0x38,0x00,6}, // 49 1
                         {0x70,0x88,0x88,0x10,0x20,0x40,0xf8,0x00,6}, // 50 2
                         {0x70,0x88,0x08,0x30,0x08,0x88,0x70,0x00,6}, // 51 3
                         {0x10,0x30,0x50,0x90,0xf8,0x10,0x10,0x00,6}, // 52 4
                         {0xf8,0x80,0xf0,0x08,0x08,0x88,0x70,0x00,6}, // 53 5
                         {0x38,0x40,0x80,0xf0,0x88,0x88,0x70,0x00,6}, // 54 6
                         {0xf8,0x88,0x10,0x20,0x20,0x20,0x20,0x00,6}, // 55 7
                         {0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00,6}, // 56 8
                         {0x70,0x88,0x88,0x78,0x08,0x10,0xe0,0x00,6}, // 57 9
                         {0x00,0xc0,0xc0,0x00,0x00,0xc0,0xc0,0x00,3}, // 58 :
                         {0x00,0x60,0x60,0x00,0x00,0x60,0x60,0xc0,4}, // 59 ;
                         {0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00,5}, // 60 <
                         {0x00,0x00,0xf0,0x00,0xf0,0x00,0x00,0x00,5}, // 61 =
                         {0x80,0x40,0x20,0x10,0x20,0x40,0x80,0x00,5}, // 62 >
                         {0x70,0x88,0x10,0x20,0x20,0x00,0x20,0x00,6}, // 63 ?
                         {0x70,0x88,0xb8,0xa8,0xa8,0x90,0x40,0x00,6}, // 64 @
                         {0x20,0x50,0x88,0x88,0xf8,0x88,0x88,0x00,6}, // 65 A
                         {0xf0,0x88,0x88,0xf0,0x88,0x88,0xf0,0x00,6}, // 66 B
                         {0x38,0x40,0x80,0x80,0x80,0x40,0x38,0x00,6}, // 67 C
                         {0xe0,0x90,0x88,0x88,0x88,0x90,0xe0,0x00,6}, // 68 D
                         {0xf8,0x80,0x80,0xf0,0x80,0x80,0xf8,0x00,6}, // 69 E
                         {0xf8,0x80,0x80,0xf0,0x80,0x80,0x80,0x00,6}, // 70 F
                         {0x70,0x88,0x80,0xb8,0x88,0x88,0x70,0x00,6}, // 71 G
                         {0x88,0x88,0x88,0xf8,0x88,0x88,0x88,0x00,6}, // 72 H
                         {0xe0,0x40,0x40,0x40,0x40,0x40,0xe0,0x00,4}, // 73 I
                         {0xf8,0x08,0x08,0x08,0x08,0x88,0x70,0x00,6}, // 74 J
                         {0x88,0x90,0xa0,0xc0,0xa0,0x90,0x88,0x00,6}, // 75 K
                         {0x80,0x80,0x80,0x80,0x80,0x80,0xf8,0x00,6}, // 76 L
                         {0x82,0xc6,0xaa,0x92,0x92,0x92,0x92,0x00,8}, // 77 M
                         {0x88,0xc8,0xc8,0xa8,0x98,0x98,0x88,0x00,6}, // 78 N
                         {0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00,6}, // 79 O
                         {0xf0,0x88,0x88,0xf0,0x80,0x80,0x80,0x00,6}, // 80 P
                         {0x70,0x88,0x88,0x88,0xa8,0x90,0x68,0x00,6}, // 81 Q
                         {0xf0,0x88,0x88,0xf0,0x90,0x88,0x88,0x00,6}, // 82 R
                         {0x78,0x80,0x80,0x70,0x08,0x08,0xf0,0x00,6}, // 83 S
                         {0xf8,0x20,0x20,0x20,0x20,0x20,0x20,0x00,6}, // 84 T
                         {0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,6}, // 85 U
                         {0x88,0x88,0x88,0x88,0x50,0x50,0x20,0x00,6}, // 86 V
                         {0x82,0x82,0x82,0x92,0x54,0x54,0x28,0x00,8}, // 87 W
                         {0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00,6}, // 88 X
                         {0x88,0x88,0x50,0x50,0x20,0x20,0x20,0x00,6}, // 89 Y
                         {0xf8,0x08,0x10,0x20,0x40,0x80,0xf8,0x00,6}, // 90 Z
                         {0xe0,0x80,0x80,0x80,0x80,0x80,0xe0,0x00,4}, // 91 [
                         {0x80,0x80,0x40,0x20,0x10,0x08,0x08,0x00,6}, // 92
                         {0xe0,0x20,0x20,0x20,0x20,0x20,0xe0,0x00,4}, // 93 ]
                         {0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,6}, // 94 ^
                         {0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x00,6}, // 95 _
                         {0x80,0x40,0x20,0x00,0x00,0x00,0x00,0x00,5}, // 96
                         {0x00,0x00,0xf0,0x08,0x78,0x98,0x68,0x00,6}, // 97 a
                         {0x80,0x80,0xf0,0x88,0x88,0xc8,0xb0,0x00,6}, // 98 b
                         {0x00,0x00,0x70,0x80,0x80,0x80,0x70,0x00,5}, // 99 c
                         {0x08,0x08,0x78,0x88,0x88,0x98,0x68,0x00,6}, // 100 d
                         {0x00,0x00,0x70,0x88,0xf8,0x80,0x70,0x00,6}, // 101 e
                         {0x30,0x40,0xf0,0x40,0x40,0x40,0x40,0x00,5}, // 102 f
                         {0x00,0x00,0x78,0x88,0x88,0x78,0x08,0xf0,6}, // 103 g
                         {0x80,0x80,0xe0,0x90,0x90,0x90,0x90,0x00,5}, // 104 h
                         {0x40,0x00,0xc0,0x40,0x40,0x40,0xe0,0x00,4}, // 105 i
                         {0x20,0x00,0x60,0x20,0x20,0x20,0x20,0xc0,4}, // 106 j
                         {0x80,0x80,0x90,0xa0,0xc0,0xa0,0x90,0x00,5}, // 107 k
                         {0xc0,0x40,0x40,0x40,0x40,0x40,0xe0,0x00,4}, // 108 l
                         {0x00,0x00,0xe8,0x54,0x54,0x54,0x54,0x00,7}, // 109 m
                         {0x00,0x00,0xd0,0x68,0x48,0x48,0x48,0x00,6}, // 110 n
                         {0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00,6}, // 111 o
                         {0x00,0x00,0xf0,0x48,0x48,0x70,0x40,0xe0,6}, // 112 p
                         {0x00,0x00,0x78,0x90,0x90,0x70,0x10,0x38,6}, // 113 q
                         {0x00,0x00,0xd0,0x68,0x40,0x40,0xe0,0x00,6}, // 114 r
                         {0x00,0x00,0x70,0x80,0x60,0x10,0xe0,0x00,5}, // 115 s
                         {0x40,0xe0,0x40,0x40,0x40,0x40,0x20,0x00,4}, // 116 t
                         {0x00,0x00,0x90,0x90,0x90,0x90,0x78,0x00,6}, // 117 u
                         {0x00,0x00,0x88,0x88,0x50,0x50,0x20,0x00,6}, // 118 v
                         {0x00,0x00,0x88,0x88,0xa8,0x50,0x50,0x00,6}, // 119 w
                         {0x00,0x00,0x90,0x90,0x60,0x90,0x90,0x00,5}, // 120 x
                         {0x00,0x00,0x90,0x90,0x90,0x70,0x10,0xe0,5}, // 121 y
                         {0x00,0x00,0xf8,0x10,0x20,0x40,0xf8,0x00,6}, // 122 z
                         {0x20,0x40,0x40,0x80,0x40,0x40,0x20,0x00,4}, // 123 {
                         {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,6}, // 124 |
                         {0x80,0x40,0x40,0x20,0x40,0x40,0x80,0x00,4}, // 125 }
                         {0x00,0x00,0x00,0x40,0xa8,0x10,0x00,0x00,6}, // 126 ~
                         {0x60,0x90,0x90,0xa0,0x90,0x90,0xa0,0x00,5}, // 127 ß
                         {0x88,0x20,0x50,0x88,0xf8,0x88,0x88,0x00,6}, // 128 Ä
                         {0x88,0x70,0x88,0x88,0x88,0x88,0x70,0x00,6}, // 129 Ö
                         {0x88,0x00,0x88,0x88,0x88,0x88,0x70,0x00,6}, // 130 Ü
                         {0x50,0x00,0xf0,0x08,0x78,0x98,0x68,0x00,6}, // 131 ä
                         {0x50,0x00,0x70,0x88,0x88,0x88,0x70,0x00,6}, // 132 ö
                         {0x50,0x00,0x90,0x90,0x90,0x90,0x78,0x00,6}, // 133 ü
                         {0x60,0x90,0x90,0x60,0x00,0x00,0x00,0x00,5}, // 134 °
    };

};

#endif
