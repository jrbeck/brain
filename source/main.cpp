#include "engine/SdlApp.h"
#include "EngineModes/Brain/Controller.h"

#define DEFAULT_SCREEN_WIDTH (1024)
#define DEFAULT_SCREEN_HEIGHT (1024)

void mainLoop(Brain::Controller* brainController, int screenW, int screenH) {
  SdlApp sdlApp;
  if (sdlApp.init(screenW, screenH) != 0) {
    printf("ERROR: could not initialize SdlApp\n");
    return;
  }

  bool quit = false;
  Uint32 frameCount = 0;

  while (!quit) {
    quit = brainController->handleInput();
    brainController->update();
    brainController->drawFrame();
    sdlApp.drawFrame(brainController->getOutputImageBuffer());

    frameCount++;
  }

  sdlApp.quit();
}

int main(int nargs, char** argv) {
  int screenW = DEFAULT_SCREEN_WIDTH;
  int screenH = DEFAULT_SCREEN_HEIGHT;

  if (nargs == 2) {
    screenW = screenH = atoi(argv[1]);
  }
  else if (nargs == 3) {
    screenW = atoi(argv[1]);
    screenH = atoi(argv[2]);
  }

  Brain::Controller* brainController = new Brain::Controller(screenW, screenH);
  mainLoop(brainController, screenW, screenH);
  delete brainController;

  return 0;
}
