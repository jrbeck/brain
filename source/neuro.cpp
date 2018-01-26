// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * neuron2.cpp
// *
// * neuron simulation program
// *
// * by john beck
// * last modified: 15 Jan 2010
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <vector>
#include <assert.h>

#include "include/sdl_util.h"
#include "include/v2d.h"
#include "include/math_util.h"
//#include "../qs_util.h"

using namespace std;


// defines * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#define SCREEN_W (1000)
#define SCREEN_H (1000)

#define FLASH_TIME (20)
#define NEURON_DRAW_RADIUS (2.0f)
#define MIN_THRESHOLD (25.0f)
#define MAX_THRESHOLD (30.0f)
#define DECAY_RATE (0.00053f)
#define SIGNAL_PROPOGATION_RATE (5.15f)

#define MAX_DENDRITE_LENGTH (1.35)
#define MIN_AXON_LENGTH (1.25)
#define MAX_AXON_LENGTH (1.35)

#define INPUT_DELAY (33)
#define INITIAL_INPUT_LEVEL (10.0f)

#define MIN_TIME_STEP (30)
#define SIMULATION_SPEED (.8)

#define NUM_COLORS (50)

#define COL_SOMA (0)
#define COL_SOMA_FIRE (1)
#define COL_AXON (2)
#define COL_AXON_FIRE (3)
#define COL_AXON_LINE (4)
#define COL_AXON_LINE_FIRE (5)
#define COL_SYNAPSE (6)
#define COL_SYNAPSE_FIRE (7)
#define COL_INPUT (8)
#define COL_PROG1 (9)
#define COL_PROG2 (10)

#define NUM_PROGRAM_MODES (3)

#define MODE_VIEW (0)
#define MODE_PAINT_NEURON (1)
#define MODE_PAINT_INPUT (2)

#define PAINT_DENSITY (0.0002)

// default viewport
#define VIEW_DEFAULT_BOTTOM (-10)
#define VIEW_DEFAULT_LEFT (-10)
#define VIEW_DEFAULT_TOP (10)
#define VIEW_DEFAULT_RIGHT (10)


// typedefs * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
typedef struct {
  int source;
  int target;
  double length;
  double percent;
} synapse_t;

typedef struct {
  v2d_t soma;    // soma position
  v2d_t axon;    // axon branch point

  v2d_t soma_d;  // translated drawing pos
  v2d_t axon_d;

  vector<synapse_t> synapse;  // synapse list

  double rest_potential;  // polarization
  double threshold;    // cross this the fire neuron
  double polarization;  // membrane potential
  double last_decay;    // bring the polarization back to rest potential

  double flash_time;    // for visualization
} neuron_t;

typedef struct {
  double time;
  int target;
  double charge;
} event_t;


// globals * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
SDL_Event sdlevent;
v2d_t mouse_position;
v2d_t mouse_delta;
int mouse_drag = 0;
int mouse_moved = 0;

vector<neuron_t> neuron;
vector<int> input_neuron;
vector<event_t> event_list;
rgb_float_t color[NUM_COLORS];
double sim_time;
double next_input_time;

long int frame = 0;
int num_synapses = 0;
int num_events = 0;
int num_fires = 0;
double furthest_right_fire = 0.0;
double input_level = INITIAL_INPUT_LEVEL;

int program_mode = MODE_PAINT_NEURON;
double paint_radius = 300.0;

double view_zoom = 1.0;
v2d_t view_bl, view_tr;

int draw_input_flag = 0;

// prototypes * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
int main(int argc, char** argv);
int initialize();
void init_colors();
void sim_reset();
void view_reset();
void init_neurons();
void grow_neurons(v2d_t tl, v2d_t br, int num);
void paint_neurons(v2d_t center, double radius, int num);
void grow_synapses();
void remove_synapses();
int shutdown();
void flush_neurons();
void main_loop();
int handle_keystroke();
int handle_input();
void handle_mouse_button(int button, v2d_t pos, int mode);
void remove_neurons(v2d_t pos, double radius);
void remove_neuron(int i);
void paint_input_neuron(v2d_t pos, double radius);
void remove_input_neurons(v2d_t pos, double radius);
void remove_input_neuron(int i);
void sim_update();
void process_events(double process_until);
void event_insert(vector<event_t>& heap, event_t e);
event_t event_extract_soonest(vector<event_t>& heap);
void event_decrease_key(vector<event_t>& heap, int i);
void event_min_heapify(vector<event_t>& heap, int i);
inline int heap_parent(int i);
inline int heap_left(int i);
inline int heap_right(int i);
void draw_neurons();
v2d_t window_to_world(v2d_t a);
v2d_t world_to_window(v2d_t a);


