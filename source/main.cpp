#include "engine/SdlApp.h"
#include "EngineModes/Brain/Controller.h"

#define SCREEN_WIDTH (512)
#define SCREEN_HEIGHT (512)

#define NUM_PROGRAM_MODES (3)

#define MODE_VIEW (0)
#define MODE_PAINT_NEURON (1)
#define MODE_PAINT_INPUT (2)


SDL_Event gSdlEvent;
v2d_t gMousePosition;
v2d_t gMouseDelta;
int gMouseDrag = 0;
int gMouseMoved = 0;
int gProgramMode = MODE_PAINT_NEURON;


void mainLoop(Brain::Controller* brainController);
int handleKeystroke(Brain::Controller* brainController);
int handleInput(Brain::Controller* brainController);
// void handleMouseButton(int button, v2d_t pos, int mode, Brain::Controller* brainController);


int main() {
  gMousePosition.x = 0;
  gMousePosition.y = 0;

  Brain::Controller* brainController = new Brain::Controller(SCREEN_WIDTH, SCREEN_HEIGHT);
  mainLoop(brainController);
  delete brainController;

  return 0;
}

void mainLoop(Brain::Controller* brainController) {
  SdlApp sdlApp;
  if (sdlApp.init(SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
    printf("ERROR: could not initialize SdlApp\n");
    return;
  }

  bool quit = false;
  Uint32 frameCount = 0;

  while (!quit) {
    if (gProgramMode == MODE_VIEW) {
      brainController->update();
    }

    brainController->drawFrame();
    sdlApp.drawFrame(brainController->getOutputImageBuffer());

    // if (gProgramMode == MODE_VIEW) {
    //   draw_mouse(gMousePosition, 22, 32);
    // }
    // else if (gProgramMode == MODE_PAINT_NEURON || gProgramMode == MODE_PAINT_INPUT) {
    //   draw_ring((float)paint_radius, v2d_vector(gMousePosition.x, SCREEN_H - gMousePosition.y), color[gProgramMode]);
    // }

    frameCount++;

    quit = handleInput(brainController);
  }

  sdlApp.quit();
}

int handleKeystroke(Brain::Controller* brainController) {
  bool quit = false;

  switch (gSdlEvent.key.keysym.sym) {
    // case SDLK_a:
    //   if (input_level < 100) {
    //     input_level++;
    //   }
    //   break;
    // case SDLK_z:
    //   if (input_level > 0) {
    //     input_level--;
    //   }
    //   break;

    case SDLK_s:
      brainController->growSynapses();
      brainController->flushNeurons();
      break;

    case SDLK_r:
      brainController->simReset();
      break;

    case SDLK_m:
      gProgramMode = (gProgramMode + 1) % NUM_PROGRAM_MODES;
      brainController->simReset();
      break;

    case SDLK_ESCAPE:
      quit = true;
      break;

    default:
      break;
  }

  return quit;
}

int handleInput(Brain::Controller* brainController) {
  int quit = 0;

  while (SDL_PollEvent(&gSdlEvent) && !quit) {
    switch (gSdlEvent.type) {
      case SDL_KEYDOWN:
        quit = handleKeystroke(brainController);
        break;

    //   case SDL_MOUSEMOTION:
    //     gMouseDelta.x = (gMousePosition.x - gSdlEvent.motion.x);
    //     gMouseDelta.y = -(gMousePosition.y - gSdlEvent.motion.y);
    //
    //     gMousePosition.x = gSdlEvent.motion.x;
    //     gMousePosition.y = gSdlEvent.motion.y;
    //
    //     gMouseMoved = 1;
    //
    //     break;
    //
    //   case SDL_MOUSEBUTTONDOWN:
    //     handleMouseButton(gSdlEvent.button.button, v2d_vector(gSdlEvent.button.x, SCREEN_H - gSdlEvent.button.y), gProgramMode);
    //     break;
    //
    //   case SDL_MOUSEBUTTONUP:
    //     if (gSdlEvent.button.button == SDL_BUTTON_LEFT) {
    //       gMouseDrag = 0;
    //     }
    //    // handleMouseButton(gSdlEvent.button.button, windowToWorld(v2d_vector(gSdlEvent.button.x, SCREEN_H - gSdlEvent.button.y)), gProgramMode);
    //
    //     break;
    //
    }
  }

  // handle mouse drag
  // FIXME: not here please!
  // Uint8 ms;
  // ms = SDL_GetMouseState(NULL, NULL);
  //
  // if (gProgramMode == MODE_VIEW) {
  //   if (gMouseMoved && (ms & SDL_BUTTON(SDL_BUTTON_LEFT))) {
  //     v2d_t t = v2d_scale(gMouseDelta, view_zoom);
  //     view_bl = v2d_add(view_bl, t);
  //     view_tr = v2d_add(view_tr, t);
  //     gMouseMoved = 0;
  //   }
  // }

  return quit;
}

// void handleMouseButton(int button, v2d_t windowCoords, int mode, Brain::Controller* brainController) {
//   v2d_t worldCoords = windowToWorld(windowCoords);
//
//   if (mode == MODE_VIEW) {
//     if (button == SDL_BUTTON_WHEELUP) {
//       view_bl = v2d_sub(worldCoords, v2d_scale(v2d_sub(worldCoords, view_bl), 0.8333));
//       view_tr = v2d_add(worldCoords, v2d_scale(v2d_sub(view_tr, worldCoords), 0.8333));
//     }
//     else if (button == SDL_BUTTON_WHEELDOWN) {
//       view_bl = v2d_sub(worldCoords, v2d_scale(v2d_sub(worldCoords, view_bl), 1.2));
//       view_tr = v2d_add(worldCoords, v2d_scale(v2d_sub(view_tr, worldCoords), 1.2));
//     }
//     view_zoom = (view_tr.x - view_bl.x) / SCREEN_W;
//   }
//   else if (mode == MODE_PAINT_NEURON) {
//     if (button == SDL_BUTTON_LEFT) {
//       brainController->paintNeurons(worldCoords, paint_radius * view_zoom,(int)(PAINT_DENSITY *(MY_PI * paint_radius * paint_radius)));
//     }
//     if (button == SDL_BUTTON_RIGHT) {
//       brainController->removeSynapses();
//       brainController->removeNeurons(worldCoords, paint_radius * view_zoom);
//     }
//
//     if (button == SDL_BUTTON_WHEELDOWN && paint_radius > 7) {
//       paint_radius -= 6;
//     }
//     else if (button == SDL_BUTTON_WHEELUP) {
//       paint_radius += 6;
//     }
//   }
//   else if (mode == MODE_PAINT_INPUT) {
//     if (button == SDL_BUTTON_LEFT) {
//       brainController->paintInputNeuron(worldCoords, paint_radius * view_zoom);
//     }
//     if (button == SDL_BUTTON_RIGHT) {
//       brainController->removeInputNeurons(worldCoords, paint_radius * view_zoom);
//     }
//
//     if (button == SDL_BUTTON_WHEELDOWN && paint_radius > 7) {
//       paint_radius -= 6;
//     }
//
//     else if (button == SDL_BUTTON_WHEELUP) {
//       paint_radius += 6;
//     }
//   }
// }
