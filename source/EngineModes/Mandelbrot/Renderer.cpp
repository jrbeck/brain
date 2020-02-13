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
      // drawWhiskers();
      mState->mViewportChanged = false;
    }

    mImageBuffer->copy(mState->mFractalBuffer);

    Vec2 worldCoords = mState->windowToWorld(mState->mMousePosition);
    drawWhisker(worldCoords, 500);

    mPainter->drawFormattedString(5, 5, "hi");

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

  // ********************************************
  // non-threaded version
  // ********************************************
  // void Renderer::drawMandelbrot() {
  //   uint maxIterations = 1024;

  //   Vec2 windowCoords;
  //   for (int y = 0; y < mWindowHeight; ++y) {
  //     windowCoords.y = y;
  //     for (int x = 0; x < mWindowWidth; ++x) {
  //       windowCoords.x = x;
  //       Vec2 worldCoords = mState->mViewport.windowToWorld(windowCoords);

  //       uint iterations = calculateIterations(worldCoords, maxIterations);

  //       RgbFloat color = calculateColor(iterations, maxIterations);
  //       mState->mFractalPainter.setRgb(x, y, color);
  //     }
  //   }
  // }

  // ********************************************
  // threaded version
  // ********************************************
  void Renderer::drawMandelbrot() {
    // uint maxIterations = 1024;
    const int kNumThreads = 8;
    Vec2 worldCoordsTL = mState->mViewport.windowToWorld(Vec2(0, 0));
    Vec2 worldCoordsBR = mState->mViewport.windowToWorld(Vec2(mWindowWidth - 1, mWindowHeight - 1));
    RenderThread renderThreads[kNumThreads];

    for (int i = 0; i < kNumThreads; ++i) {
      renderThreads[i].state = RenderThreadStates::dormant;
      renderThreads[i].leftEdge = worldCoordsTL.x;
      renderThreads[i].rightEdge = worldCoordsBR.x;
    }

    for (int y = 0; y < mWindowHeight;) {
      for (unsigned t = 0; (t < kNumThreads) && (y < mWindowHeight); ++t) {
        if (renderThreads[t].state == RenderThreadStates::working) {
          continue;
        }

        if (renderThreads[t].state == RenderThreadStates::complete) {
          renderThreads[t].thread.join();
        }

        renderThreads[t].state = RenderThreadStates::working;
        renderThreads[t].y = y;
        VEC2_DATA_TYPE percent = (VEC2_DATA_TYPE)y / (VEC2_DATA_TYPE)(mWindowHeight - 1);
        renderThreads[t].j = worldCoordsTL.y + ((worldCoordsBR.y - worldCoordsTL.y) * percent);
        renderThreads[t].thread = std::thread(&Renderer::drawScanline, this, &renderThreads[t]);

        ++y;
      }

      SDL_Delay(1);
    }

    for (unsigned t = 0; t < kNumThreads; ++t) {
      if (renderThreads[t].state == RenderThreadStates::working || renderThreads[t].state == RenderThreadStates::complete) {
        renderThreads[t].thread.join();
        renderThreads[t].state = RenderThreadStates::dormant;
      }
    }
  }

  void Renderer::drawScanline(RenderThread* renderThread) {
    int maxIterations = 1024;
    Vec2 worldCoords;
    worldCoords.y = renderThread->j;
    for (int x = 0; x < mWindowWidth; ++x) {
      VEC2_DATA_TYPE percent = (VEC2_DATA_TYPE)x / (VEC2_DATA_TYPE)(mWindowWidth - 1);
      worldCoords.x = renderThread->leftEdge + ((renderThread->rightEdge - renderThread->leftEdge) * percent);

      uint iterations = calculateIterations(worldCoords, maxIterations);

      RgbFloat color = calculateColor(iterations, maxIterations);
      mState->mFractalPainter.setRgb(x, renderThread->y, color);
    }

    renderThread->state = RenderThreadStates::complete;
  }

  // void Renderer::drawWhiskers() {
  //   Vec2 windowCoords;
  //   for (int y = 0; y < mWindowHeight; ++y) {
  //     windowCoords.y = y;
  //     for (int x = 0; x < mWindowWidth; ++x) {
  //       windowCoords.x = x;
  //       Vec2 worldCoords = mState->mViewport.windowToWorld(windowCoords);

  //       uint iterations = calculateIterations(worldCoords, maxIterations);

  //       if (iterations == maxIterations && (y % 100 == 0) && (x % 100 == 0)) {
  //         Complex x, lastX(worldCoords);
  //         Complex first(worldCoords);

  //         RgbFloat color(1.0, 0.0, 0.0);

  //         for (uint i = 0; i < 3; ++i) {
  //           x = (lastX * lastX) + first;
  //           mState->mFractalPainter.drawLine(mState->worldToWindow(x.mValue), mState->worldToWindow(lastX.mValue), color);
  //           lastX = x;
  //         }
  //       }
  //     }
  //   }
  // }

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
    RgbFloat offset(0.2, 2.3, 1.33);
    float t = 2.0 * MY_PI * (float)iterations / (float)maxIterations;
    return RgbFloat(
      0.5 + (0.5 * sin(t * scale.r + offset.r)),
      0.5 + (0.5 * sin(t * scale.g + offset.g)),
      0.5 + (0.5 * sin(t * scale.b + offset.b))
    );
  }
}
