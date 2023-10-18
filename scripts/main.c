//
// Created by conta on 10/18/2023.
//
#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

bool isRunning;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t * color_buffer;
SDL_Texture * color_buffer_texture;

int window_width = 800;
int window_height = 600;

void destroy_window();

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

void draw_grid(void) {
    for(int y =0; y < window_height; y++) {
        for(int x =0; x < window_width; x++) {
            int row = window_width * y;
            int position = row + x;
            if(y%10==0) {
                color_buffer[position] = 0xFFFFFFFF;
            }
            if(x%10==0) {
                color_buffer[position] = 0xFFFFFFFF;
            }
        }
    }
}

void draw_grid_points(void) {
    for(int y =0; y < window_height; y += 10) {
        for(int x =0; x < window_width; x += 10) {
            int row = window_width * y;
            int position = row + x;
            color_buffer[position] = 0xFF777777;
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
            int row = window_width * y;
            int position = row + x;
            color_buffer[position] = color;
        }
    }
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 180, 255); // Set the background color to purple
    SDL_RenderClear(renderer);

//    draw_grid();
    draw_grid_points();
    draw_rect(0,0,2560,900,0xFFFF0000);
    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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


