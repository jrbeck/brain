#pragma once

#include <SDL2/SDL.h>

#include "../../engine/Vec2.h"
#include "../../engine/PseudoRandom.h"

#include "../../PriorityQueue.h"

#define INITIAL_INPUT_LEVEL (10.0f)

namespace Brain {
  enum ProgramModes {
    MODE_SIMULATE,
    MODE_PAINT_NEURON,
    MODE_PAINT_INPUT,
    NUM_PROGRAM_MODES
  };

  enum Colors {
    COLOR_SOMA,
    COLOR_SOMA_FIRE,
    COLOR_AXON,
    COLOR_AXON_FIRE,
    COLOR_AXON_LINE,
    COLOR_AXON_LINE_FIRE,
    COLOR_SYNAPSE,
    COLOR_SYNAPSE_FIRE,
    COLOR_INPUT,
    COLOR_PROG1,
    COLOR_PROG2,
    NUM_COLORS,
  };

  struct RgbFloat {
    float r;
    float g;
    float b;
  };

  struct Synapse {
    int source;
    int target;
    double length;
    double percent;
  };

  struct Neuron {
    v2d_t soma; // soma position
    v2d_t axon; // axon branch point

    v2d_t soma_d; // translated drawing pos
    v2d_t axon_d;

    std::vector<Synapse> synapses; // synapse list

    double rest_potential; // polarization
    double threshold; // cross this the fire neuron
    double polarization; // membrane potential
    double last_decay; // bring the polarization back to rest potential
    double charge;

    double flash_time; // for visualization
  };

  class State {
public:
    State();
    ~State();

    void initColors();

    void reset();


    void flushNeurons();
    void removeSynapses();


    v2d_t windowToWorld(v2d_t a);
    v2d_t worldToWindow(v2d_t a);

    int mProgramMode;

    PseudoRandom* mPseudoRandom;

    RgbFloat mColors[NUM_COLORS];

    PriorityQueue mEvents;
    std::vector<Neuron> mNeurons;
    std::vector<int> mInputNeurons;

    unsigned long mLastUpdateTicks;
    double mSimTime;
    double mNextInputTime;

    int mNumSynapses = 0;
    int mNumFires = 0;
    double mFurthestRightFire = 0.0;
    double mInputLevel = INITIAL_INPUT_LEVEL;

    v2d_t mMousePosition;
    bool mMouseMoved;

    double mPaintRadius = 300.0;

    double mViewZoom = 1.0;
    v2d_t mViewBottomLeft;
    v2d_t mViewTopRight;

    // int mDrawInputFlag = 0;

  };
}
