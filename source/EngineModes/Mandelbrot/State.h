#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "../../engine/Vec2.h"
#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"
#include "../../engine/Viewport.h"

#include "../../PriorityQueue.h"

#define INITIAL_INPUT_LEVEL (10.0f)

namespace Mandelbrot {
  class State {
  public:
    State() = delete;
    State(uint windowWidth, uint windowHeight);
    ~State();

    void reset();

    Vec2 windowToWorld(const Vec2& windowCoords) const;
    Vec2 worldToWindow(const Vec2& worldCoords) const;

    uint mWindowWidth;
    uint mWindowHeight;

    unsigned long mLastUpdateTicks;

    Vec2 mMousePosition;
    bool mMouseMoved;

    Viewport mViewport;
    bool mViewportChanged;

    ImageBuffer mFractalBuffer;
    Painter mFractalPainter;

    std::vector<Vec2> mWhiskers;
  };
}
