#include <avr/io.h>
#include <avr/interrupt.h> 
#include <avr/eeprom.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#define NOTE_MASK 0x1F
#define NOTE_VIBRATO 0x20
#define NOTE_OCTAVE  0x40
#define NOTE_SHORT   0x80

#if defined(__AVR_ATmega8__)
#define OCR1A_PIN PB1
#else
#define OCR1A_PIN PB3
#define ONLY32MELODY
#endif


const uint8_t MelodyArray[] PROGMEM = {
0xCB,0xDF,0xC8,0xDF,0xC4,0xDF,0xC0,0xDF,0xC0,0x99,0x96,0x96,0x90,0x96,0x96,0x8B,
0xCB,0xDF,0xC8,0xDF,0xC4,0xDF,0xC0,0xDF,0xC0,0x99,0x96,0x96,0x90,0x8B,0x8B,0x8B,
0x80,0x8B,0x8B,0x90,0x93,0x99,0x99,0x93,0x90,0x8B,0x8B,0x86,0x82,0x80,0x80,0xDF,
0x80,0x8B,0x8B,0x90,0x93,0x99,0x99,0x93,0x99,0xC0,0xC0,0x93,0x99,0xC0,0xC0,0xC0,
0x96,0x96,0x96,0x96,0x84,0x88,0x8B,0x90,0x96,0x96,0x96,0x96,0x84,0x88,0x8B,0x90,
0x96,0x96,0xCB,0xCB,0xC8,0xC8,0xC4,0xC4,0x96,0x96,0x96,0x96,0xDF,0xDF,0xDF,0xDF,
0x90,0x90,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC9,0xC0,0x9C,0x9C,0x9C,0x9C,0x9C,0x9C,
0xC0,0x9C,0x96,0x96,0x96,0x96,0x96,0x96,0x8B,0x88,0x84,0x84,0x84,0x84,0x84,0x84,
0xC6,0xC2,0x8D,0xDF,0xCD,0xDF,0xCD,0xDF,0xC6,0xC2,0x8D,0xDF,0xCD,0xDF,0xCD,0xDF,
0xC6,0xC2,0x8D,0xDF,0xCD,0xDF,0x86,0xDF,0xCD,0xDF,0x82,0xDF,0xCB,0xDF,0xCB,0xDF,
0x84,0x84,0x99,0x99,0x99,0x99,0x99,0x99,0x96,0x90,0x96,0x96,0x96,0x96,0x96,0x96,
0x84,0x84,0x96,0x96,0x96,0x96,0x96,0x96,0x99,0x96,0x90,0x90,0x90,0x90,0x90,0xDF,
0x90,0x8D,0x90,0x86,0x84,0x80,0x80,0x80,0x90,0x8D,0x90,0x86,0x84,0x80,0x80,0x80,
0x90,0x8D,0x90,0x93,0xDF,0x90,0xDF,0x86,0xDF,0x80,0xDF,0x84,0x84,0xDF,0xDF,0xDF,
0xD0,0xD3,0xD0,0xD0,0xC0,0xC4,0xC6,0xC0,0xD0,0xD3,0xD0,0xD0,0xC4,0xC6,0xCB,0xC4,
0xD0,0xD3,0xD0,0xD0,0xC4,0xC6,0xCB,0xCB,0xD0,0xD3,0xD0,0xD0,0xDF,0xDF,0xDF,0xDF,
0x96,0x96,0x90,0x8B,0x88,0x8B,0x90,0x90,0xDF,0x90,0x8B,0x88,0x84,0x88,0x8B,0x8B,
0xDF,0x8B,0x88,0x88,0xDF,0x88,0x99,0x99,0xDF,0x99,0x96,0x90,0x8B,0x88,0x84,0xDF,
0x96,0x96,0x88,0x96,0x88,0x96,0x88,0x96,0xC0,0xC0,0x96,0xC0,0x96,0xC0,0x96,0xC0,
0xC8,0xC8,0xC0,0xC8,0xC4,0xC4,0x9C,0xC4,0xC0,0xC0,0x96,0xC0,0x9C,0x9C,0xC8,0xC8,
0x08,0x16,0x40,0x1C,0x16,0x40,0x16,0x1C,0x16,0x0B,0x16,0x08,0x08,0x08,0xDF,0x08,
0x16,0x40,0x1C,0x16,0x40,0x16,0x1C,0x16,0x0B,0x08,0x04,0x04,0x04,0x04,0xDF,0xDF,
0x48,0x4B,0x44,0x44,0x40,0x40,0x1C,0x16,0x50,0x56,0x50,0x50,0x4B,0x5F,0x4B,0x5F,
0x40,0x44,0x40,0x40,0x1C,0x1C,0x16,0x10,0x4B,0x50,0x4B,0x4B,0x48,0x5F,0x48,0x5F,
0xC8,0xCB,0xC8,0xCB,0xC8,0xCB,0xC8,0xC4,0xC4,0x9C,0x9C,0xDF,0xCB,0xC8,0xC4,0xC4,
0x9C,0x9C,0xDF,0xD0,0xCB,0xCB,0xCB,0xCB,0xC8,0xC8,0xC8,0xDF,0xDF,0xDF,0xDF,0xDF,
0x96,0xD0,0xD0,0xCB,0x96,0xD0,0xCB,0x96,0xD0,0xD0,0xCB,0xCB,0xCB,0xDF,0xDF,0x96,
0xCB,0xCB,0xC8,0x96,0xCB,0xC8,0x96,0xCB,0xCB,0xC8,0xC8,0xC8,0xDF,0xDF,0xDF,0xDF,
0xC0,0xC0,0x9C,0x9C,0x96,0x96,0x93,0x93,0x93,0x96,0x9C,0x9C,0x9C,0x9C,0xDF,0x88,
0x93,0x9C,0xCB,0xC8,0xCB,0xC8,0x96,0x96,0x96,0x9C,0xC0,0xC0,0xC0,0xC0,0xDF,0xDF,
0xC2,0xC6,0xC8,0xC6,0xC2,0x9C,0xC6,0xDF,0xDF,0xDF,0xC6,0xC6,0xC6,0xDF,0xDF,0xC2,
0xC6,0xC8,0xC6,0xC2,0xC6,0xC2,0xDF,0xDF,0xDF,0x9C,0x9C,0x9C,0xDF,0xDF,0xDF,0xDF,
0x96,0xC0,0x96,0xC0,0xC4,0xC4,0xC0,0x9C,0x96,0xC0,0x96,0xC0,0xC4,0xC4,0xC0,0x9C,
0x96,0x96,0x96,0xD6,0xD0,0xCB,0xC8,0xC4,0xC0,0xC0,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,
0xC0,0xC4,0xC8,0xC8,0xC0,0xC4,0xC8,0xC4,0xC0,0x9C,0x96,0x9C,0xC0,0xC4,0x9C,0x90,
0xC0,0xC4,0xC8,0xC8,0xC0,0xC4,0xC8,0xC4,0xC0,0x9C,0x96,0xC4,0x9C,0x90,0xC0,0xC0,
0xED,0xED,0xED,0xF0,0xF6,0xF6,0xFC,0xFC,0xF0,0xF0,0xE8,0xE8,0xFC,0xFC,0xF6,0xF0,
0xED,0xED,0xED,0xF0,0xF6,0xF6,0xFC,0xFC,0xF0,0xF0,0xE8,0xFF,0xE8,0xE8,0xE8,0xFF,
0xF6,0xEB,0xF9,0xF6,0xF6,0xEB,0xE8,0xE4,0xF0,0xE8,0xF6,0xF0,0xF0,0xF0,0xFF,0xFF,
0xF0,0xE8,0xF6,0xF0,0xF0,0xF6,0xF0,0xF6,0xE4,0xE4,0xE4,0xE4,0xFF,0xFF,0xFF,0xFF,
0x9C,0x96,0x96,0xC0,0xC8,0xD0,0xCB,0xCB,0xC0,0x9C,0x9C,0xC8,0x9C,0xC0,0xC0,0xC0,
0x9C,0x96,0x96,0xC0,0xC8,0xD6,0xDC,0xDC,0xD6,0xD0,0xD0,0xC4,0xCB,0xC8,0xC8,0xC8,
0x96,0xC0,0x90,0x90,0x90,0x90,0x90,0x90,0x96,0xC0,0x8B,0x8B,0x8B,0x8B,0x8B,0x8B,
0x90,0x96,0x99,0x99,0x90,0x90,0x88,0x88,0x90,0x90,0x8B,0x8B,0x8B,0x8B,0x8B,0x8B,
0x99,0x99,0x99,0x99,0xC0,0xC0,0x99,0x99,0x96,0x96,0x90,0x90,0x90,0x90,0x8B,0x90,
0x96,0x96,0x96,0x96,0x99,0x99,0x96,0x96,0x90,0x90,0x8B,0x8B,0x8B,0x8B,0x9F,0x9F,
0xC4,0xC8,0xD0,0xCB,0xD0,0xCB,0x9C,0x9C,0xC0,0xC4,0xCB,0xC8,0xCB,0xC8,0x96,0x96,
0x9C,0xC0,0xC8,0xC8,0xC4,0xC4,0xC0,0x9C,0xC4,0xC0,0x96,0x96,0x96,0x96,0x96,0xDF,
0x96,0x9C,0xC2,0xC4,0xC4,0x96,0x96,0xC0,0x99,0x96,0x90,0x8B,0x8B,0x84,0x84,0x9F,
0x84,0x88,0x8B,0x90,0x90,0x99,0x99,0x96,0x90,0x8B,0x90,0x96,0x96,0x96,0x96,0x96,
0xC8,0xC4,0xC0,0xC0,0x96,0x96,0x93,0x93,0x96,0x96,0x9C,0x9C,0x88,0x88,0x88,0x88,
0xC8,0xC4,0xC0,0xC0,0x96,0x96,0xD6,0xD6,0xD6,0xD0,0xCB,0xCB,0xCB,0xCB,0xCB,0xCB,
0x96,0xCB,0xC8,0xC4,0xC8,0xCB,0xD6,0xD0,0xC4,0xC8,0xC8,0xC8,0x96,0xD0,0xCB,0xC8,
0xD0,0xCB,0xC8,0xC4,0xC8,0xCB,0xCB,0xCB,0xCB,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0x96,0x96,0x96,0x96,0x99,0x96,0xCB,0xCB,0xCB,0xCB,0xC8,0xC4,0xC2,0xC2,0xC2,0xC2,
0xC8,0xC2,0x96,0x96,0x96,0x96,0x96,0x96,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0x9C,0xC0,0x9C,0x90,0x88,0x90,0x90,0x8D,0x96,0x9C,0x96,0x8D,0x96,0xC0,0xC0,0x9C,
0xC4,0xC8,0xC4,0x9C,0x90,0x9C,0x9C,0x96,0x96,0x9C,0x96,0x9C,0xC0,0xC4,0xC4,0xC4,
0xC4,0xC0,0xC4,0x96,0x8B,0x96,0x84,0x84,0xC4,0xC0,0xC4,0x96,0x8B,0x96,0x84,0x84,
0xC4,0xC8,0xCB,0xC8,0xCB,0xC4,0xC8,0xC4,0xC8,0xC0,0xC4,0xC0,0xC4,0x96,0xC4,0xC4,
0x96,0x96,0x96,0x96,0x8B,0x90,0x96,0x96,0x96,0x96,0x96,0xDF,0x96,0x90,0x96,0xCB,
0xCB,0xC8,0xC4,0xC4,0xC4,0xC4,0xC4,0xC4,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0x96,0x9C,0xC0,0xC4,0xC8,0xC8,0xC0,0x9C,0x96,0x96,0x93,0x96,0xC0,0xC0,0x9C,0xDF,
0x9C,0xC0,0xC4,0xC8,0xD0,0xD0,0xCB,0xC8,0xC4,0xC4,0xC0,0xC4,0xC8,0xC8,0xC8,0xC8
#if !defined(ONLY32MELODY)
,
0x8B,0x96,0xCB,0xC8,0xC4,0x96,0x99,0x96,0x8B,0x8B,0x8B,0x8B,0xDF,0x8B,0x96,0xCB,
0xC8,0xC4,0x96,0x99,0x96,0xC0,0xC0,0x99,0x99,0x99,0x99,0xDF,0xDF,0xDF,0xDF,0xDF,
0x8B,0x8B,0x84,0x84,0x96,0x99,0x96,0x90,0x8B,0x88,0x84,0x84,0x84,0x84,0x8B,0x8B,
0x96,0x96,0xC0,0xC4,0xC0,0x99,0x96,0x96,0x96,0x96,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0xB0,0xB0,0xB6,0xB6,0xBC,0xBC,0xE4,0xE4,0xE4,0xE4,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE4,0xE0,0xB0,0xB0,0xA6,0xA6,0xB0,0xB0,0xB0,0xB0,0xAB,0xAB,0xAB,0xAB,0xAB,0xAB,
0x80,0x8B,0x8B,0x8B,0x90,0x99,0x93,0x90,0x8B,0xC2,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0x8B,0x99,0x99,0x99,0xC0,0xC6,0xC2,0xC0,0x99,0xC2,0xC2,0xC2,0xC2,0xC2,0xDF,0xDF,
0x90,0x96,0x9C,0xC0,0xC0,0x90,0x90,0x90,0x90,0xC4,0xC6,0xC0,0xC0,0x90,0x90,0x90,
0xDF,0x90,0x93,0x99,0x99,0x96,0x99,0xC0,0xC0,0x99,0x93,0x93,0x93,0x90,0x90,0x90,
0x8D,0x8B,0x8D,0x84,0x84,0x84,0x84,0x84,0x9C,0x99,0x9C,0x90,0x90,0x90,0x90,0xDF,
0x90,0x8D,0x8B,0x88,0x88,0x88,0x88,0xDF,0x88,0x9C,0x88,0x96,0x96,0x96,0x96,0x96,
0x99,0xC0,0xC2,0xC6,0xC6,0xCB,0xCB,0xCB,0xDF,0xCB,0xC6,0xC2,0xCB,0xCB,0xC0,0xC0,
0xC0,0xC0,0xC2,0xC2,0xC0,0xC0,0x8B,0x90,0x93,0x93,0x90,0x90,0x8B,0x8B,0x8B,0x8B,
0xD0,0xD0,0xCB,0xCB,0xC6,0xC4,0xC4,0xC4,0xD3,0xD3,0xD0,0xD0,0xC6,0xC0,0xC0,0xC0,
0xC6,0xC6,0xC4,0xC4,0xC0,0xC0,0x9C,0x9C,0xD3,0xD3,0xD0,0xD0,0xD0,0xD0,0xDF,0xDF,
0x04,0x0B,0x16,0x0B,0x10,0x10,0x0B,0x08,0x16,0x16,0x10,0x10,0x04,0x04,0x04,0x04,
0x0B,0x16,0x40,0x40,0x44,0x44,0x40,0x19,0x16,0x16,0x16,0x16,0x16,0x5F,0x5F,0xDF,
0xEB,0xEB,0xEB,0xF0,0xF6,0xF6,0xF0,0xEB,0xE8,0xE4,0xE8,0xEB,0xF0,0xF0,0xE0,0xE0,
0xEB,0xEB,0xEB,0xF0,0xF6,0xF6,0xF0,0xEB,0xE8,0xE4,0xE8,0xEB,0xF0,0xF0,0xF0,0xF0,
0x8B,0x84,0x8B,0x99,0x99,0x99,0x99,0x99,0x96,0x90,0x84,0x8B,0x8B,0x8B,0x8B,0x8B,
0xDF,0x8B,0x8B,0x90,0x90,0x90,0x90,0x90,0x8B,0x86,0x84,0x8B,0x8B,0x86,0x86,0x86,
0x8B,0x90,0x93,0x93,0x8B,0x90,0x93,0x93,0x8B,0x90,0x93,0x8B,0x90,0x90,0x90,0x90,
0x88,0x8B,0x90,0x90,0x88,0x8B,0x90,0x90,0xC0,0x99,0x93,0x90,0x8B,0x8B,0x8B,0x8B,
0xE0,0xE2,0xE0,0xB9,0xB3,0xB3,0xAB,0xAB,0xB9,0xFF,0xB3,0xAB,0xAB,0xAB,0xAB,0xB9,
0xFF,0xB3,0xAB,0xAB,0xAB,0xAB,0xA8,0xA0,0xA8,0xAB,0xAB,0xAB,0xAB,0xFF,0xFF,0xFF,
0xCB,0xCB,0xC0,0xCB,0xC6,0xC2,0x99,0x99,0xC2,0xC0,0x90,0x90,0xC2,0xC0,0x93,0x93,
0xCB,0xCB,0xC8,0xCB,0xD0,0xCB,0xC0,0xDF,0xC0,0xD3,0xD0,0xD3,0xCB,0xCB,0xCB,0xCB,
0x84,0x84,0x86,0x84,0x90,0x90,0x96,0x90,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
0x84,0x84,0x90,0x99,0xC4,0xC4,0xC6,0xC4,0x96,0x96,0x96,0x96,0x96,0x96,0x96,0x96,
0xC0,0xC0,0x99,0x99,0x99,0x99,0xC2,0xC6,0xCB,0xC6,0xC2,0xC0,0xC0,0xC0,0xDF,0xC0,
0xC0,0x99,0x99,0x99,0x99,0x90,0x93,0x99,0x93,0x90,0x8B,0x8B,0x8B,0x8B,0xDF,0xDF,
0x60,0x3C,0x36,0x36,0x36,0x3C,0x60,0x36,0x60,0x6D,0x68,0x68,0x3C,0x3C,0x3C,0x3C,
0x30,0x28,0x64,0x64,0x64,0x60,0x3C,0x60,0x3C,0x36,0x68,0x68,0x68,0x68,0x7F,0x7F,
0x62,0x62,0x62,0x62,0x62,0x36,0x33,0x2D,0x24,0x24,0x3C,0x3C,0x3C,0x7F,0x3C,0x3C,
0x3C,0x3C,0x3C,0x33,0x2D,0x28,0x36,0x36,0x36,0x36,0x33,0x33,0x33,0x33,0x7F,0x7F,
0x30,0x36,0x30,0x28,0x28,0x28,0x28,0x28,0x30,0x68,0x60,0x64,0x64,0x64,0x64,0x64,
0x30,0x36,0x30,0x6B,0x6B,0x6B,0x6B,0x6B,0x30,0x36,0x3C,0x60,0x60,0x60,0x60,0x60,
0xB3,0xB3,0xB3,0xB3,0xAB,0xAB,0xB3,0xB3,0xB0,0xB0,0xB0,0xB0,0xA0,0xA0,0xFF,0xA0,
0xB3,0xB0,0xAB,0xA6,0xAB,0xAB,0xE0,0xE0,0xB0,0xB0,0xB0,0xB0,0xA0,0xA0,0xA0,0xA0,
0x90,0x8B,0x86,0x90,0x99,0xC2,0xC0,0x99,0x93,0x93,0x93,0x93,0xDF,0x90,0x8B,0x86,
0x90,0x99,0xCB,0xCB,0xCB,0xC6,0xC6,0xC2,0xC0,0xC0,0xC0,0xC0,0xDF,0xDF,0xDF,0xDF,
0x90,0x90,0x93,0x99,0x99,0x93,0x90,0x8B,0x8B,0x99,0x99,0xDF,0x8B,0x8B,0x90,0x93,
0x93,0x90,0x8B,0x90,0x90,0x86,0x86,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0x88,0x8B,0x88,0x86,0x88,0x80,0x80,0x80,0x88,0x8B,0x88,0x86,0x88,0x80,0x80,0x80,
0x88,0xC0,0x9C,0x96,0x88,0x90,0x8B,0x88,0x8B,0x88,0x84,0x84,0x82,0x84,0x84,0x84,
0x96,0x9C,0x96,0x96,0x88,0x88,0x88,0x88,0x96,0x9C,0x96,0x96,0x88,0x88,0x88,0x88,
0xC0,0xC0,0xC0,0x88,0xC0,0x9C,0x96,0x90,0x8B,0x8B,0x8B,0x8B,0x8B,0x8B,0xDF,0xDF,
0x93,0x99,0xC0,0xC0,0x93,0x99,0xC0,0xC0,0x93,0x99,0xC0,0xC0,0x99,0x93,0x90,0x90,
0x90,0x86,0x84,0x84,0x8B,0x93,0xC0,0xC0,0x99,0x93,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xC8,0xC0,0x96,0x8D,0x88,0x8D,0x90,0x9C,0xC8,0xC8,0x9C,0x9C,0x9C,0x9C,0xDF,0xC8,
0xC0,0x96,0x8D,0x88,0x8D,0x90,0x9C,0xC8,0xC8,0xC8,0xC8,0xC8,0xDF,0xDF,0xDF,0xDF,
0x26,0x20,0x2B,0x2B,0x20,0x20,0x20,0x20,0x26,0x20,0x2B,0x2B,0x20,0x20,0x20,0x20,
0x26,0x26,0x33,0x33,0x30,0x30,0x2B,0x2B,0x26,0x20,0x2B,0x2B,0x60,0x60,0x60,0x60,
0xC2,0xC2,0xC0,0xC0,0x99,0x99,0x8B,0x8B,0x8B,0x93,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xDF,0xC0,0x99,0x93,0x90,0x8B,0x80,0x80,0x80,0x86,0x90,0x90,0x90,0x90,0x90,0x90,
0x88,0x96,0x9C,0xC0,0x9C,0x9C,0x96,0x88,0x88,0x88,0x88,0xDF,0x88,0x96,0x9C,0xC0,
0xC8,0xD0,0xD0,0xCB,0x9C,0x9C,0x9C,0x9C,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,
0x96,0x9C,0xC0,0xC0,0x9C,0x9C,0x96,0x96,0x93,0x96,0xC0,0xC0,0x9C,0x9C,0x9C,0x9C,
0x93,0x96,0x9C,0x9C,0x96,0x96,0x93,0x93,0x8D,0x93,0x9C,0x9C,0x96,0x96,0x96,0x96,
0x90,0x90,0xC0,0xC0,0xC0,0xC0,0xC6,0xC6,0xC6,0xC6,0xC4,0xC4,0xC0,0xC0,0xC0,0xC0,
0x9C,0xC0,0xC4,0xC4,0xC4,0xC0,0x99,0x99,0xC0,0xC0,0x90,0x90,0x90,0x90,0x90,0xDF,
0xC6,0xC6,0xC0,0x90,0x90,0x90,0x90,0xDF,0x90,0xC0,0xC6,0xC4,0xC0,0xC4,0xC4,0x9C,
0x90,0x90,0x90,0x90,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF
#endif /* 32MELODY */
};