// functions * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

int main(int argc, char** argv) {
  printf("program entry ----------------\n");

  if (initialize()) {
		return 1;
	}

  main_loop();
  shutdown();

  return 0;
}

int initialize() {
  sdl_mode_info_t mode;

  mode.screen_w = SCREEN_W;
  mode.screen_h = SCREEN_H;
  mode.fullscreen = 0;

  if (init_sdl(mode)) return 1;

  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapBuffers();

  mouse_position.x = 0;
  mouse_position.y = 0;

  sim_reset();
  init_colors();
  view_reset();

  return 0;
}

void init_colors() {
  for (int i = 0; i < NUM_COLORS; ++i) {
    color[i].r =(GLfloat)r_num(0.0f, 1.0f);
    color[i].g =(GLfloat)r_num(0.0f, 1.0f);
    color[i].b =(GLfloat)r_num(0.0f, 1.0f);
  }

  color[COL_SOMA].r = 0.0f;
  color[COL_SOMA].g = 0.5f;
  color[COL_SOMA].b = 0.75f;

  color[COL_SOMA_FIRE].r = 1.0f;
  color[COL_SOMA_FIRE].g = 1.0f;
  color[COL_SOMA_FIRE].b = 0.5f;

  color[COL_AXON].r = 0.5f;
  color[COL_AXON].g = 0.5f;
  color[COL_AXON].b = 0.75f;

  color[COL_AXON_FIRE].r = 1.0f;
  color[COL_AXON_FIRE].g = 1.0f;
  color[COL_AXON_FIRE].b = 0.5f;

  color[COL_AXON_LINE].r = 0.0f;
  color[COL_AXON_LINE].g = 0.5f;
  color[COL_AXON_LINE].b = 0.75f;

  color[COL_AXON_LINE_FIRE].r = 1.0f;
  color[COL_AXON_LINE_FIRE].g = 1.0f;
  color[COL_AXON_LINE_FIRE].b = 0.5f;

  color[COL_SYNAPSE].r = 0.0f;
  color[COL_SYNAPSE].g = 0.2f;
  color[COL_SYNAPSE].b = 0.35f;

  color[COL_SYNAPSE_FIRE].r = 1.0f;
  color[COL_SYNAPSE_FIRE].g = 0.5f;
  color[COL_SYNAPSE_FIRE].b = 0.0f;

  color[COL_INPUT].r = 0.5f;
  color[COL_INPUT].g = 1.0f;
  color[COL_INPUT].b = 1.0f;

  color[COL_PROG1].r = 0.6f;
  color[COL_PROG1].g = 0.65f;
  color[COL_PROG1].b = 0.85f;

  color[COL_PROG2].r = 0.20f;
  color[COL_PROG2].g = 0.25f;
  color[COL_PROG2].b = 0.35f;
}

void sim_reset() {
  //neuron.clear();
  //init_neurons();

  // initialize the input neurons
//  init_inputs();

  //grow_synapses();

  flush_neurons();
  remove_synapses();

  // reset the global stats
  num_events = 0;
  num_fires = 0;
  furthest_right_fire = 0.0f;

  sim_time = 0.0f;
  event_list.clear();

  next_input_time = 0.0;

//  view_zoom = 1.0;
//  view_trans.x = SCREEN_W * 0.5;
//  view_trans.y = SCREEN_H * 0.5;

  printf("\nsimulation restarted .........................\n");
}

void view_reset() {
  view_bl.x = VIEW_DEFAULT_LEFT;
  view_bl.y = VIEW_DEFAULT_BOTTOM;

  view_tr.x = VIEW_DEFAULT_RIGHT;
  view_tr.y = VIEW_DEFAULT_TOP;

  view_zoom =(view_tr.x - view_bl.x) / SCREEN_W;
}

