#pragma once

#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"

#include "State.h"

#define MY_PI (3.14159265358979323846)
#define DEG2RAD (0.01745329251994329577)

#define NEURON_DRAW_RADIUS (2.0f)

namespace Brain {
  class Renderer {
  public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void drawFrame(State& state);

  private:
    void drawNeurons();

    void drawLine(Vec2 a, Vec2 b, RgbFloat color);
    void drawRing(float radius, Vec2 center, RgbFloat color);
    void drawHex(float radius, Vec2 center, RgbFloat color);

    int mWindowWidth, mWindowHeight;

    ImageBuffer* mImageBuffer;
    Painter* mPainter;

    State* mState;
  };
}
