#include "Viewport.h"


Viewport::Viewport(size_t windowWidth, size_t windowHeight) :
  mWindowWidth(windowWidth),
  mWindowHeight(windowHeight),
  mWindowDimensions(windowWidth, windowHeight)
{
  reset();
}

void Viewport::reset() {
  Vec2 center = Vec2(0.0, 0.0);
  Vec2 dimensions = Vec2(1.0, 1.0 / mWindowDimensions.ratio());
  setViewport(center, dimensions);
}

void Viewport::setViewport(const Vec2& center, const Vec2& dimensions) {
  mViewRectangle.center = center;
  mViewRectangle.dimensions = dimensions;
  recalculate();
}

void Viewport::zoom(VEC2_DATA_TYPE scale, const Vec2& worldCoords) {
  Vec2 diff = mViewRectangle.center - worldCoords;

  mViewRectangle.dimensions *= scale;
  mViewRectangle.center = worldCoords + (diff * scale);

  recalculate();
}

void Viewport::translateByPixels(const Vec2& offset) {
  mViewRectangle.center += (offset.hadamard(mAntiPixelScale));
  recalculate();
}

Vec2 Viewport::windowToWorld(const Vec2& windowCoords) const {
  Vec2 yFlipped = Vec2(windowCoords.x, mWindowHeight - windowCoords.y);
  return (yFlipped.hadamard(mAntiPixelScale)) + mViewBottomLeft;
}

Vec2 Viewport::worldToWindow(const Vec2& worldCoords) const {
  Vec2 windowCoords = (worldCoords - mViewRectangle.center + (mViewRectangle.dimensions * 0.5)).hadamard(mPixelScale);
  // windowCoords += mWindowDimensions * 0.5;
  // windowCoords -= (mViewBottomLeft.hadamard(mPixelScale));
  windowCoords.y = mWindowDimensions.y - windowCoords.y;

  return windowCoords;
}

void Viewport::recalculate() {
  mViewBottomLeft = mViewRectangle.center - (mViewRectangle.dimensions * 0.5);

  mPixelScale.x = mWindowDimensions.x / mViewRectangle.dimensions.x;
  mPixelScale.y = mWindowDimensions.y / mViewRectangle.dimensions.y;

  mAntiPixelScale.x = mViewRectangle.dimensions.x / mWindowDimensions.x;
  mAntiPixelScale.y = mViewRectangle.dimensions.y / mWindowDimensions.y;
}
