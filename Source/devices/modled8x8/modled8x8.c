/****************************************************************************************
 Copyright (C) Zuehlke Engineering
 All Rights Reserved
 ****************************************************************************************

 DESCRIPTION:        Interface of the MODLED8x8 extension board device.

 ****************************************************************************************/

#include "modled8x8.h"
#include <bsp/omx_p207_eval/spi/spi.h>
#include <stdlib.h>

/** Number of MOD-LED8x8RGB in columns. */
#define MODLED8X8_BIG_X 5

/** Number of MOD-LED8x8RGB in rows. */
#define MODLED8X8_BIG_Y 1 /*2*/

/** Total number of MOD-LED8x8RGBs connected together. */
#define MODLED8X8_NUMBER_X MODLED8X8_BIG_X*MODLED8X8_BIG_Y

/** The delay in ms during scrolling. */
static const uint8_t scrollDelay = 100;

/** The video buffer. */
static uint8_t videoBuffer[MODLED8X8_NUMBER_X * 24];

static uint8_t color = 1;
static uint8_t cX = 1;
static uint8_t cY = 1;

//----------------------------------------------------------------------------------------------

/** Send data to the display. */
static void Transfer() {
  omsEval_spi_write(sizeof(videoBuffer), videoBuffer);
}

/** function needed for drawing an ellipse. */
static void Draw_4_Ellipse_Points(int32_t CX, int32_t CY, int32_t X, int32_t Y) {
  ModLED8x8_drawPixel(CX + X, CY + Y);
  ModLED8x8_drawPixel(CX - X, CY + Y);
  ModLED8x8_drawPixel(CX - X, CY - Y);
  ModLED8x8_drawPixel(CX + X, CY - Y);
}

void ModLED8x8_init() {
  ModLED8x8_vClear();
  cX = 1;
  cY = 1;
  color = 1;
}

void ModLED8x8_vClear() {
  int32_t i;
  for (i = 0; i < MODLED8X8_NUMBER_X * 24; i++)
    videoBuffer[i] = 0;
}

void ModLED8x8_drawPixel(uint32_t X, uint32_t Y) {
  if (Y <= MODLED8X8_BIG_Y * 8 && X <= MODLED8X8_BIG_X * 8 && X > 0 && Y > 0) {
    if (Y > 8)
      X = (X + MODLED8X8_BIG_X * 8) * ((Y - 1) / 8);
    Y = Y % 8;
    if (Y == 0)
      Y = 8;

    int32_t p;
    p = MODLED8X8_NUMBER_X - ((X - 1) / 8) - 1;

    videoBuffer[3 * (Y - 1) + 24 * p] &= ~(1 << ((X - 1) % 8)); //turn off chosen drawPixel
    videoBuffer[3 * (Y - 1) + 1 + 24 * p] &= ~(1 << ((X - 1) % 8));
    videoBuffer[3 * (Y - 1) + 2 + 24 * p] &= ~(1 << ((X - 1) % 8));

    if (color & 1)
      videoBuffer[3 * (Y - 1) + 24 * p] |= (1 << ((X - 1) % 8)); //set color to the drawPixel
    if (color & 2)
      videoBuffer[3 * (Y - 1) + 1 + 24 * p] |= (1 << ((X - 1) % 8));
    if (color & 4)
      videoBuffer[3 * (Y - 1) + 2 + 24 * p] |= (1 << ((X - 1) % 8));
  }
}

void ModLED8x8_drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  int32_t dx, dy, sx, sy, err, e2;

  dx = abs(x2 - x1);
  dy = abs(y2 - y1);
  if (x1 < x2)
    sx = 1;
  else
    sx = -1;
  if (y1 < y2)
    sy = 1;
  else
    sy = -1;
  err = dx - dy;
  do {
    ModLED8x8_drawPixel(x1, y1);
    if ((x1 == x2) && (y1 == y2))
      break;
    e2 = 2 * err;
    if (e2 > -dy) {
      err = err - dy;
      x1 = x1 + sx;
    }
    if (e2 < dx) {
      err = err + dx;
      y1 = y1 + sy;
    }
  } while (1);
}

void ModLED8x8_drawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  ModLED8x8_drawLine(x1, y1, x1, y2);
  ModLED8x8_drawLine(x1, y1, x2, y1);
  ModLED8x8_drawLine(x2, y1, x2, y2);
  ModLED8x8_drawLine(x1, y2, x2, y2);
}

void ModLED8x8_drawSolidRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  int32_t i;
  if (x2 > x1)
    for (i = x1; i <= x2; i++)
      ModLED8x8_drawLine(i, y1, i, y2);
  else
    for (i = x2; i <= x1; i++)
      ModLED8x8_drawLine(i, y1, i, y2);
}

