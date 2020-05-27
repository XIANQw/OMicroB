#ifndef PROTOCOL_H
#define PROTOCOL_H

#define SET_PIXEL(x, y, v) (1 << 25) | ((x) << 13) | ((y) << 1) | (v)
#define CLEAR_SCREEN (2<<25)
#define DIGITAL_WRITE(p, l) (3<<25)| ((p) << 17) | ((l) & 0b11111111111111111)
#define ANALOG_WRITE(p, l) (4<<25) | ((p) << 17) | ((l) & 0b11111111111111111)
#define PIN_MODE(p, mode) (5<<25) | ((p) << 17) | ((mode) & 0b11111111111111111)
#define SHOW (6<<25)


#endif