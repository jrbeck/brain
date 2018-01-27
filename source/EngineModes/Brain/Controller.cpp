#include "Controller.h"

namespace Brain {
  Controller::Controller(int windowWidth, int windowHeight) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mState(nullptr),
    mRenderer(nullptr)
  {
    mState = new State();
    mRenderer = new Renderer(windowWidth, windowHeight);

    simReset();
    viewReset();
  }

  Controller::~Controller() {
    // printf("\n");
    // printf("---------------------------------------------------------\n");
    // printf("simluation time: %f\n", mState->mSimTime);
    // printf("program exec time: %d\n", SDL_GetTicks());
    // printf("neurons: %d, synapses: %d\n", mState->mNeurons.size(), mState->mNumSynapses);
    // printf("synapses:neurons ratio: %.3f\n", (double)mState->mNumSynapses /(double)mState->mNeurons.size());
    // printf("total events: %d, events to time: %.3f\n", mState->mEvents.size(), (double)mState->mEvents.size() /(double)mState->mSimTime);
    // printf("fires: %d, highest x value: %.2f\n", mState->mNumFires, mState->mFurthestRightFire);
    //
    // for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
    //   printf("(%d) coord: %4.3f, %4.3f\n", i, mState->mNeurons[i].axon.x, mState->mNeurons[i].axon.y);
    // }

    if (mRenderer != nullptr) {
      delete mRenderer;
    }
    if (mState != nullptr) {
      delete mState;
    }
  }

  ImageBuffer* Controller::getOutputImageBuffer() {
    return mRenderer->getImageBuffer();
  }

  void Controller::drawFrame() {
    mRenderer->drawFrame(mState);
  }


  void Controller::simReset() {
    mState->reset();
  }

  void Controller::viewReset() {
    mState->mViewBottomLeft.x = VIEW_DEFAULT_LEFT;
    mState->mViewBottomLeft.y = VIEW_DEFAULT_BOTTOM;

    mState->mViewTopRight.x = VIEW_DEFAULT_RIGHT;
    mState->mViewTopRight.y = VIEW_DEFAULT_TOP;

    mState->mViewZoom = (mState->mViewTopRight.x - mState->mViewBottomLeft.x) / (double)mWindowWidth;
  }

  void Controller::initNeurons() {
    growNeurons(v2d_v(100, 900), v2d_v(300, 100), 300);
    growNeurons(v2d_v(300, 700), v2d_v(700, 300), 300);
    growNeurons(v2d_v(700, 900), v2d_v(900, 100), 300);
  }

  void Controller::growNeurons(v2d_t tl, v2d_t br, int num) {
    Neuron neuron;

    for (int i = 0; i < num; ++i) {
      neuron.soma.x = mState->mPseudoRandom->nextDouble(tl.x, br.x);
      neuron.soma.y = mState->mPseudoRandom->nextDouble(br.y, tl.y);

      neuron.axon.x = neuron.soma.x + mState->mPseudoRandom->nextDouble(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);
      neuron.axon.y = neuron.soma.y + mState->mPseudoRandom->nextDouble(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);

      neuron.threshold = mState->mPseudoRandom->nextDouble(MIN_THRESHOLD, MAX_THRESHOLD);
      neuron.charge = 0.0f;
      neuron.last_decay = 0.0f;

      neuron.flash_time = 0.0f;

      neuron.synapses.resize(0);

      mState->mNeurons.push_back(neuron);
    }
  }

  void Controller::paintNeurons(v2d_t center, double radius, int num) {
    Neuron neuron;

    for (int i = 0; i < num; ++i) {
      // pick a random spot in the circle
      neuron.soma = v2d_v(mState->mPseudoRandom->nextDouble(-100, 100), mState->mPseudoRandom->nextDouble(-100, 100));

      neuron.soma = v2d_normalize(neuron.soma);
      neuron.soma = v2d_scale(neuron.soma, mState->mPseudoRandom->nextDouble(0, radius));

      neuron.soma = v2d_add(neuron.soma, center);

      // now add the axon
      neuron.axon.x = neuron.soma.x + mState->mPseudoRandom->nextDouble(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);
      neuron.axon.y = neuron.soma.y + mState->mPseudoRandom->nextDouble(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);

      neuron.threshold = mState->mPseudoRandom->nextDouble(MIN_THRESHOLD, MAX_THRESHOLD);
      neuron.charge = 0.0f;
      neuron.last_decay = 0.0f;

      neuron.flash_time = 0.0f;

      neuron.synapses.resize(0);

      mState->mNeurons.push_back(neuron);
    }
  }

