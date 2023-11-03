//
// Created by conta on 10/18/2023.
//

#include "SDL.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
color_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
    if( SDL_Init(SDL_INIT_EVERYTHING) !=0) {
        fprintf(stderr, "Error while initializing SDL.\n");
        return false;
    }

    // Use SDL to query what is the full screen max W and H
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, // first display
                              &displayMode
    );
    window_width = displayMode.w; // can use another resolution that will be scaled up, 800 for instance
    window_height = displayMode.h;

    // create SDL window
    window = SDL_CreateWindow(
            NULL, // No window title = no border
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width,
            window_height,
            SDL_WINDOW_BORDERLESS);

    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // No window! real Full Screen
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // create a SDL renderer
    return true;
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
            color_buffer_texture,
            NULL, // the whole
            color_buffer,
            (int) (window_width * (sizeof(color_t )))
    );

    SDL_RenderCopy(
            renderer,
            color_buffer_texture,
            NULL,
            NULL);
}

void clear_color_buffer(color_t color) {
    for(int y =0; y < window_height; y++) {
        for(int x =0; x < window_width; x++) {
            int row = window_width * y;
            int position = row + x;
            color_buffer[position] = color;
        }
    }
}

void draw_pixel(int x, int y, color_t color) {
    if(x >= window_width || y >= window_height) return;

    int row = window_width * y;
    int position = row + x;
    color_buffer[position] = color;
}

void draw_grid(void) {
    for(int y =0; y < window_height; y++) {
        for(int x =0; x < window_width; x++) {
            if(y%10==0) {
                draw_pixel(x,y,0xFFFFFFFF);
            }
            if(x%10==0) {
                draw_pixel(x,y,0xFFFFFFFF);
            }
        }
    }
}

void draw_grid_points(void) {
    for(int y =0; y < window_height; y += 10) {
        for(int x =0; x < window_width; x += 10) {
            draw_pixel(x,y,0xFF777777);
        }
    }
}

void draw_line_vec2(vec2_t p1, vec2_t p2, color_t color) {
    draw_line(p1.x,p1.y,p2.x,p2.y,color);
}

// DDA approach, slower than Bresenham (uses divisions and floats)
void draw_line(int x0, int y0, int x1, int y1, color_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int dx = abs(delta_x);
    int dy = abs(delta_y);

    // we take the longer delta for the side length,
    // because sometimes the slope is steeper
    // (height > width of a side of the triangle)
    int side_length = dx >= dy ? dx : dy;

    float x_inc = (float)delta_x / (float)side_length;
    float y_inc = (float)delta_y / (float)side_length;

    float current_x = (float)x0;
    float current_y = (float)y0;
    for( int i = 0; i < side_length; i++ ) {
        draw_pixel(round(current_x),round(current_y),color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_rect(int wx, int wy, int width, int height, color_t color) {

    if( wx < 0 || wy < 0 || width < 0 || height < 0) {
        return;
    }

    // check if we don't try to draw things out of screen
    if(width+wx >= window_width) {
        if( width >= window_width ) {
            width = window_width - wx;
        } else {
            width = window_width - width + wx;
        }
    }

    // check if we don't try to draw things out of screen
    if(height+wy >= window_height) {
        if( height > window_height ) {
            height = window_height - wy;
        } else {
            height = window_height - height + wy;
        }
    }

    for(int y =wy; y < wy+height; y ++) {
        for(int x =wx; x < wx+width; x++) {
            draw_pixel(x,y,color);
        }
    }
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

