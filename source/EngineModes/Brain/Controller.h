#pragma once

// #include <cstdio>
// #include <cstdlib>
// #include <cmath>
#include <vector>

#include <SDL2/SDL.h>

#include "State.h"
#include "Renderer.h"


#define FLASH_TIME (20)
#define MIN_THRESHOLD (25.0f)
#define MAX_THRESHOLD (30.0f)
#define DECAY_RATE (0.00053f)
#define SIGNAL_PROPOGATION_RATE (5.15f)

#define MAX_DENDRITE_LENGTH (1.35)
#define MIN_AXON_LENGTH (1.25)
#define MAX_AXON_LENGTH (2.35)

#define INPUT_DELAY (33)

#define MIN_TIME_STEP (30)
#define SIMULATION_SPEED (.8)

#define PAINT_DENSITY (0.001)

#define VIEW_DEFAULT_BOTTOM (-10)
#define VIEW_DEFAULT_LEFT (-10)
#define VIEW_DEFAULT_TOP (10)
#define VIEW_DEFAULT_RIGHT (10)


namespace Brain {
  class Controller {
  public:
    Controller(int windowWidth, int windowHeight);
    ~Controller();

    ImageBuffer* getOutputImageBuffer();
    void drawFrame();

    void update();
    bool handleInput();

    void paintNeurons(v2d_t center, double radius, int num);
    void removeSynapses();
    void removeNeurons(v2d_t pos, double radius);
    void paintInputNeuron(v2d_t pos, double radius);
    void removeInputNeurons(v2d_t pos, double radius);

    void growSynapses();
    void flushNeurons();
    void simReset();

  private:
    bool handleKeystroke();
    void handleMouseButton(int button, v2d_t pos);

    void viewReset();
    void initNeurons();
    void growNeurons(v2d_t tl, v2d_t br, int num);
    void removeNeuron(int i);
    void removeInputNeuron(int i);
    void processEvents(double process_until);

    void drawNeurons();

    int mWindowWidth;
    int mWindowHeight;

    State* mState;
    Renderer* mRenderer;

    SDL_Event mSdlEvent;
  };
}