  void Controller::growSynapses() {
    std::vector<int> inRangeNeuronIndices;
    Synapse synapse;

    v2d_t tl, br;

    tl.x = 100;
    tl.y = 40;

    br.x = 400;
    br.y = 20;

    mState->mNumSynapses = 0;

    printf("growing synapses .....................................\n");

    // step through the neurons and assign synapses
    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      mState->mNeurons[i].synapses.clear();

      for (int j = 0; j < (int)mState->mNeurons.size(); ++j) {
        if (i == j || v2d_dist(mState->mNeurons[i].axon, mState->mNeurons[j].soma) > MAX_DENDRITE_LENGTH) {
          continue;
        }
        inRangeNeuronIndices.push_back(j);
      }

      for (size_t j = 0; j < inRangeNeuronIndices.size(); ++j) {
        double dist = v2d_dist(mState->mNeurons[i].axon, mState->mNeurons[inRangeNeuronIndices[j]].soma);

        synapse.length = dist;
        synapse.source = i;
        synapse.target = inRangeNeuronIndices[j];
        synapse.percent = 1.0f /(double)inRangeNeuronIndices.size();
        mState->mNeurons[i].synapses.push_back(synapse);

        mState->mNumSynapses++;
      }

      inRangeNeuronIndices.resize(0);

      // TODO: apparently this takes a while and used to provide some
      // feedback to the user while it was working
      // glClear(GL_COLOR_BUFFER_BIT);
      //
      // handle_input();
      //
      // draw_mouse(mouse_position, 22, 32);
      // draw_progress_bar(tl, br, (double)i /(double)mState->mNeurons.size(), mColors[COL_PROG1], mColors[COL_PROG2]);
      //
      // SDL_GL_SwapBuffers();
    }
  }

  void Controller::removeSynapses() {
    mState->removeSynapses();
  }

  void Controller::flushNeurons() {
    mState->flushNeurons();
  }

  void Controller::removeNeurons(v2d_t pos, double radius) {
    for (int i = (int)mState->mNeurons.size() - 1; i >= 0; i--) {
      if (v2d_dist(mState->mNeurons[i].soma, pos) <= radius) {
        removeNeuron(i);
      }
    }
  }

  void Controller::removeNeuron(int i) {
    for (int j = 0; j < (int)mState->mInputNeurons.size(); ++j) {
      if (i == mState->mInputNeurons[j]) {
        removeInputNeuron(j);
      }
      else if (mState->mInputNeurons[j] > i) {
        mState->mInputNeurons[j]--;
      }
    }

    for (; i < (int)mState->mNeurons.size() - 1; ++i) {
      mState->mNeurons[i] = mState->mNeurons[i + 1];
    }

    mState->mNeurons.pop_back();
  }

  void Controller::paintInputNeuron(v2d_t pos, double radius) {
    if (mState->mNeurons.size() == 0) {
      return;
    }

    int closest = 0;

    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      if (v2d_dist(mState->mNeurons[i].soma, pos) < v2d_dist(mState->mNeurons[closest].soma, pos)) {
        closest = i;
      }
    }

    if (v2d_dist(mState->mNeurons[closest].soma, pos) < radius) {
      for (int i = 0; i < (int)mState->mInputNeurons.size(); ++i) {
        if (i == closest) {
          return;
        }
      }

      mState->mInputNeurons.push_back(closest);
    }
  }

  void Controller::removeInputNeurons(v2d_t pos, double radius) {
    for (int i = 0; i < (int)mState->mInputNeurons.size(); ++i)
      if (v2d_dist(mState->mNeurons[mState->mInputNeurons[i]].soma, pos) <= radius) {
        removeInputNeuron(i);
        i--;
      }
  }

  void Controller::removeInputNeuron(int i) {
    for (; i < (int)mState->mInputNeurons.size() - 1; ++i) {
      mState->mInputNeurons[i] = mState->mInputNeurons[i + 1];
    }

    mState->mInputNeurons.pop_back();
  }

  void Controller::update() {
    unsigned long now = SDL_GetTicks();

    double delta_time = now - mState->mLastUpdateTicks;
    if (delta_time < MIN_TIME_STEP) {
      return;
    }
    mState->mLastUpdateTicks = now;

    mState->mSimTime += (delta_time * SIMULATION_SPEED);
    processEvents(mState->mSimTime);

    for (int i = 0; i < (int)mState->mNeurons.size(); ++i) {
      mState->mNeurons[i].last_decay = mState->mSimTime;
      if (mState->mNeurons[i].charge == 0.0f) {
        continue;
      }

      mState->mNeurons[i].charge -= (delta_time * DECAY_RATE);
      if (mState->mNeurons[i].charge < 0.0f) {
        mState->mNeurons[i].charge = 0.0f;
      }
    }
  }

  void Controller::processEvents(double process_until) {
    Event event;

    // send input signals if it is time
    while (process_until > mState->mNextInputTime) {
      // mDrawInputFlag = 3;
      event.time = mState->mNextInputTime;
      mState->mNextInputTime += INPUT_DELAY;
      for (int i = 0; i < (int)mState->mInputNeurons.size(); ++i) {
        event.target = mState->mInputNeurons[i];
        event.charge = mState->mInputLevel;
        mState->mEvents.insert(event);
      }
    }

    // process all the events which occur in this time frame
    while (mState->mEvents.peek(&event) ? event.time <= process_until : false) {
      mState->mEvents.extract(nullptr);

      int t = event.target;

      // handle the decay of the target neuron's charge
      mState->mNeurons[t].charge -= ((event.time - mState->mNeurons[t].last_decay) * DECAY_RATE);
      mState->mNeurons[t].last_decay = event.time;

      if (mState->mNeurons[t].charge < 0.0f) {
        mState->mNeurons[t].charge = 0.0f;
      }

      // add the charge from the synapse
      mState->mNeurons[t].charge += event.charge;

      // does the neuron fire?
      if (mState->mNeurons[t].charge >= mState->mNeurons[t].threshold) {
        // update the stats
        if (mState->mNeurons[t].soma.x > mState->mFurthestRightFire) {
          mState->mFurthestRightFire = mState->mNeurons[t].soma.x;
        }
        mState->mNumFires++;

        // send a signal through each synapse
        for (int j = 0; j < (int)mState->mNeurons[t].synapses.size(); ++j) {
          event.target = mState->mNeurons[t].synapses[j].target;
          event.charge = mState->mNeurons[t].threshold * mState->mNeurons[t].synapses[j].percent;
          event.time = event.time +(mState->mNeurons[t].synapses[j].length *(1.0f / SIGNAL_PROPOGATION_RATE));

          mState->mEvents.insert(event);
          mState->mNeurons[t].flash_time = event.time + FLASH_TIME;
        }

        mState->mNeurons[t].charge = 0.0f;
      }
    }
  }
}