void init_neurons() {
  grow_neurons(v2d_vector(100, 900), v2d_vector(300, 100), 300);
  grow_neurons(v2d_vector(300, 700), v2d_vector(700, 300), 300);
  grow_neurons(v2d_vector(700, 900), v2d_vector(900, 100), 300);
}

void grow_neurons(v2d_t tl, v2d_t br, int num) {
  neuron_t n;

  for (int i = 0; i < num; ++i) {
    n.soma.x = r_num(tl.x, br.x);
    n.soma.y = r_num(br.y, tl.y);

    n.axon.x = n.soma.x + r_num(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);
    n.axon.y = n.soma.y + r_num(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);

    n.threshold = r_num(MIN_THRESHOLD, MAX_THRESHOLD);
    n.charge = 0.0f;
    n.last_decay = 0.0f;

    n.flash_time = 0.0f;

    n.synapse.resize(0);

    neuron.push_back(n);
  }
}

void paint_neurons(v2d_t center, double radius, int num) {
  neuron_t n;

  for (int i = 0; i < num; ++i) {
    // pick a random spot in the circle
    n.soma = v2d_vector(r_num(-100, 100), r_num(-100, 100));

    n.soma = v2d_normalize(n.soma);
    n.soma = v2d_scale(n.soma, r_num(0, radius));

    n.soma = v2d_add(n.soma, center);

    // now add the axon
    n.axon.x = n.soma.x + r_num(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);
    n.axon.y = n.soma.y + r_num(-MIN_AXON_LENGTH, MIN_AXON_LENGTH);

    n.threshold = r_num(MIN_THRESHOLD, MAX_THRESHOLD);
    n.charge = 0.0f;
    n.last_decay = 0.0f;

    n.flash_time = 0.0f;

    n.synapse.resize(0);

    neuron.push_back(n);
  }
}




// grow the synapses
void grow_synapses() {
  vector<int> in_range;
  synapse_t new_syn;

  v2d_t tl, br;

  tl.x = 100;
  tl.y = 40;

  br.x = 400;
  br.y = 20;

  // reset the global counter
  num_synapses = 0;

  printf("growing synapses .....................................\n");

  // step through the neurons and assign synapses
  for (int i = 0; i <(int)neuron.size(); ++i) {
    // mkae sure this is empty
    neuron[i].synapse.clear();

    // build a list of neurons in range
    for (int j = 0; j <(int)neuron.size(); ++j) {
      if (i == j || v2d_dist(neuron[i].axon, neuron[j].soma) > MAX_DENDRITE_LENGTH) continue;

      in_range.push_back(j);
    }

    for (int j = 0; j <(int)in_range.size(); ++j) {
      double dist = v2d_dist(neuron[i].axon, neuron[in_range[j]].soma);

      new_syn.length = dist;
      new_syn.source = i;
      new_syn.target = in_range[j];
      new_syn.percent = 1.0f /(double)in_range.size();

      // push the new synapse on the stack
      neuron[i].synapse.push_back(new_syn);

      // this is the global count for stats
      num_synapses++;
    }

    // reset the in range list
    in_range.resize(0);

    glClear(GL_COLOR_BUFFER_BIT);

    handle_input();

    draw_mouse(mouse_position, 22, 32);
    draw_progress_bar(tl, br,(double)i /(double)neuron.size(), color[COL_PROG1], color[COL_PROG2]);

    SDL_GL_SwapBuffers();
  }
}


void remove_synapses() {
  for (int i = 0; i <(int)neuron.size(); ++i) {
    neuron[i].synapse.clear();
  }
}


