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

  void Renderer::drawFrame(State& state) {
    mState = &state;

    mPainter->clear(0, 0, 0);

    drawNeurons();

    if (mState->mProgramMode == MODE_SIMULATE) {
      // draw_mouse(mState->mMousePosition, 22, 32);
    }
    else if (mState->mProgramMode == MODE_PAINT_NEURON || mState->mProgramMode == MODE_PAINT_INPUT) {
      mPainter->drawRing(mState->mPaintRadius, Vec2(mState->mMousePosition.x, mState->mMousePosition.y), mState->mColors[mState->mProgramMode]);
    }
  }


  void Renderer::drawNeurons() {
    int numFlashes = 0;

    // project to window coords
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      mState->mNeurons[i].soma_d = mState->worldToWindow(mState->mNeurons[i].soma);
      mState->mNeurons[i].axon_d = mState->worldToWindow(mState->mNeurons[i].axon);
    }

    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      Vec2 soma = mState->mNeurons[i].soma_d;
      Vec2 axon = mState->mNeurons[i].axon_d;

      // draw soma to axon line
      mPainter->drawLine(soma, axon, mState->mColors[COLOR_AXON_LINE]);

      // draw synapses
      for (int j = 0; j < (int)mState->mNeurons[i].synapses.size(); ++j) {
        if (mState->mNeurons[i].flash_time > mState->mSimTime) {
          mPainter->drawLine(soma, axon, mState->mColors[COLOR_AXON_LINE_FIRE]);
          mPainter->drawLine(axon, mState->mNeurons[mState->mNeurons[i].synapses[j].target].soma_d, mState->mColors[COLOR_SYNAPSE_FIRE]);
        }
        else {
          mPainter->drawLine(soma, axon, mState->mColors[COLOR_AXON_LINE]);
          mPainter->drawLine(axon, mState->mNeurons[mState->mNeurons[i].synapses[j].target].soma_d, mState->mColors[COLOR_SYNAPSE]);
        }
      }
    }

    // draw axon and soma non flashers
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      if (mState->mNeurons[i].flash_time <= mState->mSimTime) {
        mPainter->drawHex(NEURON_DRAW_RADIUS + (8.0f *(mState->mNeurons[i].charge / mState->mNeurons[i].threshold)), mState->mNeurons[i].soma_d, mState->mColors[COLOR_SOMA]);
        mPainter->drawRing(NEURON_DRAW_RADIUS, mState->mNeurons[i].axon_d, mState->mColors[COLOR_AXON]);
      }
    }

    // draw axon and soma flashers
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      if (mState->mNeurons[i].flash_time > mState->mSimTime) {
        mPainter->drawHex(NEURON_DRAW_RADIUS + 4.0f, mState->mNeurons[i].soma_d, mState->mColors[COLOR_SOMA_FIRE]);
        mPainter->drawHex(NEURON_DRAW_RADIUS + 3.0f, mState->mNeurons[i].axon_d, mState->mColors[COLOR_AXON_FIRE]);
        numFlashes++;
      }
    }

    // identify the input neurons
    for (int i = 0; i < (int)mState->mInputNeurons.size(); ++i) {
      mPainter->drawRing(8.0f, mState->mNeurons[mState->mInputNeurons[i]].soma_d, mState->mColors[COLOR_INPUT]);
    }

    // draw the flashing neuron ratio
    // Vec2 tl, br;
    //
    // tl.x = 100;
    // tl.y = 40;
    //
    // br.x = 400;
    // br.y = 20;
    //
    // draw_progress_bar(tl, br, (double)numFlashes /(double)mState->mNeurons.size(), mState->mColors[COLOR_PROG1], mState->mColors[COLOR_PROG2]);
  }
}
