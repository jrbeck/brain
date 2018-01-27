#include "Painter.h"

Painter::Painter(ImageBuffer* imageBuffer) :
  mImageBuffer(imageBuffer),
  mBasicText(nullptr),
  mWidth(0),
  mHeight(0)
{
  mBasicText = new BasicText(this);

  mWidth = mImageBuffer->getWidth();
  mHeight = mImageBuffer->getHeight();
}

Painter::~Painter() {
  if (mBasicText != nullptr) {
    delete mBasicText;
  }
}

void Painter::clear(unsigned char red, unsigned char green, unsigned char blue) {
  mImageBuffer->clear(red, green, blue);
}

bool Painter::isOnScreen(int x, int y) const {
  if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) return false;
  return true;
}

void Painter::setRgb(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
  if (!isOnScreen(x, y)) {
    return;
  }

  mImageBuffer->setRgb(x, y, red, green, blue);
}

void Painter::setRgb(const v2di_t& a, unsigned char red, unsigned char green, unsigned char blue) {
  if (!isOnScreen(a.x, a.y)) {
    return;
  }

  mImageBuffer->setRgb(a.x, a.y, red, green, blue);
}

void Painter::setRgb(const v2d_t& a, unsigned char red, unsigned char green, unsigned char blue) {
  int x = (int)a.x;
  int y = (int)a.y;

  if (!isOnScreen(x, y)) {
    return;
  }

  mImageBuffer->setRgb(x, y, red, green, blue);
}

void Painter::drawLine(const v2d_t& a, const v2d_t& b, unsigned char red, unsigned char green, unsigned char blue) {
  int x0 = (int)a.x;
  int y0 = (int)a.y;
  int x1 = (int)b.x;
  int y1 = (int)b.y;

  // if (x0 < 0 || x0 >= mWidth) return;
  // if (y0 < 0 || y0 >= mHeight) return;
  // if (x1 < 0 || x1 >= mWidth) return;
  // if (y1 < 0 || y1 >= mHeight) return;

  int dx =  abs(x1 - x0);
  int dy = -abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for (;;) {
    if (isOnScreen(x0, y0)) {
      mImageBuffer->setRgb(x0, y0, red, green, blue);
    }

    if (x0 == x1 && y0 == y1) {
      break;
    }

    e2 = 2 * err;

    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void Painter::drawString(int x, int y, const char* text) {
  int initialX = x;

  for (int i = 0; text[i] != '\0'; ++i) {
    if (text[i] == '\n') {
      x = initialX;
      y += CHAR_HEIGHT;
    }
    else {
      mBasicText->drawChar(x, y, text[i]);
      x += CHAR_WIDTH;
    }
  }
}

void Painter::drawFormattedString(int x, int y, const char* format, ...) {
  va_list argList;
  va_start(argList, format);
  vsnprintf(mTextBuffer, DRAWSTRINGF_MAX_LENGTH, format, argList);
  va_end(argList);
  drawString(x, y, mTextBuffer);
}
