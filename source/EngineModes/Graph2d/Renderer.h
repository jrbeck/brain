#pragma once

#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"

#include "State.h"

#define MY_PI (3.14159265358979323846)
#define DEG2RAD (0.01745329251994329577)

#define NEURON_DRAW_RADIUS (2.0f)

namespace Graph2d {
  class Renderer {
  public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void drawFrame(State* state);

  private:
    void drawNeurons();

    void drawLine(v2d_t a, v2d_t b, RgbFloat color);
    void drawRing(float radius, v2d_t center, RgbFloat color);
    void drawHex(float radius, v2d_t center, RgbFloat color);

    int mWindowWidth, mWindowHeight;

    ImageBuffer* mImageBuffer;
    Painter* mPainter;

    State* mState;
  };
}
