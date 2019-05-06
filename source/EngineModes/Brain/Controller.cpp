#include "Controller.h"

namespace Brain {
  Controller::Controller(int windowWidth, int windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mState(windowWidth, windowHeight),
    mRenderer(windowWidth, windowHeight)
  {
    // mState = new State();
    // mRenderer = new Renderer(windowWidth, windowHeight);

    simReset();
    viewReset();
  }

  Controller::~Controller() {
    // printf("\n");
    // printf("---------------------------------------------------------\n");
    // printf("simluation time: %f\n", mState.mSimTime);
    // printf("program exec time: %d\n", SDL_GetTicks());
    // printf("neurons: %d, synapses: %d\n", mState.mNeurons.size(), mState.mNumSynapses);
    // printf("synapses:neurons ratio: %.3f\n", (double)mState.mNumSynapses /(double)mState.mNeurons.size());
    // printf("total events: %d, events to time: %.3f\n", mState.mEvents.size(), (double)mState.mEvents.size() /(double)mState.mSimTime);
    // printf("fires: %d, highest x value: %.2f\n", mState.mNumFires, mState.mFurthestRightFire);
    //
    // for (int i = 0; i < (int)mState.mNeurons.size(); ++i) {
    //   printf("(%d) coord: %4.3f, %4.3f\n", i, mState.mNeurons[i].axon.x, mState.mNeurons[i].axon.y);
    // }

    // if (mRenderer != nullptr) {
    //   delete mRenderer;
    // }
    // if (mState != nullptr) {
    //   delete mState;
    // }
  }

  ImageBuffer* Controller::getOutputImageBuffer() {
    return mRenderer.getImageBuffer();
  }

  void Controller::render() {
    mRenderer.render(mState);
  }

  bool Controller::handleInput() {
    bool quit = 0;
    Vec2 mouseDelta;
    // bool mouseDrag = false;

    while (SDL_PollEvent(&mSdlEvent) && !quit) {
      switch (mSdlEvent.type) {
        case SDL_KEYDOWN:
          quit = handleKeystroke();
          break;

        case SDL_MOUSEMOTION:
          mouseDelta.x = mSdlEvent.motion.x - mState.mMousePosition.x;
          mouseDelta.y = -(mSdlEvent.motion.y - mState.mMousePosition.y);

          mState.mMousePosition.x = mSdlEvent.motion.x;
          mState.mMousePosition.y = mSdlEvent.motion.y;

          mState.mMouseMoved = true;
          break;

        case SDL_MOUSEBUTTONDOWN:
          handleMouseButton(mSdlEvent.button.button, Vec2(mSdlEvent.button.x, mSdlEvent.button.y));
          break;

        case SDL_MOUSEBUTTONUP:
          if (mSdlEvent.button.button == SDL_BUTTON_LEFT) {
            // mouseDrag = false;
          }
         // handleMouseButton(mSdlEvent.button.button, mState.windowToWorld(Vec2(mSdlEvent.button.x, mSdlEvent.button.y)));

          break;

        case SDL_MOUSEWHEEL:
          handleMouseWheelEvent(mSdlEvent.wheel);
      }

      // handle mouse drag
      // FIXME: not here please!
      Uint8 mouseState = SDL_GetMouseState(NULL, NULL);

      if (mState.mProgramMode == MODE_SIMULATE) {
        if (mState.mMouseMoved && (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
          mState.mViewport.translateByPixels(-mouseDelta);
          mState.mMouseMoved = false;
        }
      }

    }


    return quit;
  }

  bool Controller::handleKeystroke() {
    bool quit = false;

    switch (mSdlEvent.key.keysym.sym) {
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
        growSynapses();
        flushNeurons();
        break;

      case SDLK_r:
        simReset();
        break;

      case SDLK_m:
        mState.mProgramMode = (mState.mProgramMode + 1) % NUM_PROGRAM_MODES;
        simReset();
        break;

      case SDLK_ESCAPE:
        quit = true;
        break;

      default:
        break;
    }

    return quit;
  }

  void Controller::handleMouseButton(int button, const Vec2& windowCoords) {
    // TODO: stop trying to be so clever ... just use screen coords and let the
    // viewport handle the transform
    Vec2 worldCoords = mState.windowToWorld(windowCoords);

    // TODO: this is just here because the zoom that used to live in mState is no longer there
    VEC2_DATA_TYPE zoom = mState.mViewport.getAntipixelScale().x;

    if (mState.mProgramMode == MODE_SIMULATE) {
    }
    else if (mState.mProgramMode == MODE_PAINT_NEURON) {
      if (button == SDL_BUTTON_LEFT) {
        paintNeurons(worldCoords,
          mState.mPaintRadius * zoom,
          (int)(PAINT_DENSITY * (MY_PI * mState.mPaintRadius * mState.mPaintRadius)));
      }
      if (button == SDL_BUTTON_RIGHT) {
        removeSynapses();
        removeNeurons(worldCoords, mState.mPaintRadius * zoom);
      }
    }
    else if (mState.mProgramMode == MODE_PAINT_INPUT) {
      if (button == SDL_BUTTON_LEFT) {
        paintInputNeuron(worldCoords, mState.mPaintRadius * zoom);
      }
      if (button == SDL_BUTTON_RIGHT) {
        removeInputNeurons(worldCoords, mState.mPaintRadius * zoom);
      }
    }
  }

  void Controller::handleMouseWheelEvent(const SDL_MouseWheelEvent& wheelEvent) {
    if (mState.mProgramMode == MODE_SIMULATE) {
      Vec2 worldPosition = mState.windowToWorld(mState.mMousePosition);
      if (wheelEvent.y > 0) {
        mState.mViewport.zoom(0.8333, worldPosition);
      }
      else if (wheelEvent.y < 0) {
        mState.mViewport.zoom(1.2, worldPosition);
      }
    }
    else if (mState.mProgramMode == MODE_PAINT_NEURON || mState.mProgramMode == MODE_PAINT_INPUT) {
      if (wheelEvent.y < 0 && mState.mPaintRadius > 7) {
        mState.mPaintRadius -= 6;
      }
      else if (wheelEvent.y > 0) {
        mState.mPaintRadius += 6;
      }
    }
  }

  void Controller::simReset() {
    mState.reset();
  }

  void Controller::viewReset() {
    mState.mViewport.reset();
  }

  void Controller::initNeurons() {
    growNeurons(Vec2(100, 900), Vec2(300, 100), 300);
    growNeurons(Vec2(300, 700), Vec2(700, 300), 300);
    growNeurons(Vec2(700, 900), Vec2(900, 100), 300);
  }

  void Controller::growNeurons(Vec2 tl, Vec2 br, int num) {
    Neuron neuron;

    VEC2_DATA_TYPE minAxonLength = MIN_AXON_LENGTH * mState.mViewport.getAntipixelScale().x;
    VEC2_DATA_TYPE maxAxonLength = MAX_AXON_LENGTH * mState.mViewport.getAntipixelScale().x;

    for (int i = 0; i < num; ++i) {
      neuron.soma.x = mState.mPseudoRandom.nextDouble(tl.x, br.x);
      neuron.soma.y = mState.mPseudoRandom.nextDouble(br.y, tl.y);

      neuron.axon.x = neuron.soma.x + mState.mPseudoRandom.nextDouble(-minAxonLength, minAxonLength);
      neuron.axon.y = neuron.soma.y + mState.mPseudoRandom.nextDouble(-maxAxonLength, maxAxonLength);

      neuron.threshold = mState.mPseudoRandom.nextDouble(MIN_THRESHOLD, MAX_THRESHOLD);
      neuron.charge = 0.0f;
      neuron.last_decay = 0.0f;

      neuron.flash_time = 0.0f;

      neuron.synapses.resize(0);

      mState.mNeurons.push_back(neuron);
    }
  }

  void Controller::paintNeurons(Vec2 center, double radius, int num) {
    Neuron neuron;

    VEC2_DATA_TYPE minAxonLength = MIN_AXON_LENGTH * mState.mViewport.getAntipixelScale().x;
    VEC2_DATA_TYPE maxAxonLength = MAX_AXON_LENGTH * mState.mViewport.getAntipixelScale().x;

    for (int i = 0; i < num; ++i) {
      // pick a random spot in the circle
      neuron.soma = Vec2(mState.mPseudoRandom.nextDouble(-100, 100), mState.mPseudoRandom.nextDouble(-100, 100));

      neuron.soma.normalize();
      neuron.soma *= mState.mPseudoRandom.nextDouble(0, radius);
      neuron.soma += center;

      // now add the axon
      neuron.axon.x = neuron.soma.x + mState.mPseudoRandom.nextDouble(-minAxonLength, minAxonLength);
      neuron.axon.y = neuron.soma.y + mState.mPseudoRandom.nextDouble(-maxAxonLength, maxAxonLength);

      neuron.threshold = mState.mPseudoRandom.nextDouble(MIN_THRESHOLD, MAX_THRESHOLD);
      neuron.charge = 0.0f;
      neuron.last_decay = 0.0f;

      neuron.flash_time = 0.0f;

      neuron.synapses.resize(0);

      mState.mNeurons.push_back(neuron);
    }
  }

  void Controller::growSynapses() {
    std::vector<int> inRangeNeuronIndices;
    Synapse synapse;

    mState.mNumSynapses = 0;

    printf("growing synapses .....................................\n");

    for (int i = 0; i < (int)mState.mNeurons.size(); ++i) {
      mState.mNeurons[i].synapses.clear();

      for (int j = 0; j < (int)mState.mNeurons.size(); ++j) {
        if (i == j || Vec2::distance(mState.mNeurons[i].axon, mState.mNeurons[j].soma) > MAX_DENDRITE_LENGTH) {
          continue;
        }
        inRangeNeuronIndices.push_back(j);
      }

      for (size_t j = 0; j < inRangeNeuronIndices.size(); ++j) {
        double dist = Vec2::distance(mState.mNeurons[i].axon, mState.mNeurons[inRangeNeuronIndices[j]].soma);

        synapse.length = dist;
        synapse.source = i;
        synapse.target = inRangeNeuronIndices[j];
        synapse.percent = 1.0f / (double)inRangeNeuronIndices.size();
        mState.mNeurons[i].synapses.push_back(synapse);

        mState.mNumSynapses++;
      }

      inRangeNeuronIndices.resize(0);
    }
  }

  void Controller::removeSynapses() {
    mState.removeSynapses();
  }

  void Controller::flushNeurons() {
    mState.flushNeurons();
  }

  void Controller::removeNeurons(Vec2 pos, double radius) {
    for (int i = (int)mState.mNeurons.size() - 1; i >= 0; i--) {
      if (Vec2::distance(mState.mNeurons[i].soma, pos) <= radius) {
        removeNeuron(i);
      }
    }
  }

  void Controller::removeNeuron(int i) {
    for (int j = 0; j < (int)mState.mInputNeurons.size(); ++j) {
      if (i == mState.mInputNeurons[j]) {
        removeInputNeuron(j);
      }
      else if (mState.mInputNeurons[j] > i) {
        mState.mInputNeurons[j]--;
      }
    }

    for (; i < (int)mState.mNeurons.size() - 1; ++i) {
      mState.mNeurons[i] = mState.mNeurons[i + 1];
    }

    mState.mNeurons.pop_back();
  }

  void Controller::paintInputNeuron(Vec2 pos, double radius) {
    if (mState.mNeurons.size() == 0) {
      return;
    }

    int closest = 0;

    for (int i = 0; i < (int)mState.mNeurons.size(); ++i) {
      if (Vec2::distance(mState.mNeurons[i].soma, pos) < Vec2::distance(mState.mNeurons[closest].soma, pos)) {
        closest = i;
      }
    }

    if (Vec2::distance(mState.mNeurons[closest].soma, pos) < radius) {
      for (int i = 0; i < (int)mState.mInputNeurons.size(); ++i) {
        if (i == closest) {
          return;
        }
      }

      mState.mInputNeurons.push_back(closest);
    }
  }

  void Controller::removeInputNeurons(Vec2 pos, double radius) {
    for (int i = 0; i < (int)mState.mInputNeurons.size(); ++i)
      if (Vec2::distance(mState.mNeurons[mState.mInputNeurons[i]].soma, pos) <= radius) {
        removeInputNeuron(i);
        --i;
      }
  }

  void Controller::removeInputNeuron(int i) {
    for (; i < (int)mState.mInputNeurons.size() - 1; ++i) {
      mState.mInputNeurons[i] = mState.mInputNeurons[i + 1];
    }

    mState.mInputNeurons.pop_back();
  }

  bool Controller::update() {
    // if (mState.mProgramMode == MODE_SIMULATE) {
    // }

    unsigned long now = SDL_GetTicks();

    double delta_time = now - mState.mLastUpdateTicks;
    if (delta_time < MIN_TIME_STEP) {
      return false;
    }
    mState.mLastUpdateTicks = now;

    mState.mSimTime += (delta_time * SIMULATION_SPEED);
    processEvents(mState.mSimTime);

    for (int i = 0; i < (int)mState.mNeurons.size(); ++i) {
      mState.mNeurons[i].last_decay = mState.mSimTime;
      if (mState.mNeurons[i].charge == 0.0f) {
        continue;
      }

      mState.mNeurons[i].charge -= (delta_time * DECAY_RATE);
      if (mState.mNeurons[i].charge < 0.0f) {
        mState.mNeurons[i].charge = 0.0f;
      }
    }

    return false;
  }

  void Controller::processEvents(double process_until) {
    Event event;

    // send input signals if it is time
    while (process_until > mState.mNextInputTime) {
      event.time = mState.mNextInputTime;
      mState.mNextInputTime += INPUT_DELAY;
      for (int i = 0; i < (int)mState.mInputNeurons.size(); ++i) {
        event.target = mState.mInputNeurons[i];
        event.charge = mState.mInputLevel;
        mState.mEvents.insert(event);
      }
    }

    // process all the events which occur in this time frame
    while (mState.mEvents.peek(&event) ? event.time <= process_until : false) {
      mState.mEvents.extract(nullptr);

      int t = event.target;

      // handle the decay of the target neuron's charge
      mState.mNeurons[t].charge -= ((event.time - mState.mNeurons[t].last_decay) * DECAY_RATE);
      mState.mNeurons[t].last_decay = event.time;

      if (mState.mNeurons[t].charge < 0.0f) {
        mState.mNeurons[t].charge = 0.0f;
      }

      // add the charge from the synapse
      mState.mNeurons[t].charge += event.charge;

      // does the neuron fire?
      if (mState.mNeurons[t].charge >= mState.mNeurons[t].threshold) {
        // update the stats
        if (mState.mNeurons[t].soma.x > mState.mFurthestRightFire) {
          mState.mFurthestRightFire = mState.mNeurons[t].soma.x;
        }
        mState.mNumFires++;

        // send a signal through each synapse
        for (int j = 0; j < (int)mState.mNeurons[t].synapses.size(); ++j) {
          event.target = mState.mNeurons[t].synapses[j].target;
          event.charge = mState.mNeurons[t].threshold * mState.mNeurons[t].synapses[j].percent;
          event.time = event.time +(mState.mNeurons[t].synapses[j].length *(1.0f / SIGNAL_PROPOGATION_RATE));

          mState.mEvents.insert(event);
          mState.mNeurons[t].flash_time = event.time + FLASH_TIME;
        }

        mState.mNeurons[t].charge = 0.0f;
      }
    }
  }
}
