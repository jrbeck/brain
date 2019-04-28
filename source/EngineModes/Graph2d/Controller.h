#pragma once

// #include <cstdio>
// #include <cstdlib>
// #include <cmath>
#include <vector>

#include <SDL2/SDL.h>

#include "State.h"
#include "Renderer.h"

#define VIEW_DEFAULT_BOTTOM (-10)
#define VIEW_DEFAULT_LEFT (-10)
#define VIEW_DEFAULT_TOP (10)
#define VIEW_DEFAULT_RIGHT (10)

namespace Graph2d {
  class Controller {
  public:
    Controller(int windowWidth, int windowHeight);
    ~Controller();

    ImageBuffer* getOutputImageBuffer();
    void drawFrame();

    void update();
    bool handleInput();

  private:
    bool handleKeystroke();
    void handleMouseButton(int button, v2d_t pos);

    void reset();
    void viewReset();

    int mWindowWidth;
    int mWindowHeight;

    State* mState;
    Renderer* mRenderer;

    SDL_Event mSdlEvent;
  };
}
