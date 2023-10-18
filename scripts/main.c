//
// Created by conta on 10/18/2023.
//
#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "display/display.h"
#include "vector.h"

// array of vectors/points
#define N_POINTS (9 *9 *9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {0,0,-5};
// Field of View
float fov_Factor = 640;

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

    int point_count = 0;
    // start loading array of vectors
    // from -1 to 1
    for(float x = -1;x<=1; x += 0.25){
        for(float y = -1;y<=1; y += 0.25){
            for(float z = -1;z<=1; z += 0.25){
                vec3_t new_point = {.x=x, .y=y, .z=z};
                cube_points[point_count++] = new_point;
            }
        }
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

// 3D vector to projected 2D point
vec2_t project(vec3_t point) {
    vec2_t  projected_point = {
      .x=(fov_Factor * point.x) / point.z,
      .y=(fov_Factor * point.y) / point.z
    };

    return projected_point;
}

void update(void) {
    for(int i = 0;i < N_POINTS; i++) {
        vec3_t point = cube_points[i];

        // move the point away from the camera
        point.z -= camera_position.z;

        // project the point
        vec2_t projected_point = project(point);

        // save the projected 2D point
        projected_points[i] = projected_point;
    }
}

void render(void) {
//    SDL_SetRenderDrawColor(renderer, 100, 100, 180, 255); // Set the background color to purple
//    SDL_RenderClear(renderer);
//    draw_grid();
    draw_grid_points();
    // loop all projected and render them
    for(int i =0;i < N_POINTS;i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(
                projected_point.x + window_width * 0.5,
                projected_point.y + window_height * 0.5,
                4,
                4,
                0xFFFFFF00);
    }

//    draw_pixel(20,20, 0xFF00FF00);
//    draw_rect(1230,670,100,100,0xFFFF0000);
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


