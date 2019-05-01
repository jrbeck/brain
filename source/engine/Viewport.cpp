#include "Viewport.h"


Viewport::Viewport(size_t windowWidth, size_t windowHeight) :
  mWindowWidth(windowWidth),
  mWindowHeight(windowHeight),
  mWindowDimensions(windowWidth, windowHeight)
{
  reset();
}

void Viewport::reset() {
  Vec2 dimensions = Vec2(1.0, mWindowDimensions.ratio());
  Vec2 center = Vec2(0.0, 0.0);
  setViewport(dimensions, center);
}

void Viewport::setViewport(const Vec2& dimensions, const Vec2& center) {
  mViewRectangle.dimensions = dimensions;
  mViewRectangle.center = center;
  recalculate();
}

void Viewport::zoom(VEC2_DATA_TYPE scale, const Vec2& worldCoords) {
  Vec2 oldBLtoZoomFocus = mViewBottomLeft - worldCoords;

  mViewRectangle.dimensions *= scale;
  recalculate();

  Vec2 desired = oldBLtoZoomFocus * scale;
  Vec2 desiredOffser = mViewBottomLeft - (worldCoords + desired);
  mViewRectangle.center += desiredOffser;
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
  Vec2 windowCoords = (worldCoords + mViewRectangle.center).hadamard(mPixelScale);
  windowCoords += mWindowDimensions * 0.5;
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
