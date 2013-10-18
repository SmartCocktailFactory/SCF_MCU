/****************************************************************************************
 Copyright (C) Zuehlke Engineering
 All Rights Reserved
 ****************************************************************************************

 DESCRIPTION:        Interface of the MODLED8x8 extension board device.

 ****************************************************************************************/

#ifndef MODLED8X8_H_
#define MODLED8X8_H_

#include <stdint.h>

typedef enum {
  SCROLL_LEFT, SCROLL_RIGHT
} ScrollDirection;

/** The font lookup table. */
extern const uint8_t ModLED8x8_FontLookup[][5];

/** Initialize the MODLED8x8 board. */
void ModLED8x8_init();

/** clear the video buffer. */
void ModLED8x8_vClear();

/** draw drawPixel at x,y coordinates to MOD-LED8x8RGB 1,1 is upper left corner. */
void ModLED8x8_drawPixel(uint32_t X, uint32_t Y);

/** draw a line from x1,y1 to x2,y2. */
void ModLED8x8_drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/** draw a rectangle from x1,y1 to x2,y2. */
void ModLED8x8_drawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/** draw a solid rectangle. */
void ModLED8x8_drawSolidRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/** draw an ellipse & fix radius if negative. */
void ModLED8x8_drawEllipse(int32_t CX, int32_t CY, int32_t XRadius, int32_t YRadius);

/** draw a circle. */
void ModLED8x8_drawCircle(int32_t x, int32_t y, int32_t r);

/** draw a triangle. */
void ModLED8x8_drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);

/** draw static character if within the Font limit. */
void ModLED8x8_drawChar(uint8_t c);

/** draw static string. */
void ModLED8x8_drawString(uint8_t c[]);

/** scroll everything left. */
void ModLED8x8_leftScroll();

/** scroll everything right. */
void ModLED8x8_rightScroll();

/** move everything left/right until it leaves the screen. */
void ModLED8x8_theEnder(ScrollDirection directions);

/** scroll one character left if within the Font limit. */
void ModLED8x8_scrollCharLeft(uint8_t c);

/** scroll one character right if within the Font limit. */
void ModLED8x8_scrollCharRight(uint8_t c);

/** draw a scrolling string. */
void ModLED8x8_scrollString(uint8_t c[], ScrollDirection directions);

#endif /* MODLED8X8_H_ */