// cleanup and report
int shutdown() {
  deinit_sdl();

  printf("\n");
  printf("---------------------------------------------------------\n");
  printf("simluation time: %f\n", sim_time);
  printf("program exec time: %d\n", SDL_GetTicks());
  printf("frames: %d\n", frame);
  printf("neurons: %d, synapses: %d\n", neuron.size(), num_synapses);
  printf("synapses:neurons ratio: %.3f\n",(double)num_synapses /(double)neuron.size());
  printf("total events: %d, events to time: %.3f\n", num_events,(double)num_events /(double)sim_time);
  printf("fires: %d, highest x value: %.2f\n", num_fires, furthest_right_fire);

  for (int i = 0; i <(int)neuron.size(); ++i) {
    printf("(%d) coord: %4.3f, %4.3f\n", i, neuron[i].axon.x, neuron[i].axon.y);
  }

  // free the neurons
  neuron.clear();

  return 0;
}


void flush_neurons() {
  for (int i = 0; i <(int)neuron.size(); ++i) {
    neuron[i].flash_time = 0.0;
    neuron[i].last_decay = 0.0;
    neuron[i].charge = 0.0;
  }
}


// main program loop * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void main_loop() {
  int quit = 0;

  while (!quit) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (program_mode == MODE_VIEW) sim_update();

    draw_neurons();
    if (program_mode == MODE_VIEW) draw_mouse(mouse_position, 22, 32);
    else if (program_mode == MODE_PAINT_NEURON || program_mode == MODE_PAINT_INPUT)
      draw_ring((float)paint_radius, v2d_vector(mouse_position.x, SCREEN_H - mouse_position.y), color[program_mode]);

    SDL_GL_SwapBuffers();
    frame++;

    quit = handle_input();
  }
}


// deal with a keystroke
int handle_keystroke() {
  int quit = 0;

  switch (sdlevent.key.keysym.sym) {
    case SDLK_a:
      if (input_level < 100) input_level++;
      break;
    case SDLK_z:
      if (input_level > 0) input_level--;
      break;

    case SDLK_s:
      grow_synapses();
      flush_neurons();
      break;

    case SDLK_r:
      sim_reset();
      break;

    case SDLK_m:
      program_mode =(program_mode + 1) % NUM_PROGRAM_MODES;
      sim_reset();

      break;

    case SDLK_p:
      //paused =(paused + 1) % 2;
      break;
    case SDLK_SPACE:
      //if (!paused) break;
      //advance_frame = 1;
      break;
    case SDLK_ESCAPE:
      quit = 1;
      break;

    default:
      break;
  }

  return quit;
}


// handle an SDL_Event
int handle_input() {
  int quit = 0;
  //Uint8 *keystates = SDL_GetKeyState(NULL);

  //LEFT = keystates[SDLK_LEFT];
  //RIGHT = keystates[SDLK_RIGHT];
  //if (!p.layout_with_down) DOWN = keystates[SDLK_DOWN];
  //else DOWN = 0;

  //mouse_moved = 0;

  while (SDL_PollEvent(&sdlevent) && !quit) {
    switch (sdlevent.type) {
      case SDL_KEYDOWN:
        quit = handle_keystroke();
        break;

      case SDL_KEYUP:
        //handle_keyup();
        break;

      case SDL_MOUSEMOTION:
        mouse_delta.x =(mouse_position.x - sdlevent.motion.x);
        mouse_delta.y = -(mouse_position.y - sdlevent.motion.y);

        mouse_position.x = sdlevent.motion.x;
        mouse_position.y = sdlevent.motion.y;

        mouse_moved = 1;

        break;

      case SDL_MOUSEBUTTONDOWN:
        handle_mouse_button(sdlevent.button.button, v2d_vector(sdlevent.button.x, SCREEN_H - sdlevent.button.y), program_mode);

      case SDL_MOUSEBUTTONUP:
        if (sdlevent.button.button == SDL_BUTTON_LEFT) mouse_drag = 0;
//        handle_mouse_button(sdlevent.button.button, window_to_world(v2d_vector(sdlevent.button.x, SCREEN_H - sdlevent.button.y)), program_mode);

        break;

      default:
        break;
    }
  }



  // handle mouse drag
  // FIXME: not here please!
  Uint8 ms;
  ms = SDL_GetMouseState(NULL, NULL);

  if (program_mode == MODE_VIEW) {
    if (mouse_moved && (ms & SDL_BUTTON(SDL_BUTTON_LEFT))) {
      v2d_t t = v2d_scale(mouse_delta, view_zoom);

      view_bl = v2d_add(view_bl, t);
      view_tr = v2d_add(view_tr, t);

      mouse_moved = 0;
    }
  }


  return quit;
}


