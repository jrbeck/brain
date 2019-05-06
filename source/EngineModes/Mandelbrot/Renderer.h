#pragma once

#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"
#include "../../engine/Complex.h"

#include "State.h"

#define MY_PI (3.14159265358979323846)
#define DEG2RAD (0.01745329251994329577)

#define NEURON_DRAW_RADIUS (2.0f)

namespace Mandelbrot {
  class Renderer {
  public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void render(State& state);

  private:
    void drawWhisker(const Vec2& worldCoords, uint iterations);
    void drawMandelbrot();
    uint calculateIterations(const Vec2& worldCoords, uint maxIterations) const;
    RgbFloat calculateColor(uint iterations, uint maxIterations) const;

    int mWindowWidth, mWindowHeight;

    ImageBuffer* mImageBuffer;
    Painter* mPainter;

    State* mState;
  };
}