#define CLOCK_FREQ 1000000

/* ������������ ������� 12 ��� */
#define G_PERIOD      (uint16_t)(CLOCK_FREQ/392.00) // ����
#define G_HASH_PERIOD (uint16_t)(CLOCK_FREQ/415.30) // ���� ����
#define A_PERIOD      (uint16_t)(CLOCK_FREQ/440.00) // ��
#define A_HASH_PERIOD (uint16_t)(CLOCK_FREQ/466.16) // �� ������
#define B_PERIOD      (uint16_t)(CLOCK_FREQ/493.88) // ��
#define C_PERIOD      (uint16_t)(CLOCK_FREQ/523.25) // ��
#define C_HASH_PERIOD (uint16_t)(CLOCK_FREQ/554.36) // �� ����
#define D_PERIOD      (uint16_t)(CLOCK_FREQ/587.32) // ��
#define D_HASH_PERIOD (uint16_t)(CLOCK_FREQ/622.26) // �� ����
#define E_PERIOD      (uint16_t)(CLOCK_FREQ/659.26) // ��
#define F_PERIOD      (uint16_t)(CLOCK_FREQ/698.46) // ��
#define F_HASH_PERIOD (uint16_t)(CLOCK_FREQ/739.98) // ��
#define PAUSE_PERIOD  (uint16_t)(CLOCK_FREQ/4000)   // Pause
#define VIBRATO_STROBE (uint16_t)(CLOCK_FREQ*120.0/1000000) /* 120uS */


