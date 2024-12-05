//
// Created by conta on 10/19/2023.
//

#include <stdio.h>
#include "mesh.h"
#include "array.h"
#include "SDL.h"
#include <ctype.h>

// initialize global mesh
mesh_t mesh ={
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0}
};


vec3_t cube_vertices[N_CUBE_VERTICES] = {
        {-1,-1,-1},
        {-1,1,-1},
        {1,1,-1},
        {1,-1,-1},
        {1,1,1},
        {1,-1,1},
        {-1,1,1},
        {-1,-1,1},
};

// array of indexes that refer to cube_vertices
// order IS important!
// Note: index starts from 1 no 0
face_t cube_faces[N_CUBE_FACES] = {
        // front
        {1,2,3, 0xFFFF0000},
        {1,3,4, 0xFFFF0000},
        // right
        {4,3,5, 0xFF00FF00},
        {4,5,6, 0xFF00FF00},
        // back
        {6,5,7, 0xFF0000FF},
        {6,7,8, 0xFF0000FF},
        // left
        {8,7,2, 0xFFFFFF00},
        {8,2,1, 0xFFFFFF00},
        // top
        {2,7,5, 0xFFFF00FF},
        {2,5,3, 0xFFFF00FF},
        // bottom
        {6,8,1, 0xFF000FFF},
        {6,1,4, 0xFF000FFF}
};

void load_cube_mesh_data(void) {
    for(int i =0;i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    for(int i =0;i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void removeExtraSpaces(char *str) {
    int i, j;
    int spaceFlag = 0;  // Flag to track consecutive spaces

    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            // Copy non-space characters to the result string
            str[j++] = str[i];
            spaceFlag = 0;
        } else if (spaceFlag == 0) {
            // Copy the first space encountered
            str[j++] = ' ';
            spaceFlag = 1;
        }
    }

    // Null-terminate the result string
    str[j] = '\0';
}

void load_obj_file_data(char* filename) {

    SDL_Log("opening %s\n", filename);

    FILE *filePointer;
    char line[1024];
    filePointer = fopen(filename, "r");

    if (filePointer == NULL) {

        // if we can't open the file, just display the standard cube
        for(int i =0;i < N_CUBE_VERTICES; i++) {
            vec3_t cube_vertex = cube_vertices[i];
            array_push(mesh.vertices, cube_vertex);
        }
        for(int i =0;i < N_CUBE_FACES; i++) {
            face_t cube_face = cube_faces[i];
            array_push(mesh.faces, cube_face);
        }

        SDL_LogError(0, "Error opening the file");
//        perror("Error opening the file");
        return;
    }

    while (fgets(line, sizeof(line), filePointer) != NULL) {

        removeExtraSpaces(line);

            SDL_Log("line=%s\n", line);

            if(line[0] == 'v') {
                char v[3];
                double num1, num2, num3;
                if (sscanf(line, "%s %lf %lf %lf", v, &num1, &num2, &num3) == 4) {
                    SDL_Log("Parsed values:\n");
                    SDL_Log("v: %s\n", v);
                    SDL_Log("Number 1: %lf\n", num1);
                    SDL_Log("Number 2: %lf\n", num2);
                    SDL_Log("Number 3: %lf\n", num3);
                    vec3_t cube_vertex = {
                            .x = num1,
                            .y = num2,
                            .z = num3
                    };
                    array_push(mesh.vertices, cube_vertex);

                } else {
                    printf("Invalid input format.\n");
                    continue;
                }
                continue;
            }

            if(line[0] == 'f') {
                int vertex_indices[3];
                int texture_indices[3];
                int normal_indices[3];
                if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                           &vertex_indices[0], &texture_indices[1], &normal_indices[1],
                           &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                           &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                           ) == 9) {
                    face_t cube_face = {
                            .a = vertex_indices[0] ,
                            .b = vertex_indices[1],
                            .c = vertex_indices[2],
                    };
                    array_push(mesh.faces, cube_face);
                } else {
                    printf("Invalid input format.\n");
                    continue;
                }
                continue;
            }

        }

    fclose(filePointer);
}