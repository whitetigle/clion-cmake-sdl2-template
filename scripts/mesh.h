//
// Created by conta on 10/19/2023.
//

#ifndef PROJECT_NAME_MESH_H
#define PROJECT_NAME_MESH_H

#include "vector.h"
#include "triangle.h"

// cube mesh
#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern vec3_t cube_vertices[N_CUBE_VERTICES] ;
extern face_t cube_faces[N_CUBE_FACES];

// dynamic size meshes
typedef struct {
    vec3_t* vertices; // dynamic array of vertices
    face_t* faces;
    vec3_t rotation;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);

#endif //PROJECT_NAME_MESH_H