#define T0_FREQ (uint16_t)(CLOCK_FREQ/8/256)
#define VIBRATO_INTERVAL    (uint16_t)(T0_FREQ*50.0/1000) /* 50mS */
#define SHORT_NOTE_INTERVAL (uint16_t)(T0_FREQ*260.0/1000) /* 260mS */
#define LONG_NOTE_INTERVAL  (uint16_t)(T0_FREQ*400.0/1000) /* 260mS */

#define MELODY_MASK (sizeof(MelodyArray)/32-1)
#define CAMERA_DELAY (uint16_t)(T0_FREQ*60)


volatile uint16_t Period = 0;
volatile uint16_t MelodyStart = 0; /* (0 - 63)*32 */
volatile uint8_t  NoteNumber; /* 0 - 31 */
volatile uint8_t  VibratoFlag; /* Next period must be increased */
volatile uint8_t  MelodyPalaing = 0; /* The melody is plaing */
volatile uint8_t  NoteLengthCounter;
volatile uint8_t  VibratoCounter;
uint8_t  Note;
volatile uint8_t MelodyNumber;
volatile uint16_t CameraDelay;

ISR(TIMER1_OVF_vect)
{
  uint16_t Tmp;

  /* ���������� ������������ ����������� ��������� */
  OCR1A = Period/2;
 
  /* ������������ ������� - ������ ������� */
  /* ������ ����������� �� 1/8 ��� ��������� ������ ������� */
  if (VibratoFlag)
  {
    Tmp = Period - VIBRATO_STROBE;
    VibratoFlag = 0; /* � ��������� ������ ����� ��������� ������� ������������ ������� */
  }
  else
  {
    Tmp = Period - 1;
    TIMSK &= ~(1<<TOIE1); /* ���������� ������� ���������� ����� �������� ���� */
  }
  ICR1 = Tmp;
}

