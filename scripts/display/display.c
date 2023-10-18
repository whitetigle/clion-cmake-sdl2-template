//
// Created by conta on 10/18/2023.
//

#include "SDL.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
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
            (int) (window_width * (sizeof(uint32_t )))
    );

    SDL_RenderCopy(
            renderer,
            color_buffer_texture,
            NULL,
            NULL);
}

void clear_color_buffer(uint32_t color) {
    for(int y =0; y < window_height; y++) {
        for(int x =0; x < window_width; x++) {
            int row = window_width * y;
            int position = row + x;
            color_buffer[position] = color;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if(x >= window_width || y >= window_height) return;

    int row = window_width * y;
    int position = row + x;
    color_buffer[position] = color;
}

void draw_grid(void) {
    for(int y =0; y < window_height; y++) {
        for(int x =0; x < window_width; x++) {
//            int row = window_width * y;
//            int position = row + x;
            if(y%10==0) {
                draw_pixel(x,y,0xFFFFFFFF);
//                color_buffer[position] = 0xFFFFFFFF;
            }
            if(x%10==0) {
                draw_pixel(x,y,0xFFFFFFFF);
//                color_buffer[position] = 0xFFFFFFFF;
            }
        }
    }
}

void draw_grid_points(void) {
    for(int y =0; y < window_height; y += 10) {
        for(int x =0; x < window_width; x += 10) {
            draw_pixel(x,y,0xFF777777);
//            int row = window_width * y;
//            int position = row + x;
//            color_buffer[position] = 0xFF777777;
        }
    }
}

void draw_rect(int wx, int wy, int width, int height, uint32_t color) {

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
//            int row = window_width * y;
//            int position = row + x;
//            color_buffer[position] = color;
        }
    }
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

