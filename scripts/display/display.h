//
// Created by conta on 10/18/2023.
//
#ifndef PROJECT_NAME_DISPLAY_H
#define PROJECT_NAME_DISPLAY_H

#include "SDL.h"
#include "../vector.h"
#include <stdbool.h>
#include <stdint.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t * color_buffer;
extern SDL_Texture * color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(void);
void draw_grid_points(void);
void draw_pixel(int x,int y, uint32_t color);
void draw_rect(int wx, int wy, int width, int height, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_vec2(vec2_t p1, vec2_t p2, uint32_t color);

#endif //PROJECT_NAME_DISPLAY_H
