//
// Created by conta on 10/18/2023.
//
#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "display/display.h"

bool isRunning;

bool setup(void) {

    // allocate the required memory in bytes to hold the color buffer
    size_t s32 = sizeof(uint32_t);
    color_buffer = (uint32_t *) malloc(s32 * window_width * window_height);
    if(!color_buffer) {
        return false;
    }
    // creating a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width,
            window_height
            );

    if(!color_buffer_texture) {
        return false;
    }

    return true;
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT :
            isRunning = false;
            break;
        case SDL_KEYDOWN :
            if(event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN :
            isRunning = false;
            break;
    }
}

void update(void) {

}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 180, 255); // Set the background color to purple
    SDL_RenderClear(renderer);

//    draw_grid();
    draw_grid_points();
    draw_rect(1230,670,100,100,0xFFFF0000);
    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void cleanup(void) {
    free(color_buffer);
    destroy_window();
}

int main(int argc, char* args []) {
    isRunning = initialize_window();

    if( !setup()) {
        destroy_window();
        return 1;
    }

    while (isRunning) {
        process_input();
        update();
        render();
    }

    cleanup();
    return 0;
}


