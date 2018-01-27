#pragma once

#include "ImageBuffer.h"

class EngineModeController {
public:
  virtual ~EngineModeController() {};

  virtual int update() = 0;
  virtual void render() = 0;

  virtual ImageBuffer* getOutputImageBuffer() = 0;
};
