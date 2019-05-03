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

  void Controller::drawFrame() {
    mRenderer.drawFrame(mState);
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

          mState.mMousePosition.print("asd");
          mState.windowToWorld(mState.mMousePosition).print();
          mState.worldToWindow(mState.windowToWorld(mState.mMousePosition)).print();

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
      // case SDLK_r:
      //   simReset();
      //   break;

      case SDLK_ESCAPE:
        quit = true;
        break;

      default:
        break;
    }

    return quit;
  }

  void Controller::handleMouseButton(int button, const Vec2& windowCoords) {
    button++;
    int x = windowCoords.x;
    x++;


    // // TODO: stop trying to be so clever ... just use screen coords and let the
    // // viewport handle the transform
    // Vec2 worldCoords = mState.windowToWorld(windowCoords);

    // // TODO: this is just here because the zoom that used to live in mState is no longer there
    // VEC2_DATA_TYPE zoom = mState.mViewport.getAntipixelScale().x;

    // if (mState.mProgramMode == MODE_SIMULATE) {
    // }
    // else if (mState.mProgramMode == MODE_PAINT_NEURON) {
    //   if (button == SDL_BUTTON_LEFT) {
    //     paintNeurons(worldCoords,
    //       mState.mPaintRadius * zoom,
    //       (int)(PAINT_DENSITY * (MY_PI * mState.mPaintRadius * mState.mPaintRadius)));
    //   }
    //   if (button == SDL_BUTTON_RIGHT) {
    //     removeSynapses();
    //     removeNeurons(worldCoords, mState.mPaintRadius * zoom);
    //   }
    // }
    // else if (mState.mProgramMode == MODE_PAINT_INPUT) {
    //   if (button == SDL_BUTTON_LEFT) {
    //     paintInputNeuron(worldCoords, mState.mPaintRadius * zoom);
    //   }
    //   if (button == SDL_BUTTON_RIGHT) {
    //     removeInputNeurons(worldCoords, mState.mPaintRadius * zoom);
    //   }
    // }
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
    Vec2 center = Vec2(-0.5, 0.5);
    Vec2 dimensions = Vec2(3.5, 3.5);
    mState.mViewport.setViewport(center, dimensions);
  }

  bool Controller::update() {
    return false;
  }
}
