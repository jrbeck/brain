#pragma once

#include <thread>

#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"
#include "../../engine/Complex.h"

#include "State.h"

#define MY_PI (3.14159265358979323846)
#define DEG2RAD (0.01745329251994329577)

#define NEURON_DRAW_RADIUS (2.0f)

namespace Mandelbrot {
  enum RenderThreadStates {
    dormant = 0,
    working = 1,
    complete = 2
  };

  struct RenderThread {
    bool active;
    int state;
    std::thread thread;
    int y;
    VEC2_DATA_TYPE j;
    VEC2_DATA_TYPE leftEdge;
    VEC2_DATA_TYPE rightEdge;
  };


  class Renderer {
  public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void render(State& state);

  private:
    void drawWhisker(const Vec2& worldCoords, uint iterations);
    void drawMandelbrot();
    void drawScanline(RenderThread* renderThread);
    void drawWhiskers();
    uint calculateIterations(const Vec2& worldCoords, uint maxIterations) const;
    RgbFloat calculateColor(uint iterations, uint maxIterations) const;

    int mWindowWidth, mWindowHeight;

    ImageBuffer* mImageBuffer;
    Painter* mPainter;

    State* mState;
  };
}
