#pragma once

#include "Vec2.h"


struct Rectangle {
  Vec2 dimensions;
  Vec2 center;
};



class Viewport {
public:
  Viewport(size_t windowWidth, size_t windowHeight);
  ~Viewport() {};

  void reset();
  void setViewport(const Vec2& center, const Vec2& dimensions);

  void print();

  void zoom(VEC2_DATA_TYPE scale, const Vec2& worldCoords);
  void translateByPixels(const Vec2& offset);

  Vec2 windowToWorld(const Vec2& windowCoords) const;
  Vec2 worldToWindow(const Vec2& worldCoords) const;

  const Vec2& getAntipixelScale() { return mAntiPixelScale; };

private:
  void recalculate();

  size_t mWindowWidth, mWindowHeight;
  Vec2 mWindowDimensions;

  Rectangle mViewRectangle;

  // these are calculated
  Vec2 mViewBottomLeft;
  Vec2 mPixelScale;
  Vec2 mAntiPixelScale;
};
