// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * sdl_util.h
// *
// * SDL utility library
// *
// * by john beck
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#ifndef SDL_UTIL_H_
#define SDL_UTIL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "v2d.h"


// DEFINES * * * * * * * * * * * * * * * * * * * * * * * *
#define MY_PI       (3.14159265358979323846)
#define DEG2RAD		(0.01745329251994329577)


// TYPEDEFS * * * * * * * * * * * * * * * * * * * * * * * *
typedef struct {
	GLfloat r;
	GLfloat g;
	GLfloat b;
} rgb_float_t;

typedef struct {
	int initialized;
	int screen_w;
	int screen_h;
	int fullscreen;
} sdl_mode_info_t;


// GLOBALS * * * * * * * * * * * * * * * * * * * * * * * *
sdl_mode_info_t sdlu_mode;


// PROTOTYPES * * * * * * * * * * * * * * * * * * * * * * * *
void draw_circle (float radius, v2d_t center, rgb_float_t color);
void draw_ring (float radius, v2d_t center, rgb_float_t color);
void draw_hex (float radius, v2d_t center, rgb_float_t color);
void draw_mouse (v2d_t position, float w, float h);
void draw_quad (float x, float y, float w, float h, rgb_float_t color);
void draw_progress_bar (v2d_t tl, v2d_t br, double percent, rgb_float_t c1, rgb_float_t c2);
void draw_line (v2d_t a, v2d_t b, rgb_float_t color);
//void draw_img_buf (char *data);
void draw_texture (GLuint tex, int left, int top, int right, int bottom);
int load_texture (char *filename, GLuint *texture_handle);
int init_sdl (sdl_mode_info_t mode);
int deinit_sdl (void);


// FUNCTIONS * * * * * * * * * * * * * * * * * * * * * * * *

void draw_circle (double radius, v2d_t center, rgb_float_t color) {
	if (radius <= 0.0f) return;

	glColor3f (color.r, color.g, color.b);

	glBegin (GL_TRIANGLE_FAN);
		for (int i = 0; i < 360; i += 10) {
			double degInRad = (double)i * DEG2RAD;
			glVertex2f ((GLfloat)((cos (degInRad) * radius) + center.x), (GLfloat)((sin (degInRad) * radius) + center.y));
		}
	glEnd ();
}

void draw_ring (float radius, v2d_t center, rgb_float_t color) {
	if (radius <= 0.0f) return;

	glColor3f (color.r, color.g, color.b);

	v2d_t a;
	
	a.x = center.x + radius;
	a.y = center.y;
	
	glBegin (GL_LINE_STRIP);

	glVertex2d (a.x, a.y);

		for (int i = 10; i < 360; i += 10) {
			double degInRad = (double)i * DEG2RAD;
			a.x = center.x + (cos (degInRad) * radius);
			a.y = center.y + (sin (degInRad) * radius);
			
			glVertex2d (a.x, a.y);
		}

		a.x = center.x + radius;
		a.y = center.y;

		glVertex2d (a.x, a.y);

	glEnd ();
}

void draw_hex (float radius, v2d_t center, rgb_float_t color) {
	if (radius <= 0.0f) return;

	glColor3f (color.r, color.g, color.b);

	glBegin (GL_TRIANGLE_FAN);
		for (int i = 90; i < 450; i += 60) {
			double degInRad = (double)i * DEG2RAD;
			glVertex2f ((GLfloat)((cos (degInRad) * radius) + center.x), (GLfloat)((sin (degInRad) * radius) + center.y));
		}
	glEnd ();
}


void draw_mouse (v2d_t position, float w, float h) {
	double x = position.x;
	double y = position.y;

	glColor3f (0.0f, 0.2f, 1.0f);

	glBegin (GL_TRIANGLES);
		glVertex2f ((GLfloat)position.x, (GLfloat)(sdlu_mode.screen_h - position.y));
		glVertex2f ((GLfloat)position.x, (GLfloat)(sdlu_mode.screen_h - (position.y + h)));
		glVertex2f ((GLfloat)(position.x + w), (GLfloat)(sdlu_mode.screen_h - (position.y + (h * .666f))));
	//glVertex2f (x + w, sdlu_mode.screen_h - y);
	glEnd ();
}


void draw_quad (float x, float y, float w, float h, rgb_float_t color) {
	glColor3f (color.r, color.g, color.b);

	glBegin (GL_QUADS);
		glVertex2f (x, y);
		glVertex2f (x, (y + h));
		glVertex2f (x + w, (y + h));
		glVertex2f (x + w, y);
	glEnd ();
}