//
void handle_mouse_button(int button, v2d_t pos, int mode) {
  if (mode == MODE_VIEW) {
    if (button == SDL_BUTTON_WHEELUP) {
      // get the world pos of the event
      v2d_t wp = window_to_world(pos);

      view_bl = v2d_sub(wp, v2d_scale(v2d_sub(wp, view_bl), 0.8333));
      view_tr = v2d_add(wp, v2d_scale(v2d_sub(view_tr, wp), 0.8333));

      view_zoom =(view_tr.x - view_bl.x) / SCREEN_W;

    }
    else if (button == SDL_BUTTON_WHEELDOWN) {
      v2d_t wp = window_to_world(pos);

      view_bl = v2d_sub(wp, v2d_scale(v2d_sub(wp, view_bl), 1.2));
      view_tr = v2d_add(wp, v2d_scale(v2d_sub(view_tr, wp), 1.2));

      view_zoom =(view_tr.x - view_bl.x) / SCREEN_W;
    }
  }

  // we need to know where we are in the world
  pos = window_to_world(pos);

  if (mode == MODE_PAINT_NEURON) {
    if (button == SDL_BUTTON_LEFT) paint_neurons(pos, paint_radius * view_zoom,(int)(PAINT_DENSITY *(MY_PI * paint_radius * paint_radius)));
    if (button == SDL_BUTTON_RIGHT) {
      remove_synapses();
      remove_neurons(pos, paint_radius * view_zoom);
    }

    if (button == SDL_BUTTON_WHEELDOWN && paint_radius > 7)
      paint_radius -= 6;
    else if (button == SDL_BUTTON_WHEELUP)
      paint_radius += 6;
  }
  else if (mode == MODE_PAINT_INPUT) {
    if (button == SDL_BUTTON_LEFT) paint_input_neuron(pos, paint_radius * view_zoom);
    if (button == SDL_BUTTON_RIGHT) remove_input_neurons(pos, paint_radius * view_zoom);

    if (button == SDL_BUTTON_WHEELDOWN && paint_radius > 7)
      paint_radius -= 6;
    else if (button == SDL_BUTTON_WHEELUP)
      paint_radius += 6;
  }
}


void remove_neurons(v2d_t pos, double radius) {
  for (int i =(int)neuron.size() - 1; i >= 0; i--)
    if (v2d_dist(neuron[i].soma, pos) <= radius) remove_neuron(i);

}


void remove_neuron(int i) {
  for (int j = 0; j <(int)input_neuron.size(); ++j) {
    if (i == input_neuron[j]) remove_input_neuron(j);
    else if (input_neuron[j] > i) input_neuron[j]--;
  }

  for (; i <(int)neuron.size() - 1; ++i) {
    neuron[i] = neuron[i + 1];
  }

  neuron.pop_back();
}


void paint_input_neuron(v2d_t pos, double radius) {
  if (neuron.size() == 0) return;

  int closest = 0;

  for (int i = 0; i <(int)neuron.size(); ++i) {
    if (v2d_dist(neuron[i].soma, pos) < v2d_dist(neuron[closest].soma, pos)) closest = i;
  }

  if (v2d_dist(neuron[closest].soma, pos) < radius) {
    for (int i = 0; i <(int)input_neuron.size(); ++i) {
      if (i == closest) return;
    }

    input_neuron.push_back(closest);
  }
}


void remove_input_neurons(v2d_t pos, double radius) {
  for (int i = 0; i <(int)input_neuron.size(); ++i)
    if (v2d_dist(neuron[input_neuron[i]].soma, pos) <= radius) {
      remove_input_neuron(i);
      i--;
    }
}


void remove_input_neuron(int i) {
  for (; i <(int)input_neuron.size() - 1; ++i) {
    input_neuron[i] = input_neuron[i + 1];
  }

  input_neuron.pop_back();
}


