#pragma once

#include "ImageBuffer.h"

class EngineModeController {
public:
  virtual ~EngineModeController() {};

  virtual bool handleInput() = 0;
  virtual bool update() = 0;
  virtual void render() = 0;

  virtual ImageBuffer* getOutputImageBuffer() = 0;
};
