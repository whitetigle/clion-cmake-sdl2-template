//
// Created by conta on 10/19/2023.
//

#include "mesh.h"

vec3_t mesh_vertices[N_MESH_VERTICES] = {
        {-1,-1,-1},
        {-1,1,-1},
        {1,1,-1},
        {1,-1,-1},
        {1,1,1},
        {1,-1,1},
        {-1,1,1},
        {-1,-1,1},
};

// array of indexes that refer to mesh_vertices
// order IS important!
// Note: index starts from 1 no 0
face_t mesh_faces[N_MESH_FACES] = {
        // front
        {1,2,3},
        {1,3,4},
        // right
        {4,3,5},
        {4,5,6},
        // back
        {6,5,7},
        {6,7,8},
        // left
        {8,7,2},
        {8,2,1},
        // top
        {2,7,5},
        {2,5,3},
        // bottom
        {6,8,1},
        {6,1,4}
};
