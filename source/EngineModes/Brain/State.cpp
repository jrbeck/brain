#include "State.h"

namespace Brain {
  State::State() :
    mPseudoRandom(nullptr)
  {
    mPseudoRandom = new PseudoRandom();

    initColors();
    reset();
  }

  State::~State() {
    mNeurons.clear();

    if (mPseudoRandom != nullptr) {
      delete mPseudoRandom;
    }
  }

  void State::initColors() {
    for (int i = 0; i < NUM_COLORS; ++i) {
      mColors[i].r = (float)mPseudoRandom->nextDouble(0.0f, 1.0f);
      mColors[i].g = (float)mPseudoRandom->nextDouble(0.0f, 1.0f);
      mColors[i].b = (float)mPseudoRandom->nextDouble(0.0f, 1.0f);
    }

    mColors[COL_SOMA].r = 0.0f;
    mColors[COL_SOMA].g = 0.5f;
    mColors[COL_SOMA].b = 0.75f;

    mColors[COL_SOMA_FIRE].r = 1.0f;
    mColors[COL_SOMA_FIRE].g = 1.0f;
    mColors[COL_SOMA_FIRE].b = 0.5f;

    mColors[COL_AXON].r = 0.5f;
    mColors[COL_AXON].g = 0.5f;
    mColors[COL_AXON].b = 0.75f;

    mColors[COL_AXON_FIRE].r = 1.0f;
    mColors[COL_AXON_FIRE].g = 1.0f;
    mColors[COL_AXON_FIRE].b = 0.5f;

    mColors[COL_AXON_LINE].r = 0.0f;
    mColors[COL_AXON_LINE].g = 0.5f;
    mColors[COL_AXON_LINE].b = 0.75f;

    mColors[COL_AXON_LINE_FIRE].r = 1.0f;
    mColors[COL_AXON_LINE_FIRE].g = 1.0f;
    mColors[COL_AXON_LINE_FIRE].b = 0.5f;

    mColors[COL_SYNAPSE].r = 0.0f;
    mColors[COL_SYNAPSE].g = 0.2f;
    mColors[COL_SYNAPSE].b = 0.35f;

    mColors[COL_SYNAPSE_FIRE].r = 1.0f;
    mColors[COL_SYNAPSE_FIRE].g = 0.5f;
    mColors[COL_SYNAPSE_FIRE].b = 0.0f;

    mColors[COL_INPUT].r = 0.5f;
    mColors[COL_INPUT].g = 1.0f;
    mColors[COL_INPUT].b = 1.0f;

    mColors[COL_PROG1].r = 0.6f;
    mColors[COL_PROG1].g = 0.65f;
    mColors[COL_PROG1].b = 0.85f;

    mColors[COL_PROG2].r = 0.20f;
    mColors[COL_PROG2].g = 0.25f;
    mColors[COL_PROG2].b = 0.35f;
  }


  void State::reset() {
    //mNeurons.clear();
    //initNeurons();

    //growSynapses();

    flushNeurons();
    removeSynapses();

    mNumFires = 0;
    mFurthestRightFire = 0.0f;

    mLastUpdateTicks = SDL_GetTicks();
    mSimTime = 0.0f;
    mEvents.clear();

    mNextInputTime = 0.0;

  //  mViewZoom = 1.0;
  //  view_trans.x = (double)mScreenWidth * 0.5;
  //  view_trans.y = (double)mScreenHeight * 0.5;
  }


  void State::flushNeurons() {
    for (size_t i = 0; i < mNeurons.size(); ++i) {
      mNeurons[i].flash_time = 0.0;
      mNeurons[i].last_decay = 0.0;
      mNeurons[i].charge = 0.0;
    }
  }

  void State::removeSynapses() {
    for (size_t i = 0; i < mNeurons.size(); ++i) {
      mNeurons[i].synapses.clear();
    }
  }


  v2d_t State::windowToWorld(v2d_t a) {
    return v2d_add(v2d_scale(a, mViewZoom), mViewBottomLeft);
  }

  v2d_t State::worldToWindow(v2d_t a) {
    return v2d_scale(v2d_sub(a, mViewBottomLeft), 1.0 / mViewZoom);
  }
}
