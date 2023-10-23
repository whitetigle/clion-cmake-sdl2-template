//
// Created by conta on 10/18/2023.
//
#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "display/display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

//triangle_t triangles_to_render[N_CUBE_FACES];
triangle_t * triangles_to_render = NULL;

vec3_t camera_position = {0,0,-5};

// Field of View
float fov_Factor = 640;

bool isRunning;

uint64_t previous_frame_time = 0;

//#define ASSETS_PATH "${ASSETS_PATH}"

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

//    load_cube_mesh_data();
    char result[100];
    sprintf(result, "%sf22.obj",ASSETS_PATH);
    char* name = result;
    load_obj_file_data(name);

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

// this is just a test. All the faces will have the same size, no perspective at all
// it could be useful in isometric projects
vec2_t project_orthographic(vec3_t point) {
    int fov = 128;
    vec2_t  projected_point = {
            .x=(fov * point.x),
            .y=(fov * point.y)
    };

    return projected_point;
}

void update(void) {

    // while not passed lock the update (manual version)
    while(!SDL_TICKS_PASSED(SDL_GetTicks64(),previous_frame_time + FRAME_TARGET_TIME));

    // wait before update (multicore)
    uint64_t time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks64() - previous_frame_time);
    if(time_to_wait > 0 && time_to_wait < FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks64();

    // initialize the array of triangles to render
    triangles_to_render = NULL;

    mesh.rotation.y += 0.00f; // rotation by Y
    mesh.rotation.x += 0.01f; // rotation by Y
    mesh.rotation.z += 0.00f; // rotation by Y

    // loop all triangles faces of our mesh
    int num_faces = array_length(mesh.faces);
    for(int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];
        vec3_t face_vertices[3];

        // for each face vertex index, get actual vertex value
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        triangle_t projected_triangle;

        // for each vertex value (=a 3D point), project and get the projected result as a triangle_t
        for(int j=0; j<3;j++) {

            // apply rotation
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate the vertex away from the camera in z
            transformed_vertex.z -= camera_position.z;

            // project on our 2D plane (screen/flatten)
            vec2_t projected_point = project(transformed_vertex);

            // --- test of orthographic projection. Uncomment to play with
//            vec2_t projected_point = project_orthographic(transformed_vertex);
            // ---

            // scale and translate at the middle of the screen
            projected_point.x += window_width * 0.5;
            projected_point.y += window_height * 0.5;

            // store result for the current vertex
            projected_triangle.points[j] = projected_point;
        }

        // store the result for the current face as a proper projected triangle_t
//        triangles_to_render[i] = projected_triangle;
        array_push(triangles_to_render, projected_triangle);
    }


}

void render(void) {
//    draw_grid();
    draw_grid_points();

    // render faces
    int number_triangles = array_length(triangles_to_render);
    for(int i =0;i < number_triangles;i++) {
        triangle_t triangle = triangles_to_render[i];

        // draw the 3 vertices of the triangle
        for(int j=0;j<3;j++) {
            vec2_t point = triangle.points[j];
            draw_rect(
                    point.x,
                    point.y,
                    4,
                    4,
                    0xFFFFFF00);
        }

        vec2_t p1 = triangle.points[0];
        vec2_t p2 = triangle.points[1];
        vec2_t p3 = triangle.points[2];
        draw_line_vec2(p1,p2,0xFFFF00FF);
        draw_line_vec2(p2,p3,0xFFFF00FF);
        draw_line_vec2(p3,p1,0xFFFF00FF);
    }

//    draw_pixel(20,20, 0xFF00FF00);
//    draw_rect(1230,670,100,100,0xFFFF0000);
    // clear the array of triangles every frame loop
    array_free(triangles_to_render);

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}

void free_resources(void) {
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
}

void cleanup(void) {
    free_resources();
    destroy_window();
}

int main(int argc, char* args []) {
    isRunning = initialize_window();

    if( !setup()) {
        cleanup();
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


