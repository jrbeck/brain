#include <SDL/SDL.h>

class stopwatch_c {
	long total_time;
	long start_time;
	int paused;
	int timer_on;


public:
	stopwatch_c (void);
	void start (void);
	void stop (void);
	double read (void);
	void set_timer (double seconds);
	int timer_done (void);
	void reset (void);
};

stopwatch_c (void) {
	total_time = 0;
	start_time = SDL_GetTicks ();
	paused = 1;
}


void start (void) {
	paused = 0;
	start_time = SDL_GetTicks ();
}

void stop (void) {
	paused = 1;
	total_time += (SDL_GetTicks () - start_time);
}

double read (void) {
	long time;
	
	if (!paused) time = total_time + (SDL_GetTicks () - start_time);
	else time = total_time;
	
	return (double)time / 1000.0f;
}

void set_timer (double seconds) {
	
}

int timer_done (void) {
}

void reset (void) {
	total_time = 0;
	paused = 1;
}