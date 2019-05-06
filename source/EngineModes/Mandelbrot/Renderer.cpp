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

  void Renderer::render(State& state) {
    mState = &state;

    // mPainter->clear(0, 0, 0);

    if (mState->mViewportChanged) {
      drawMandelbrot();
      mState->mViewportChanged = false;
    }

    mImageBuffer->copy(mState->mFractalBuffer);

    // Vec2 worldCoords = mState->windowToWorld(mState->mMousePosition);
    // drawWhisker(worldCoords, 500);

    // for (Vec2 whiskerPosition : mState->mWhiskers) {
    //   drawWhisker(whiskerPosition, 500);
    // }
  }

  void Renderer::drawWhisker(const Vec2& worldCoords, uint iterations) {
    Complex x(worldCoords), lastX;
    Complex first(worldCoords);

    RgbFloat color(1.0, 0.0, 0.0);

    for (uint i = 0; i < iterations; ++i) {
      // x = (lastX * lastX) + first;
      x.squareAndAdd(first);

      color = calculateColor(i, iterations);
      mPainter->drawLine(mState->worldToWindow(x.mValue), mState->worldToWindow(lastX.mValue), color);
      lastX = x;
    }
  }

  void Renderer::drawMandelbrot() {
    uint maxIterations = 1024;

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
    Complex x(worldCoords), lastX;
    Complex first(worldCoords);

    for (; i < maxIterations; ++i) {
      x.squareAndAdd(first);
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

    // grey
    // float value = (64.0f + (((float)iterations * (255.0f - 64.0f) / (float)maxIterations))) / 255.0f;
    // return RgbFloat(value, value, value);

    RgbFloat scale(0.412, 1.15, 0.666);
    RgbFloat offset(0.6, 0.3, 1.33);
    float t = 2.0 * MY_PI * (float)iterations / (float)maxIterations;
    return RgbFloat(
      0.5 + (0.5 * sin(t * scale.r + offset.r)),
      0.5 + (0.5 * sin(t * scale.g + offset.g)),
      0.5 + (0.5 * sin(t * scale.b + offset.b))
    );
  }
}