// update the simulation
void sim_update() {
  static unsigned long last = 0;
  unsigned long now = SDL_GetTicks();

  double delta_time = now - last;
  if (delta_time < MIN_TIME_STEP) return; // enforce update granularity
  last = now;

  sim_time +=(delta_time * SIMULATION_SPEED);

//  printf("updating sim: sim:%f, delta: %f * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n", sim_time, delta_time);

//  if (paused) {
//    if (!advance_frame) return;
//    delta_time = 150;
//    advance_frame = 0;
//  }

  // catch the simulation up
  process_events(sim_time);

  for (int i = 0; i <(int)neuron.size(); ++i) {
    neuron[i].last_decay = sim_time;
    if (neuron[i].charge == 0.0f) continue;

    neuron[i].charge -=(delta_time * DECAY_RATE);
    if (neuron[i].charge < 0.0f) neuron[i].charge = 0.0f;
  }
}


// advance the simulation by a time step: process_until
void process_events(double process_until) {
  event_t temp;

  // send input signals if it is time
  while (process_until > next_input_time) {
    draw_input_flag = 3;
    temp.time = next_input_time;
    next_input_time += INPUT_DELAY;
    for (int i = 0; i <(int)input_neuron.size(); ++i) {
      temp.target = input_neuron[i];
      temp.charge = input_level;

      event_insert(event_list, temp);

//      printf("%d: %f @ %f\n", e[i].target, e[i].charge, e[i].time);
    }
  }

  // process all the events which occur in this time frame
  while (event_list.size() > 0 && event_list[0].time <= process_until) {
    event_t cur = event_extract_soonest(event_list);

    // just to make things a touch easier to write
    int t = cur.target;

//    printf("%5.3f:(q_size: %d): target:%d, targ charge:%.2f, syn charge:%.2f, targ thresh:%.2f\n", cur.time, e.size(), t, neuron[t].charge, cur.charge,
//      neuron[t].threshold);

    // handle the decay of the target neuron's charge
    neuron[t].charge -=((cur.time - neuron[t].last_decay) * DECAY_RATE);
    neuron[t].last_decay = cur.time;

    if (neuron[t].charge < 0.0f) neuron[t].charge = 0.0f;

    // add the charge from the synapse
    neuron[t].charge += cur.charge;

//    printf("post decay targ chg: %.2f\n", neuron[t].charge);

    // does the neuron fire?
    if (neuron[t].charge >= neuron[t].threshold) {
      // update the stats
      if (neuron[t].soma.x > furthest_right_fire) furthest_right_fire = neuron[t].soma.x;
      num_fires++;

      // send a signal through each synapse
      for (int j = 0; j <(int)neuron[t].synapse.size(); ++j) {
        temp.target = neuron[t].synapse[j].target;
        temp.charge = neuron[t].threshold * neuron[t].synapse[j].percent;
        temp.time = cur.time +(neuron[t].synapse[j].length *(1.0f / SIGNAL_PROPOGATION_RATE));

        event_insert(event_list, temp);

//        printf("fire: %d, chg: %.2f, arrives at: %.3f\n", temp.target, temp.charge, temp.time);

        neuron[t].flash_time = cur.time + FLASH_TIME;
      }

      // reset the charge
      neuron[t].charge = 0.0f;
    }
  }

//  printf("%d, ", e.size());
}


// insert an event on the queue
void event_insert(vector<event_t>& heap, event_t e) {
  heap.push_back(e);

  event_decrease_key(heap,(int)heap.size() - 1);

  // increment the global counter
  num_events++;
}


event_t event_extract_soonest(vector<event_t>& heap) {
  // this better not happen!
  assert(heap.size() > 0);

  event_t  ret = heap[0];

  heap[0] = heap[heap.size() - 1];
  heap.pop_back();

  event_min_heapify(heap, 0);

  return ret;
}

void event_decrease_key(vector<event_t>& heap, int i) {
  int parent;

  while (i > 0 && heap[parent = heap_parent(i)].time > heap[i].time) {
    // swap!
    event_t temp = heap[i];
    heap[i] = heap[parent];
    heap[parent] = temp;

    i = parent;
  }
}

