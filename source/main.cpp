#include "engine/SdlApp.h"
#include "EngineModes/Brain/Controller.h"

#define SCREEN_WIDTH (1024)
#define SCREEN_HEIGHT (1024)

void mainLoop(Brain::Controller* brainController) {
  SdlApp sdlApp;
  if (sdlApp.init(SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
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
  Brain::Controller* brainController = new Brain::Controller(SCREEN_WIDTH, SCREEN_HEIGHT);
  mainLoop(brainController);
  delete brainController;

  return 0;
}
