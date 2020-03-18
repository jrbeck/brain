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

void Painter::setRgb(int x, int y, RgbFloat color) {
  if (!isOnScreen(x, y)) {
    return;
  }

  mImageBuffer->setRgb(x, y, color.r * 255, color.g * 255, color.b * 255);
}

void Painter::setRgb(const v2di_t& a, unsigned char red, unsigned char green, unsigned char blue) {
  if (!isOnScreen(a.x, a.y)) {
    return;
  }

  mImageBuffer->setRgb(a.x, a.y, red, green, blue);
}

void Painter::setRgb(const Vec2& a, unsigned char red, unsigned char green, unsigned char blue) {
  int x = (int)a.x;
  int y = (int)a.y;

  if (!isOnScreen(x, y)) {
    return;
  }

  mImageBuffer->setRgb(x, y, red, green, blue);
}

void Painter::drawLine(const Vec2& a, const Vec2& b, unsigned char red, unsigned char green, unsigned char blue) {
  int x0 = (int)a.x;
  int y0 = (int)a.y;
  int x1 = (int)b.x;
  int y1 = (int)b.y;

  clipLine(x0, y0, x1, y1, red, green, blue);

  // int dx =  abs(x1 - x0);
  // int dy = -abs(y1 - y0);
  // int sx = x0 < x1 ? 1 : -1;
  // int sy = y0 < y1 ? 1 : -1;
  // int err = dx + dy;
  // int e2;

  // for (;;) {
  //   if (isOnScreen(x0, y0)) {
  //     mImageBuffer->setRgb(x0, y0, red, green, blue);
  //   }

  //   if (x0 == x1 && y0 == y1) {
  //     break;
  //   }

  //   e2 = err << 1;

  //   if (e2 >= dy) {
  //     err += dy;
  //     x0 += sx;
  //   }
  //   if (e2 <= dx) {
  //     err += dx;
  //     y0 += sy;
  //   }
  // }
}

void Painter::drawOnScreenLine(int x0, int y0, int x1, int y1, unsigned char red, unsigned char green, unsigned char blue) {
  // int x0 = (int)a.x;
  // int y0 = (int)a.y;
  // int x1 = (int)b.x;
  // int y1 = (int)b.y;

  int dx =  abs(x1 - x0);
  int dy = -abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  int e2;

  for (;;) {
    mImageBuffer->setRgb(x0, y0, red, green, blue);

    if (x0 == x1 && y0 == y1) {
      break;
    }

    e2 = err << 1;

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


void Painter::drawLine(const Vec2& a, const Vec2& b, const RgbFloat& color) {
  drawLine(a, b, color.r * 255, color.g * 255, color.b * 255);
}

void Painter::drawRing(VEC2_DATA_TYPE radius, const Vec2& center, const RgbFloat& color) {
  char red = color.r * 255;
  char green = color.g * 255;
  char blue = color.b * 255;

  Vec2 a, b;
  b.x = center.x + radius;
  b.y = center.y;

  for (int i = 0; i <= 360; i += 10) {
    VEC2_DATA_TYPE radians = (VEC2_DATA_TYPE)i * DEG2RAD;
    a.x = center.x + (cos(radians) * radius);
    a.y = center.y + (sin(radians) * radius);

    drawLine(a, b, red, green, blue);
    b = a;
  }
}

void Painter::drawHex(VEC2_DATA_TYPE radius, const Vec2& center, const RgbFloat& color) {
  char red = color.r * 255;
  char green = color.g * 255;
  char blue = color.b * 255;

  Vec2 a, b;
  b.x = center.x + radius;
  b.y = center.y;

  for (int i = 10; i <= 360; i += 60) {
    VEC2_DATA_TYPE radians = (VEC2_DATA_TYPE)i * DEG2RAD;
    a.x = center.x + (cos(radians) * radius);
    a.y = center.y + (sin(radians) * radius);

    drawLine(a, b, red, green, blue);
    b = a;
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


// https://www.geeksforgeeks.org/liang-barsky-algorithm/
#define ROUND(a) ((int)(a + 0.5))

bool Painter::clipTest(VEC2_DATA_TYPE p, VEC2_DATA_TYPE q, VEC2_DATA_TYPE& t0, VEC2_DATA_TYPE& t1) {
  VEC2_DATA_TYPE r;
  int retVal = true;

  // line entry point
  if (p < 0.0) {
    r = q / p;

    // line portion is outside the clipping edge
    if (r > t1) {
      retVal = false;
    }
    else if (r > t0) {
      t0 = r;
    }
  }
  else
  //line leaving point
  if (p > 0.0) {
    r = q / p;

    // line portion is outside
    if (r < t0) {
      retVal = false;
    }

    else if (r < t1) {
      t1 = r;
    }
  }

  // p = 0, so line is parallel to this clipping edge
  else
  // Line is outside clipping edge
  if (q < 0.0) {
    retVal = false;
  }

  return retVal;
}

void Painter::clipLine(int x0, int y0, int x1, int y1, unsigned char red, unsigned char green, unsigned char blue) {
  Vec2 winMin = Vec2(0, 0);
  Vec2 winMax = Vec2(mWidth - 1, mHeight - 1);

  VEC2_DATA_TYPE t0 = 0.0, t1 = 1.0, dx = x1 - x0, dy;

  // inside test wrt left edge && inside test wrt right edge
  if (clipTest(-dx, x0 - winMin.x, t0, t1) && clipTest(dx, winMax.x - x0, t0, t1)) {
    dy = y1 - y0;

    // inside test wrt bottom edge && inside test wrt top edge
    if (clipTest(-dy, y0 - winMin.y, t0, t1) && clipTest(dy, winMax.y - y0, t0, t1)) {
      if (t1 < 1.0) {
        x1 = x0 + t1 * dx;
        y1 = y0 + t1 * dy;
      }

      if (t0 > 0.0) {
        x0 += t0 * dx;
        y0 += t0 * dy;
      }

      drawOnScreenLine(ROUND(x0), ROUND(y0), ROUND(x1), ROUND(y1), red, green, blue);
    }
  }
}
