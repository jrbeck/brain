#pragma once

// #include <cstdio>
// #include <cstdlib>
// #include <cmath>
#include <vector>

#include <SDL2/SDL.h>

#include "../../engine/EngineModeController.h"

#include "State.h"
#include "Renderer.h"


namespace Mandelbrot {
  class Controller : public EngineModeController {
  public:
    Controller(int windowWidth, int windowHeight);
    ~Controller();

    ImageBuffer* getOutputImageBuffer();
    void render();

    bool update();
    bool handleInput();

  private:
    bool handleKeystroke();
    void handleMouseButton(int button, const Vec2& windowCoords);
    void handleMouseWheelEvent(const SDL_MouseWheelEvent& wheelEvent);

    void viewReset();
    void printInfo();

    int mWindowWidth;
    int mWindowHeight;

    State mState;
    Renderer mRenderer;

    SDL_Event mSdlEvent;
  };
}
