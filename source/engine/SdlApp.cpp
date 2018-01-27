#include "SdlApp.h"

SdlApp::SdlApp() :
  mSdlWindow(nullptr),
  mSdlRenderer(nullptr),
  mSdlTexture(nullptr)
{}

SdlApp::~SdlApp()
{}

int SdlApp::init(unsigned width, unsigned height) {
  printf("initing sdl app: %d, %d\n", width, height);

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  mSdlWindow = SDL_CreateWindow("SdlApp", 100, 100, width, height, SDL_WINDOW_SHOWN);
  if (mSdlWindow == nullptr) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  mSdlRenderer = SDL_CreateRenderer(mSdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (mSdlRenderer == nullptr) {
    SDL_DestroyWindow(mSdlWindow);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  return 0;
}

int SdlApp::quit() {
  destroyTexture();
  SDL_DestroyRenderer(mSdlRenderer);
  SDL_DestroyWindow(mSdlWindow);
  SDL_Quit();

  return 0;
}

int SdlApp::drawFrame(const ImageBuffer* imageBuffer) {
  SDL_Surface* surface = imageBuffer->toSdlSurface();
  if (surface == nullptr) {
    // SDL_DestroyRenderer(mSdlRenderer);
    // SDL_DestroyWindow(mSdlWindow);
    // SDL_Quit();
    return 1;
  }

  // create the texture
  SDL_Texture* texture = SDL_CreateTextureFromSurface(mSdlRenderer, surface);
  SDL_FreeSurface(surface);
  if (texture == nullptr) {
    SDL_DestroyRenderer(mSdlRenderer);
    SDL_DestroyWindow(mSdlWindow);
    std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_RenderClear(mSdlRenderer);
  SDL_RenderCopy(mSdlRenderer, texture, nullptr, nullptr);
  SDL_RenderPresent(mSdlRenderer);

  destroyTexture();
  mSdlTexture = texture;

  return 0;
}

void SdlApp::destroyTexture() {
  if (mSdlTexture != nullptr) {
    SDL_DestroyTexture(mSdlTexture);
    mSdlTexture = nullptr;
  }
}
