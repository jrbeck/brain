#include "Controller.h"

namespace Graph2d {
  Controller::Controller(int windowWidth, int windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mState(new State()),
    mRenderer(new Renderer(windowWidth, windowHeight))
  {
    reset();
    viewReset();
  }

  Controller::~Controller() {
    if (mRenderer != nullptr) {
      delete mRenderer;
    }
    if (mState != nullptr) {
      delete mState;
    }
  }

  ImageBuffer* Controller::getOutputImageBuffer() {
    return mRenderer->getImageBuffer();
  }

  void Controller::drawFrame() {
    mRenderer->drawFrame(mState);
  }

  bool Controller::handleInput() {
    bool quit = 0;
    v2d_t mouseDelta;
    // bool mouseDrag = false;

    while (SDL_PollEvent(&mSdlEvent) && !quit) {
      switch (mSdlEvent.type) {
        case SDL_KEYDOWN:
          quit = handleKeystroke();
          break;

        case SDL_MOUSEMOTION:
          mouseDelta.x = mState->mMousePosition.x - mSdlEvent.motion.x;
          mouseDelta.y = mState->mMousePosition.y - mSdlEvent.motion.y;

          mState->mMousePosition.x = mSdlEvent.motion.x;
          mState->mMousePosition.y = mSdlEvent.motion.y;

          mState->mMouseMoved = true;
          break;

        case SDL_MOUSEBUTTONDOWN:
          handleMouseButton(mSdlEvent.button.button, v2d_v(mSdlEvent.button.x, mSdlEvent.button.y));
          break;

        case SDL_MOUSEBUTTONUP:
          if (mSdlEvent.button.button == SDL_BUTTON_LEFT) {
            // mouseDrag = false;
          }
         // handleMouseButton(mSdlEvent.button.button, mState->windowToWorld(v2d_v(mSdlEvent.button.x, mSdlEvent.button.y)));

          break;
      }
    }

    // handle mouse drag
    // FIXME: not here please!
    Uint8 mouseState = SDL_GetMouseState(NULL, NULL);

    if (mState->mProgramMode == MODE_SIMULATE) {
      if (mState->mMouseMoved && (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
        v2d_t scaledMouseDelta = v2d_scale(mouseDelta, mState->mViewZoom);
        mState->mViewBottomLeft = v2d_add(mState->mViewBottomLeft, scaledMouseDelta);
        mState->mViewTopRight = v2d_add(mState->mViewTopRight, scaledMouseDelta);
        mState->mMouseMoved = false;
      }
    }

    return quit;
  }

  bool Controller::handleKeystroke() {
    bool quit = false;

    switch (mSdlEvent.key.keysym.sym) {
      // case SDLK_s:
      //   growSynapses();
      //   flushNeurons();
      //   break;

      // case SDLK_r:
      //   reset();
      //   break;

      // case SDLK_m:
      //   mState->mProgramMode = (mState->mProgramMode + 1) % NUM_PROGRAM_MODES;
      //   reset();
      //   break;

      case SDLK_ESCAPE:
        quit = true;
        break;

      default:
        break;
    }

    return quit;
  }

  void Controller::handleMouseButton(int button, v2d_t windowCoords) {
    v2d_t worldCoords = mState->windowToWorld(windowCoords);

    if (button == SDL_BUTTON_LEFT) {
      // paintInputNeuron(worldCoords, mState->mPaintRadius * mState->mViewZoom);
    }
    if (button == SDL_BUTTON_RIGHT) {
      // removeInputNeurons(worldCoords, mState->mPaintRadius * mState->mViewZoom);
    }

    // if (button == SDL_BUTTON_WHEELDOWN && mState->mPaintRadius > 7) {
    //   mState->mPaintRadius -= 6;
    // }
    //
    // else if (button == SDL_BUTTON_WHEELUP) {
    //   mState->mPaintRadius += 6;
    // }
  }

  void Controller::reset() {
    mState->reset();
  }

  void Controller::viewReset() {
    mState->mViewBottomLeft.x = VIEW_DEFAULT_LEFT;
    mState->mViewBottomLeft.y = VIEW_DEFAULT_BOTTOM;

    mState->mViewTopRight.x = VIEW_DEFAULT_RIGHT;
    mState->mViewTopRight.y = VIEW_DEFAULT_TOP;

    mState->mViewZoom = (mState->mViewTopRight.x - mState->mViewBottomLeft.x) / (double)mWindowWidth;
  }

  void Controller::update() {
    // unsigned long now = SDL_GetTicks();

    // double delta_time = now - mState->mLastUpdateTicks;
    // if (delta_time < MIN_TIME_STEP) {
    //   return;
    // }
    // mState->mLastUpdateTicks = now;
  }
}
