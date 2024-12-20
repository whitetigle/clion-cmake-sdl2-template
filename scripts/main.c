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

vec3_t camera_position = {0,0,0};

// Field of View
float fov_Factor = 640;

bool isRunning;

uint64_t previous_frame_time = 0;

//#define ASSETS_PATH "${ASSETS_PATH}"
enum cull_method {
    CULL_NONE,
    CULL_BACKFACE
} cull_method; // kind of global variable

enum render_method {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
} render_method; // kind of global variable


bool setup(void) {

    // initialize render_mode
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

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

    load_cube_mesh_data();
//    char result[100];
//    sprintf(result, "%scube.obj",ASSETS_PATH);
//    sprintf(result, "%sf22.obj",ASSETS_PATH);
//    char* name = result;
//    load_obj_file_data(name);

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
            if(event.key.keysym.sym == SDLK_1)
                    render_method = RENDER_WIRE_VERTEX;
            if(event.key.keysym.sym == SDLK_2)
                render_method = RENDER_WIRE;
            if(event.key.keysym.sym == SDLK_3)
                render_method = RENDER_FILL_TRIANGLE;
            if(event.key.keysym.sym == SDLK_4)
                render_method = RENDER_FILL_TRIANGLE_WIRE;
            if(event.key.keysym.sym == SDLK_c)
                cull_method = CULL_BACKFACE;
            if(event.key.keysym.sym == SDLK_d)
                cull_method = CULL_NONE;
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

    mesh.rotation.y += 0.01f; // rotation by Y
    mesh.rotation.x += 0.01f; // rotation by Y
    mesh.rotation.z += 0.01f; // rotation by Y

    // loop all triangles faces of our mesh
    int num_faces = array_length(mesh.faces);
    for(int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];
        vec3_t face_vertices[3];

        // for each face vertex index, get actual vertex value
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        // for each vertex value (=a 3D point), project and get the projected result as a triangle_t
        for(int j=0; j<3;j++) {

            // apply rotation
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate the vertex away from the camera in z
            // pushing everything INSIDE the monitor
            transformed_vertex.z += 5;

            // Save transformed vertex in the array of transformed vertices
            // for later usage
            transformed_vertices[j] = transformed_vertex;
        }

        // check backface culling: do we need to display the face?
        // prepare the vectors for the triangle
        vec3_t vector_a = transformed_vertices[0]; //   A
        vec3_t vector_b = transformed_vertices[1]; /*  / \ */
        vec3_t vector_c = transformed_vertices[2]; // C _ B

        // Step 1 : find B-A and C-A
        vec3_t vector_ab = vec3_sub(vector_b,vector_a); // B - A
        vec3_t vector_ac = vec3_sub(vector_c,vector_a); // C - A

        // normalize: we don't need the actual values
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ac);

        // Step 2: find Face Normal using cross-product
        // it depends on handedness. Going inside the monitor or outside?
        // here we use a left-handed, z will go into the screen
        // the order is then
        vec3_t normal = vec3_cross(vector_ab, vector_ac);

        /*
         * otherwise, if it was right-handed, pointing to us like DirectX,
         * it would be
         * vec3_t normal = vec3_cross(vector_ac, vector_ab);
        * */

        // Normalize the face normal
        // since we just need to know the direction
        vec3_normalize(&normal);

        // Step 3: find the camera ray
        // Find the vector between a point in the triangle and the camera origin
        // by subtracting the camera position from Point A
        vec3_t camera_ray = vec3_sub(camera_position,vector_a);

        // Step 4:
        // Calculate how aligned the camera ray is with the face normal (using dot-product)
        float dot_normal_camera = vec3_dot(normal, camera_ray);

        // do we display the face?
        // Not displayed? don't project vertices
        if(dot_normal_camera < 0) continue;

        vec2_t projected_points[3];

        // loop all vertices to perform projection
        for(int j=0; j<3;j++) {

            // project on our 2D plane (screen/flatten)
            projected_points[j] = project(transformed_vertices[j]);

            // --- test of orthographic projection. Uncomment to play with
//            vec2_t projected_point = project_orthographic(transformed_vertex);
            // ---

            // scale and translate at the middle of the screen
            projected_points[j].x += window_width * 0.5;
            projected_points[j].y += window_height * 0.5;

        }

        // calculate the avg depth for each face based on their vertices after transform
        float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[0].z ) / 3.0;

        // Add color from mesh
        triangle_t projected_triangle = {
                .points = {
                        {projected_points[0].x,projected_points[0].y},
                        {projected_points[1].x,projected_points[1].y},
                        {projected_points[2].x,projected_points[2].y}
                },
                .color = mesh_face.color,
                .avg_depth =avg_depth
        };

        // store the result for the current face as a proper projected triangle_t
        array_push(triangles_to_render, projected_triangle);
    }

    // Sort the triangles to render by their avg_depth
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++) {
        for (int j = i; j < num_triangles; j++) {
            if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
                // Swap the triangles positions in the array
                triangle_t temp = triangles_to_render[i];
                triangles_to_render[i] = triangles_to_render[j];
                triangles_to_render[j] = temp;
            }
        }
    }
}

void render(void) {
//    draw_grid();
    draw_grid_points();

    // render faces
    int number_triangles = array_length(triangles_to_render);
    for(int i =0;i < number_triangles;i++) {
        triangle_t triangle = triangles_to_render[i];

        if(render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_filled_triangle(
                    triangle.points[0].x, triangle.points[0].y, // vertex A
                    triangle.points[1].x, triangle.points[1].y, // vertex B
                    triangle.points[2].x, triangle.points[2].y, // vertex C
                    triangle.color
            );
        }

        if(render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE ){
            draw_triangle(
                    triangle.points[0].x, triangle.points[0].y, // vertex A
                    triangle.points[1].x, triangle.points[1].y, // vertex B
                    triangle.points[2].x, triangle.points[2].y, // vertex C
                    triangle.color
            );
        }

        // draw the 3 vertices of the triangle
        if(render_method == RENDER_WIRE_VERTEX) {
            for(int j=0;j<3;j++) {
                vec2_t point = triangle.points[j];
                draw_rect(
                        point.x,
                        point.y,
                        4,
                        4,
                        0xFFFFFF00);
            }
        }

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


