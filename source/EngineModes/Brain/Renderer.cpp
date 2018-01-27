#include "Renderer.h"

namespace Brain {
  Renderer::Renderer(int windowWidth, int windowHeight) :
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

  void Renderer::drawFrame(State* state) {
    mState = state;

    mPainter->clear(0, 0, 0);

    drawNeurons();

    // if (gProgramMode == MODE_SIMULATE) {
    //   draw_mouse(gMousePosition, 22, 32);
    // }
    // else if (gProgramMode == MODE_PAINT_NEURON || gProgramMode == MODE_PAINT_INPUT) {
    //   draw_ring((float)paint_radius, v2d_vector(gMousePosition.x, SCREEN_HEIGHT - gMousePosition.y), color[gProgramMode]);
    // }
  }


  void Renderer::drawNeurons() {
    int num_flashes = 0;

    // project to window coords
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      mState->mNeurons[i].soma_d = mState->worldToWindow(mState->mNeurons[i].soma);
      mState->mNeurons[i].axon_d = mState->worldToWindow(mState->mNeurons[i].axon);
    }

    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      v2d_t soma = mState->mNeurons[i].soma_d;
      v2d_t axon = mState->mNeurons[i].axon_d;

      // draw soma to axon line
      drawLine(soma, axon, mState->mColors[COLOR_AXON_LINE]);

      // draw synapses
      for (int j = 0; j < (int)mState->mNeurons[i].synapses.size(); ++j) {
        if (mState->mNeurons[i].flash_time > mState->mSimTime) {
          drawLine(soma, axon, mState->mColors[COLOR_AXON_LINE_FIRE]);
          drawLine(axon, mState->mNeurons[mState->mNeurons[i].synapses[j].target].soma_d, mState->mColors[COLOR_SYNAPSE_FIRE]);
        }
        else {
          drawLine(soma, axon, mState->mColors[COLOR_AXON_LINE]);
          drawLine(axon, mState->mNeurons[mState->mNeurons[i].synapses[j].target].soma_d, mState->mColors[COLOR_SYNAPSE]);
        }
      }
    }

    // draw axon and soma non flashers
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      if (mState->mNeurons[i].flash_time <= mState->mSimTime) {
        drawHex(NEURON_DRAW_RADIUS +(8.0f *(float)(mState->mNeurons[i].charge / mState->mNeurons[i].threshold)), mState->mNeurons[i].soma_d, mState->mColors[COLOR_SOMA]);
        drawRing(NEURON_DRAW_RADIUS, mState->mNeurons[i].axon_d, mState->mColors[COLOR_AXON]);
      }
    }

    // draw axon and soma flashers
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      if (mState->mNeurons[i].flash_time > mState->mSimTime) {
        drawHex(NEURON_DRAW_RADIUS + 4.0f, mState->mNeurons[i].soma_d, mState->mColors[COLOR_SOMA_FIRE]);
        drawHex(NEURON_DRAW_RADIUS + 3.0f, mState->mNeurons[i].axon_d, mState->mColors[COLOR_AXON_FIRE]);
        num_flashes++;
      }
    }

    // identify the input neurons
    for (int i = 0; i < (int)mState->mInputNeurons.size(); ++i) {
      drawRing(8.0f, mState->mNeurons[mState->mInputNeurons[i]].soma_d, mState->mColors[COLOR_INPUT]);
    }

    // draw the flashing neuron ratio
    // v2d_t tl, br;
    //
    // tl.x = 100;
    // tl.y = 40;
    //
    // br.x = 400;
    // br.y = 20;
    //
    // draw_progress_bar(tl, br, (double)num_flashes /(double)mState->mNeurons.size(), mState->mColors[COLOR_PROG1], mState->mColors[COLOR_PROG2]);
  }


  void Renderer::drawLine(v2d_t a, v2d_t b, RgbFloat color) {
    mPainter->drawLine(a, b, color.r * 255, color.g * 255, color.b * 255);
  }

  void Renderer::drawRing(float radius, v2d_t center, RgbFloat color) {
    char red = color.r * 255;
    char green = color.g * 255;
    char blue = color.b * 255;

    v2d_t a, b;
    a.x = center.x + radius;
    a.y = center.y;

    for (int i = 0; i <= 360; i += 10) {
      double radians = (double)i * DEG2RAD;
      a.x = center.x + (cos(radians) * radius);
      a.y = center.y + (sin(radians) * radius);

      mPainter->drawLine(a, b, red, green, blue);
      b = a;
    }
  }

  void Renderer::drawHex(float radius, v2d_t center, RgbFloat color) {
    char red = color.r * 255;
    char green = color.g * 255;
    char blue = color.b * 255;

    v2d_t a, b;
    b.x = center.x + radius;
    b.y = center.y;

    for (int i = 10; i <= 360; i += 60) {
      double radians = (double)i * DEG2RAD;
      a.x = center.x + (cos(radians) * radius);
      a.y = center.y + (sin(radians) * radius);

      mPainter->drawLine(a, b, red, green, blue);
      b = a;
    }
  }
}
