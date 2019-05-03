#include "State.h"

namespace Mandelbrot {
  State::State(uint windowWidth, uint windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mMousePosition(0, 0),
    mViewport(windowWidth, windowHeight),
    mViewportChanged(true),
    mFractalBuffer(windowWidth, windowHeight),
    mFractalPainter(&mFractalBuffer)
  {
    reset();
  }

  State::~State() {
  }

  void State::reset() {
    // mMousePosition.x = 0;
    // mMousePosition.y = 0;
    mMouseMoved = false;

    mViewportChanged = true;

    mLastUpdateTicks = SDL_GetTicks();
  }

  Vec2 State::windowToWorld(const Vec2& windowCoords) const {
    return mViewport.windowToWorld(windowCoords);
  }

  Vec2 State::worldToWindow(const Vec2& worldCoords) const {
    return mViewport.worldToWindow(worldCoords);
  }
}