void event_min_heapify(vector<event_t>& heap, int i) {
  int l = heap_left(i);
  int r = heap_right(i);

  int smallest;

  if (l <(int)heap.size() && heap[l].time < heap[i].time) smallest = l;
  else smallest = i;

  if (r <(int)heap.size() && heap[r].time < heap[smallest].time) smallest = r;

  if (smallest != i) {
    // swap!
    event_t temp = heap[i];
    heap[i] = heap[smallest];
    heap[smallest] = temp;

    event_min_heapify(heap, smallest);
  }
}


inline int heap_parent(int i) {
  return((i + 1) >> 1) - 1;
}


inline int heap_left(int i) {
  return((i + 1) << 1) - 1;
}


inline int heap_right(int i) {
  return(i + 1) << 1;
}



// draw the neurons
void draw_neurons() {
  int num_flashes = 0;

  // project to window coords
  for (int i = 0; i <(int)neuron.size(); ++i) {
    neuron[i].soma_d = world_to_window(neuron[i].soma);
    neuron[i].axon_d = world_to_window(neuron[i].axon);
  }

  for (int i = 0; i <(int)neuron.size(); ++i) {
    v2d_t soma = neuron[i].soma_d;
    v2d_t axon = neuron[i].axon_d;

    // draw soma to axon line
    draw_line(soma, axon, color[COL_AXON_LINE]);

    // draw synapses
    for (int j = 0; j <(int)neuron[i].synapse.size(); ++j) {
      if (neuron[i].flash_time > sim_time) {
        draw_line(soma, axon, color[COL_AXON_LINE_FIRE]);
        draw_line(axon, neuron[neuron[i].synapse[j].target].soma_d, color[COL_SYNAPSE_FIRE]);
      }
      else {
        draw_line(soma, axon, color[COL_AXON_LINE]);
        draw_line(axon, neuron[neuron[i].synapse[j].target].soma_d, color[COL_SYNAPSE]);
      }
    }
  }

  // draw axon and soma non flashers
  for (int i = 0; i <(int)neuron.size(); ++i) {
    if (neuron[i].flash_time <= sim_time) {
      draw_hex(NEURON_DRAW_RADIUS +(8.0f *(float)(neuron[i].charge / neuron[i].threshold)), neuron[i].soma_d, color[COL_SOMA]);
      draw_ring(NEURON_DRAW_RADIUS, neuron[i].axon_d, color[COL_AXON]);
    }
  }

  // draw axon and soma flashers
  for (int i = 0; i <(int)neuron.size(); ++i) {
    if (neuron[i].flash_time > sim_time) {
      draw_hex(NEURON_DRAW_RADIUS + 4.0f, neuron[i].soma_d, color[COL_SOMA_FIRE]);
      draw_hex(NEURON_DRAW_RADIUS + 3.0f, neuron[i].axon_d, color[COL_AXON_FIRE]);
      num_flashes++;
    }
  }

  // identify the input neurons
  for (int i = 0; i <(int)input_neuron.size(); ++i) {
    draw_ring(8.0f, neuron[input_neuron[i]].soma_d, color[COL_INPUT]);
  }

  v2d_t tl, br;

  tl.x = 100;
  tl.y = 40;

  br.x = 400;
  br.y = 20;

  draw_progress_bar(tl, br,(double)num_flashes /(double)neuron.size(), color[COL_PROG1], color[COL_PROG2]);
}


v2d_t window_to_world(v2d_t a) {
//  return v2d_sub(v2d_vector(-SCREEN_W * .5, -SCREEN_H * .5), v2d_sub(v2d_scale(a, 1.0 / view_zoom), view_trans));
//  return v2d_scale(v2d_sub(a, view_trans), 1.0 / view_zoom);

  // sorta works
//  return v2d_sub(v2d_scale(a, 1.0 / view_zoom), view_trans);

  return v2d_add(v2d_scale(a, view_zoom), view_bl);


}

v2d_t world_to_window(v2d_t a) {
//  return v2d_add(v2d_vector(SCREEN_W * .5, SCREEN_H * .5), v2d_scale(v2d_add(a, view_trans), view_zoom));

  //sorta works
//  return v2d_scale(v2d_add(a, view_trans), view_zoom);

//  return v2d_add(v2d_scale(a, view_zoom), view_trans);


  return v2d_scale(v2d_sub(a, view_bl), 1.0 / view_zoom);

}
