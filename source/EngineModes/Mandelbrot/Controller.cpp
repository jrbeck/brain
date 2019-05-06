#include "Controller.h"

namespace Mandelbrot {
  Controller::Controller(int windowWidth, int windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mState(windowWidth, windowHeight),
    mRenderer(windowWidth, windowHeight)
  {
    viewReset();
  }

  Controller::~Controller() {
  }

  ImageBuffer* Controller::getOutputImageBuffer() {
    return mRenderer.getImageBuffer();
  }

  void Controller::render() {
    mRenderer.render(mState);
  }

  bool Controller::handleInput() {
    bool quit = 0;
    Vec2 mouseDelta;
    // bool mouseDrag = false;

    while (SDL_PollEvent(&mSdlEvent) && !quit) {
      switch (mSdlEvent.type) {
        case SDL_KEYDOWN:
          quit = handleKeystroke();
          break;

        case SDL_MOUSEMOTION:
          mouseDelta.x = mSdlEvent.motion.x - mState.mMousePosition.x;
          mouseDelta.y = -(mSdlEvent.motion.y - mState.mMousePosition.y);

          mState.mMousePosition.x = mSdlEvent.motion.x;
          mState.mMousePosition.y = mSdlEvent.motion.y;

          // mState.mMousePosition.print("window");
          // mState.windowToWorld(mState.mMousePosition).print();
          // mState.worldToWindow(mState.windowToWorld(mState.mMousePosition)).print();

          mState.mMouseMoved = true;
          break;

        case SDL_MOUSEBUTTONDOWN:
          handleMouseButton(mSdlEvent.button.button, Vec2(mSdlEvent.button.x, mSdlEvent.button.y));
          break;

        case SDL_MOUSEBUTTONUP:
          if (mSdlEvent.button.button == SDL_BUTTON_LEFT) {
            // mouseDrag = false;
          }
         // handleMouseButton(mSdlEvent.button.button, mState.windowToWorld(Vec2(mSdlEvent.button.x, mSdlEvent.button.y)));

          break;

        case SDL_MOUSEWHEEL:
          handleMouseWheelEvent(mSdlEvent.wheel);
      }

      // handle mouse drag
      // FIXME: not here please!
      Uint8 mouseState = SDL_GetMouseState(NULL, NULL);

      if (mState.mMouseMoved && (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        mState.mViewport.translateByPixels(-mouseDelta);
        mState.mMouseMoved = false;
        mState.mViewportChanged = true;
      }
    }

    return quit;
  }

  bool Controller::handleKeystroke() {
    bool quit = false;

    switch (mSdlEvent.key.keysym.sym) {
      case SDLK_c:
        mState.mWhiskers.clear();
        break;

      case SDLK_r:
        viewReset();
        break;

      case SDLK_ESCAPE:
        quit = true;
        break;

      default:
        break;
    }

    return quit;
  }

  void Controller::handleMouseButton(int button, const Vec2& windowCoords) {
    Vec2 worldCoords = mState.windowToWorld(windowCoords);

    // if (button == SDL_BUTTON_LEFT) {
    // }
    if (button == SDL_BUTTON_RIGHT) {
      mState.mWhiskers.push_back(worldCoords);
    }
  }

  void Controller::handleMouseWheelEvent(const SDL_MouseWheelEvent& wheelEvent) {
    Vec2 worldPosition = mState.windowToWorld(mState.mMousePosition);
    if (wheelEvent.y > 0) {
      mState.mViewport.zoom(0.8333, worldPosition);
      mState.mViewportChanged = true;
    }
    else if (wheelEvent.y < 0) {
      mState.mViewport.zoom(1.2, worldPosition);
      mState.mViewportChanged = true;
    }
  }

  void Controller::viewReset() {
    Vec2 center = Vec2(-0.5, 0.0);
    Vec2 dimensions = Vec2(3.5, 3.5 * ((float)mWindowHeight / (float)mWindowWidth));
    mState.mViewport.setViewport(center, dimensions);
    mState.mViewportChanged = true;
  }

  bool Controller::update() {
    return false;
  }
}
