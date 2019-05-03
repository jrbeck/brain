#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "../../engine/Vec2.h"
#include "../../engine/PseudoRandom.h"
#include "../../engine/Viewport.h"
#include "../../engine/Painter.h"

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

  struct Synapse {
    int source;
    int target;
    double length;
    double percent;
  };

  struct Neuron {
    Vec2 soma; // soma position
    Vec2 axon; // axon branch point

    Vec2 soma_d; // translated drawing pos
    Vec2 axon_d;

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
    State() = delete;
    State(size_t windowWidth, size_t windowHeight);
    ~State();

    void initColors();

    void reset();

    void flushNeurons();
    void removeSynapses();

    Vec2 windowToWorld(const Vec2& windowCoords) const;
    Vec2 worldToWindow(const Vec2& worldCoords) const;

    size_t mWindowWidth;
    size_t mWindowHeight;

    int mProgramMode;

    PseudoRandom mPseudoRandom;

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

    Vec2 mMousePosition;
    bool mMouseMoved;

    double mPaintRadius = 300.0;

    Viewport mViewport;
  };
}
