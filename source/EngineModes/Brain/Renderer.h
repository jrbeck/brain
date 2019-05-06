#pragma once

#include "../../engine/ImageBuffer.h"
#include "../../engine/Painter.h"

#include "State.h"

#define NEURON_DRAW_RADIUS (2.0f)

namespace Brain {
  class Renderer {
  public:
    Renderer(int windowWidth, int windowHeight);
    ~Renderer();

    ImageBuffer* getImageBuffer();
    void render(State& state);

  private:
    void drawNeurons();

    int mWindowWidth, mWindowHeight;

    ImageBuffer* mImageBuffer;
    Painter* mPainter;

    State* mState;
  };
}