static uint16_t GetNotePeriod(uint8_t _Note)
{
  _Note = _Note & NOTE_MASK;
  switch (_Note)
  {
    case 0: //����
      return G_PERIOD;
    case 2: //����#
      return G_HASH_PERIOD;
    case 4: //��
      return A_PERIOD;
    case 6: //��#
      return A_HASH_PERIOD;
    case 8: //��
    case 9: 
      return B_PERIOD;
    case 11: //��
      return C_PERIOD;
    case 13: //��#
      return C_HASH_PERIOD;
    case 16: //��
      return D_PERIOD;
    case 19: //��#
      return D_HASH_PERIOD;
    case 22: //��
      return E_PERIOD;
    case 25: //��
      return F_PERIOD;
    case 28: //��#
      return F_HASH_PERIOD;
   } //switch
   return PAUSE_PERIOD;
}

/* Every 2mS */
ISR(TIMER0_OVF_vect)
{
  if(CameraDelay)
  {
    CameraDelay--;
    if (CameraDelay == 0)
      PORTB = 0;
  }
  if (!MelodyPalaing)
    return;
  NoteLengthCounter--;
  if (NoteLengthCounter == 0) /* The note is end */
  {
    if (NoteNumber < 34)
    {
      if (NoteNumber >= 32)
        Note = 31;
      else
        Note = pgm_read_byte(MelodyArray + MelodyStart + NoteNumber);

      NoteNumber++;

      Period = GetNotePeriod(Note);
      if (Period == PAUSE_PERIOD)
      {
        VibratoCounter = 0xFF;
        OCR1A = 0xFFFF; /* Off the output. PWM with 0 duty */
        TIMSK &= ~(1<<TOIE1); /* ���������� ������� ���������� ����� �������� ���� */
      }
      else /* Pause */
      {
        if (Note & NOTE_OCTAVE)
        {
          Period = Period/2;
        }
        if (Note & NOTE_VIBRATO)
          VibratoCounter = VIBRATO_INTERVAL;
        else
          VibratoCounter = 0xFF;
        TIFR = TOV1; /* Reset interrupt flag to start handler at begin of period */
        TIMSK |= (1<<TOIE1); /* ��������� ���������� T1 ����� �������� ���� */
      } /* !Pause */
      if (Note & NOTE_SHORT)
        NoteLengthCounter = SHORT_NOTE_INTERVAL;
      else
        NoteLengthCounter = LONG_NOTE_INTERVAL;
    }
    else /* End of melody */
    {
      OCR1A = 0XFFFF; /* Off the output. PWM with 0 duty */
      TIMSK &= ~(1<<TOIE1); /* ���������� ������� ���������� ����� �������� ���� */
      MelodyPalaing = 0;
    }

    return;
  }
  if (VibratoCounter != 0xFF) /* Vibrato is on */
  {
     VibratoCounter--;
     if (VibratoCounter)
       return;
     VibratoCounter = VIBRATO_INTERVAL;
     VibratoFlag = 1;
     TIFR = TOV1; /* Reset interrupt flag to start handler at begin of period */
     TIMSK |= (1<<TOIE1); /* ��������� ���������� T1 ����� �������� ���� */
  }
}

