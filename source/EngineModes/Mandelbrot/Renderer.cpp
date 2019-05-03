#include "Renderer.h"

namespace Mandelbrot {
  Renderer::Renderer(int windowWidth, int windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mImageBuffer(nullptr),
    mPainter(nullptr)
  {
    mImageBuffer = new ImageBuffer();
    mImageBuffer->resize(windowWidth, windowHeight);
    mPainter = new Painter(mImageBuffer);
  }

  Renderer::~Renderer() {
    if (mImageBuffer != nullptr) {
      delete mImageBuffer;
    }
  }

  ImageBuffer* Renderer::getImageBuffer() {
    return mImageBuffer;
  }

  void Renderer::drawFrame(State& state) {
    mState = &state;

    mPainter->clear(0, 0, 0);

    if (mState->mViewportChanged) {
      drawMandelbrot();
      mState->mViewportChanged = false;
    }

    mImageBuffer->copy(mState->mFractalBuffer);

    Vec2 worldCoords = mState->windowToWorld(mState->mMousePosition);
    traceMandelbrotFunction(worldCoords, 50);
  }

  void Renderer::traceMandelbrotFunction(const Vec2& worldCoords, uint iterations) {
    Complex x, lastX(worldCoords);
    Complex first(worldCoords);

    RgbFloat color(1.0, 0.0, 0.0);

    for (uint i = 0; i < iterations; ++i) {
      x = (lastX * lastX) + first;
      mPainter->drawLine(mState->worldToWindow(x.mValue), mState->worldToWindow(lastX.mValue), color);
      lastX = x;
    }
  }

  void Renderer::drawMandelbrot() {
    uint maxIterations = 15;

    Vec2 windowCoords;
    for (int y = 0; y < mWindowHeight; ++y) {
      windowCoords.y = y;
      for (int x = 0; x < mWindowWidth; ++x) {
        windowCoords.x = x;
        Vec2 worldCoords = mState->mViewport.windowToWorld(windowCoords);

        uint iterations = calculateIterations(worldCoords, maxIterations);

        RgbFloat color = calculateColor(iterations, maxIterations);
        mState->mFractalPainter.setRgb(x, y, color);
      }
    }

    // this draws the 'whiskers' in the mandelbrot set
    // for (int y = 0; y < mWindowHeight; ++y) {
    //   windowCoords.y = y;
    //   for (int x = 0; x < mWindowWidth; ++x) {
    //     windowCoords.x = x;
    //     Vec2 worldCoords = mState->mViewport.windowToWorld(windowCoords);

    //     uint iterations = calculateIterations(worldCoords, maxIterations);

    //     if (iterations == maxIterations && (y % 100 == 0) && (x % 100 == 0)) {
    //       Complex x, lastX(worldCoords);
    //       Complex first(worldCoords);

    //       RgbFloat color(1.0, 0.0, 0.0);

    //       for (uint i = 0; i < 3; ++i) {
    //         x = (lastX * lastX) + first;
    //         mState->mFractalPainter.drawLine(mState->worldToWindow(x.mValue), mState->worldToWindow(lastX.mValue), color);
    //         lastX = x;
    //       }
    //     }
    //   }
    // }
  }

  uint Renderer::calculateIterations(const Vec2& worldCoords, uint maxIterations) const {
    uint i = 0;

    Complex x, lastX(worldCoords);
    Complex first(worldCoords);

    for (; i < maxIterations; ++i) {
      x = (lastX * lastX) + first;

      if (x.length() > 2) {
        return i;
      }

      lastX = x;
    }

    return i;
  }

  RgbFloat Renderer::calculateColor(uint iterations, uint maxIterations) const {
    if (iterations == maxIterations) {
      return RgbFloat(0, 0, 0);
    }

    float value = (64.0f + (((float)iterations * (255.0f - 64.0f) / (float)maxIterations))) / 255.0f;
    return RgbFloat(value, value, value);
  }
}