void ModLED8x8_drawEllipse(int32_t CX, int32_t CY, int32_t XRadius, int32_t YRadius) {
  int32_t X, Y, XChange, YChange, EllipseError, TwoASquare, TwoBSquare, StoppingX, StoppingY;
  if (XRadius < 0)
    XRadius = -XRadius;
  if (YRadius < 0)
    YRadius = -YRadius;

  TwoASquare = 2 * XRadius * XRadius;
  TwoBSquare = 2 * YRadius * YRadius;
  X = XRadius;
  Y = 0;
  XChange = YRadius * YRadius * (1 - 2 * XRadius);
  YChange = XRadius * XRadius;
  EllipseError = 0;
  StoppingX = TwoBSquare * XRadius;
  StoppingY = 0;

  while (StoppingX >= StoppingY) { // 1st set of points, y'> -1
    Draw_4_Ellipse_Points(CX, CY, X, Y);
    Y++;
    StoppingY = StoppingY + TwoASquare;
    EllipseError = EllipseError + YChange;
    YChange = YChange + TwoASquare;
    if ((2 * EllipseError + XChange) > 0) {
      X--;
      StoppingX = StoppingX - TwoBSquare;
      EllipseError = EllipseError + XChange;
      XChange = XChange + TwoBSquare;
    }
  }

  X = 0;
  Y = YRadius;
  XChange = YRadius * YRadius;
  YChange = XRadius * XRadius * (1 - 2 * YRadius);
  EllipseError = 0;
  StoppingX = 0;
  StoppingY = TwoASquare * YRadius;

  while (StoppingX <= StoppingY) { // 2nd set of points, y'< -1
    Draw_4_Ellipse_Points(CX, CY, X, Y);
    X++;
    StoppingX = StoppingX + TwoBSquare;
    EllipseError = EllipseError + XChange;
    XChange = XChange + TwoBSquare;
    if ((2 * EllipseError + YChange) > 0) {
      Y--;
      StoppingY = StoppingY - TwoASquare;
      EllipseError = EllipseError + YChange;
      YChange = YChange + TwoASquare;
    }
  }
}

void ModLED8x8_drawCircle(int32_t x, int32_t y, int32_t r) {
  ModLED8x8_drawEllipse(x, y, r, r);
}

void ModLED8x8_drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3) {
  ModLED8x8_drawLine(x1, y1, x2, y2);
  ModLED8x8_drawLine(x2, y2, x3, y3);
  ModLED8x8_drawLine(x3, y3, x1, y1);
}

void ModLED8x8_drawChar(uint8_t c) {
  uint8_t b, i;
  int8_t k;
  if (c < 32 || c > 125)
    c = 32;

  for (k = 0; k < 5; k++) {
    b = ModLED8x8_FontLookup[c - 32][k];
    for (i = 0; i < 8; i++)
      if (b & (1 << i))
        ModLED8x8_drawPixel(k + cX, i + cY);
  }
}

void ModLED8x8_drawString(uint8_t c[]) {
  int32_t i;
  for (i = 0; c[i]; i++) {
    ModLED8x8_drawChar(c[i]);
    cX += 6;
  }
}

void ModLED8x8_leftScroll() {
  int32_t i;
  for (i = MODLED8X8_NUMBER_X * 24 - 1; i >= 0; i--) {
    videoBuffer[i] = videoBuffer[i] >> 1;
    if (i >= 24)
      videoBuffer[i] |= ((videoBuffer[i - 24] & 1) << 7);
  }
}

void ModLED8x8_rightScroll() {
  int32_t i;
  for (i = 0; i < MODLED8X8_NUMBER_X * 24; i++) {
    videoBuffer[i] = videoBuffer[i] << 1;
    if (i < (MODLED8X8_NUMBER_X - 1) * 24)
      videoBuffer[i] |= (videoBuffer[i + 24] >> 7);
  }
}

void ModLED8x8_scrollCharLeft(uint8_t c) {
  uint8_t b, i, k;
  if (c < 32 || c > 125)
    c = 32;

  for (k = 0; k < 5; k++) {
    b = ModLED8x8_FontLookup[c - 32][k];
    for (i = 0; i < 8; i++)
      if (b & (1 << i))
        ModLED8x8_drawPixel(MODLED8X8_BIG_X * 8, i + 1 + (MODLED8X8_BIG_Y - 1) * 8);
    Transfer();
    ModLED8x8_leftScroll();
    delayMs(scrollDelay);
  }
  Transfer();
  ModLED8x8_leftScroll();
  delayMs(scrollDelay);
}

void ModLED8x8_scrollCharRight(uint8_t c) {
  uint8_t b, i, k;
  if (c < 32 || c > 125)
    c = 32;

  ModLED8x8_rightScroll();
  for (k = 4; k >= 0; k--) {
    b = ModLED8x8_FontLookup[c - 32][k];
    for (i = 0; i < 8; i++)
      if (b & (1 << i))
        ModLED8x8_drawPixel(1, i + 1);
    Transfer();
    ModLED8x8_rightScroll();
    delayMs(scrollDelay);
  }
  Transfer();
  delayMs(scrollDelay);
}

void ModLED8x8_theEnder(ScrollDirection directions) {
  int32_t i;
  for (i = 0; i < MODLED8X8_NUMBER_X * 8 - 1; i++) {
    if (directions)
      ModLED8x8_rightScroll();
    else
      ModLED8x8_leftScroll();
    Transfer();
    delayMs(scrollDelay);
  }
}

void ModLED8x8_scrollString(uint8_t c[], ScrollDirection directions) {

  int32_t len, i;
  for (len = 0; c[len]; len++)
    ;

  if (directions == SCROLL_RIGHT) {
    for (i = len - 1; i >= 0; i--) {
      ModLED8x8_scrollCharRight(c[i]);
      color++;
      if (color > 7)
        color = 1;
    }
  } else {
    for (i = 0; c[i]; i++) {
      ModLED8x8_scrollCharLeft(c[i]);
      color++;
      if (color > 7)
        color = 1;
    }
  }
  ModLED8x8_theEnder(directions);
}