void main( void )
{
  //��������� ������ � �������
  DDRD &= ~((1 << PD2) | (1 << PD6)); //������� ������� (������, ���������)
  PORTD |= (1 << PD2) | (1 << PD6);   //�������� ��� ��� ������������� ���������
  DDRB  |= ((1 << PB0) | (1 << OCR1A_PIN)); //������  pb1 - speaker
  PORTB = 0;

  //��������� ���������� � ��������
  OCR1A = 0xFFFF; /* OFF */
  ICR1  = 4;  /* Very fast */

  TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1A0); //14 mode - fast pwm
  TCCR1B = (1<<CS10) | (1<<WGM13) | (1<<WGM12); //Full spped - fast pwm 14 mode
 
  TIMSK = (1<<TOIE0);

#if defined(__AVR_ATmega8__)
  TCCR0 = 
#else
  TCCR0B =    
#endif
    (1 << CS01); //���� ������� 0 � ������������� 8 (������ ������ 2 mS)

  MelodyPalaing = 0;
  sei (); //��������� ��������� ����������

  while (1) //
  {

    //�������� ������� ������ ������
    if ( (PIND & (1 << PD2)) == 0 &&
      MelodyPalaing == 0 )
    {
      //���������, ����������� �� ��������� ���������� ������ �������
      if ((PIND & (1 << PD6)) == 0)
      {
        MelodyNumber = TCNT0; //������������� ��������� ����� �������
      }
      else
      {
        MelodyNumber++;
      }
      MelodyStart = (MelodyNumber & MELODY_MASK)*32;
      NoteNumber = 0;
      NoteLengthCounter = 1;
      CameraDelay = CAMERA_DELAY;
      PORTB |= (1<<PB0);
      MelodyPalaing = 1;
    } //���������� ��������� ������� ������ ������
  } //while (1)
}
