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

#define MAX_DENDRITE_LENGTH (130)
#define MIN_AXON_LENGTH (125)
#define MAX_AXON_LENGTH (235)

#define INPUT_DELAY (33)

#define MIN_TIME_STEP (30)
#define SIMULATION_SPEED (.8)

#define PAINT_DENSITY (0.0002)



namespace Brain {
  class Controller {
  public:
    Controller(int windowWidth, int windowHeight);
    ~Controller();

    ImageBuffer* getOutputImageBuffer();
    void drawFrame();

    void update();
    bool handleInput();

    void paintNeurons(Vec2 center, double radius, int num);
    void removeSynapses();
    void removeNeurons(Vec2 pos, double radius);
    void paintInputNeuron(Vec2 pos, double radius);
    void removeInputNeurons(Vec2 pos, double radius);

    void growSynapses();
    void flushNeurons();
    void simReset();

  private:
    bool handleKeystroke();
    void handleMouseButton(int button, const Vec2& windowCoords);
    void handleMouseWheelEvent(const SDL_MouseWheelEvent& wheelEvent, const Vec2& windowCoords);

    void viewReset();
    void initNeurons();
    void growNeurons(Vec2 tl, Vec2 br, int num);
    void removeNeuron(int i);
    void removeInputNeuron(int i);
    void processEvents(double process_until);

    void drawNeurons();

    int mWindowWidth;
    int mWindowHeight;

    State mState;
    Renderer mRenderer;

    SDL_Event mSdlEvent;
  };
}
