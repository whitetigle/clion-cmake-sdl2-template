//
// Created by conta on 10/18/2023.
//
#ifndef PROJECT_NAME_DISPLAY_H
#define PROJECT_NAME_DISPLAY_H

#include "SDL.h"
#include <stdbool.h>
#include <stdint.h>

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

#endif //PROJECT_NAME_DISPLAY_H
