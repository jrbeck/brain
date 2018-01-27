#pragma once

#define CHAR_WIDTH (8)
#define CHAR_HEIGHT (8)

#define FIRST_CHAR (' ')
#define LAST_CHAR ('~')

extern unsigned long gBasicFontBitmaps[][2];

class Painter;

class BasicText {
public:
  BasicText(Painter* painter);
  ~BasicText();

  void drawChar(int x, int y, char character);

private:
  Painter* mPainter;
};
