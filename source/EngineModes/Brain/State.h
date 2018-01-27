#pragma once

#include <SDL2/SDL.h>

#include "../../engine/Vec2.h"
#include "../../engine/PseudoRandom.h"

#include "../../PriorityQueue.h"

#define NUM_COLORS (50)

#define COL_SOMA (0)
#define COL_SOMA_FIRE (1)
#define COL_AXON (2)
#define COL_AXON_FIRE (3)
#define COL_AXON_LINE (4)
#define COL_AXON_LINE_FIRE (5)
#define COL_SYNAPSE (6)
#define COL_SYNAPSE_FIRE (7)
#define COL_INPUT (8)
#define COL_PROG1 (9)
#define COL_PROG2 (10)


#define INITIAL_INPUT_LEVEL (10.0f)


namespace Brain {
  typedef struct {
    float r;
    float g;
    float b;
  } rgb_float_t;

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


    PseudoRandom* mPseudoRandom;

    rgb_float_t mColors[NUM_COLORS];

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

    // double paint_radius = 300.0;

    double mViewZoom = 1.0;
    v2d_t mViewBottomLeft;
    v2d_t mViewTopRight;

    // int mDrawInputFlag = 0;

  };
}