void draw_progress_bar (v2d_t tl, v2d_t br, double percent, rgb_float_t c1, rgb_float_t c2) {
	double midx; // middle top and bottom

	midx = tl.x + ((br.x - tl.x) * percent);

	glColor3f (c1.r, c1.g, c1.b);

	glBegin (GL_QUADS);
		glVertex2d (tl.x, br.y);
		glVertex2d (tl.x, tl.y);
		glVertex2d (midx, tl.y);
		glVertex2d (midx, br.y);
	glEnd ();

	glColor3f (c2.r, c2.g, c2.b);

	glBegin (GL_QUADS);
		glVertex2d (midx, br.y);
		glVertex2d (midx, tl.y);
		glVertex2d (br.x, tl.y);
		glVertex2d (br.x, br.y);
	glEnd ();
}


void draw_line (v2d_t a, v2d_t b, rgb_float_t color) {
	glLineWidth (1.0f);
	glDisable (GL_LINE_SMOOTH);	// Disable Antialiasing

	glColor3f (color.r, color.g, color.b);

	glBegin (GL_LINES);
		glVertex2d (a.x, a.y);
		glVertex2d (b.x, b.y);
	glEnd ();
}


/*void draw_img_buf (char *data) {
	int off = 0, off2 = 0;
	for (int j = 0; j < 240; j++) {
		for (int i = 0; i < 320; i++) {
			off = (j * VID_BUF_SIZE * 3) + (i * 3);
			vid_buf[off++] = data[off2];
			vid_buf[off++] = data[off2];
			vid_buf[off++] = data[off2++];
		}
	}

	glDeleteTextures (1, &texture2);
	glGenTextures (1, &texture2);
	glBindTexture (GL_TEXTURE_2D, texture2);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, 3, VID_BUF_SIZE, VID_BUF_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, vid_buf);
	
	draw_texture_jb (texture2, 0, 0, VID_BUF_SIZE, VID_BUF_SIZE);
}*/


void draw_texture (GLuint tex, int left, int top, int right, int bottom) {
	glBindTexture (GL_TEXTURE_2D, tex);

	glBegin (GL_QUADS);
		// bottom-left vertex (corner)
		glTexCoord2f (0, 0);
		glVertex3f ((GLfloat)left, (GLfloat)(sdlu_mode.screen_h - top), 0);

		// Bottom-right vertex (corner)
		glTexCoord2f (1, 0);
		glVertex3f ((GLfloat)right, (GLfloat)(sdlu_mode.screen_h - top), 0);

		// top-right vertex (corner)
		glTexCoord2f (1, 1);
		glVertex3f ((GLfloat)right, (GLfloat)(sdlu_mode.screen_h - bottom), 0);

		// Top-left vertex (corner)
		glTexCoord2f (0, 1);
		glVertex3f ((GLfloat)left, (GLfloat)(sdlu_mode.screen_h - bottom), 0);
	glEnd();
}


int load_texture (char *filename, GLuint *texture_handle) {
	SDL_Surface *surface; // Gives us the information to make the texture

	if ((surface = SDL_LoadBMP (filename))) { 
		// Check that the image's width is a power of 2
		if ((surface->w & (surface->w - 1)) != 0) {
			printf("warning: %s's width is not a power of 2\n", filename);
		}

		// Also check if the height is a power of 2
		if ((surface->h & (surface->h - 1)) != 0) {
			printf("warning: %s's height is not a power of 2\n", filename);
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures (1, texture_handle);

		// Bind the texture object
		glBindTexture (GL_TEXTURE_2D, *texture_handle);

		// Set the texture's stretching properties
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D (GL_TEXTURE_2D, 0, 3, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
	}
	else {
//        printf ("SDL could not load image.bmp: %s\n", SDL_GetError());
//        SDL_Quit ();
		return 1;
	}

	// Free the SDL_Surface only if it was successfully created
	if (surface) { 
		SDL_FreeSurface (surface);
	}

	return 0;
}


int init_sdl (sdl_mode_info_t mode) {
	SDL_Surface *screen;

	sdlu_mode = mode;
		
	if (SDL_Init (SDL_INIT_EVERYTHING) != 0) {
		printf ("Unable to initialize SDL: %s\n", SDL_GetError ());
		return 1;
	}

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	if (mode.fullscreen)
		screen = SDL_SetVideoMode (mode.screen_w, mode.screen_h, 32, SDL_OPENGL | SDL_FULLSCREEN);
	else
		screen = SDL_SetVideoMode (mode.screen_w, mode.screen_h, 32, SDL_OPENGL); // *changed*
	
	if (!screen) {
		printf ("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	SDL_ShowCursor (0); // hide the pesky mouse cursor

	// Set the OpenGL state after creating the context with SDL_SetVideoMode
	glClearColor (0, 0, 0, 0);
	
	glEnable (GL_TEXTURE_2D | GL_BLEND); // Need this to display a texture

	glViewport (0, 0, mode.screen_w, mode.screen_h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	glOrtho (0, mode.screen_w, 0, mode.screen_h, -1, 1);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	
	sdlu_mode.initialized = 1;
	
	return 0;
}


int deinit_sdl (void) {
	SDL_Quit ();

	return 0;
}

#endif
