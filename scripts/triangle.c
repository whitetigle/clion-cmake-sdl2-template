//
// Created by conta on 10/19/2023.
//

#include "triangle.h"
#include "display/display.h"

void int_swap(int * a, int * b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat bottom
///////////////////////////////////////////////////////////////////////////////
//
//        (x0,y0)
//          / \
//         /   \
//        /     \
//       /       \
//      /         \
//  (x1,y1)------(x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

    // start from the top vertex
    float x_start = x0;
    float x_end = x0;

    // loop the scanlines from top to bottom
    for(int y = y0; y <= y2; y++) {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled a triangle with a flat top
///////////////////////////////////////////////////////////////////////////////
//
//  (x0,y0)------(x1,y1)
//      \         /
//       \       /
//        \     /
//         \   /
//          \ /
//        (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Find the two slopes (two triangle legs)
    float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

    // Start x_start and x_end from the bottom vertex (x2,y2)
    float x_start = x2;
    float x_end = x2;

    // Loop all the scanlines from bottom to top
    for (int y = y2; y >= y0; y--) {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw a filled triangle with the flat-top/flat-bottom method
// We split the original triangle in two, half flat-bottom and half flat-top
///////////////////////////////////////////////////////////////////////////////
//
//          (x0,y0)
//            / \
//           /   \
//          /     \
//         /       \
//        /         \
//   (x1,y1)------(Mx,My)
//       \_           \
//          \_         \
//             \_       \
//                \_     \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                         (x2,y2)
//
///////////////////////////////////////////////////////////////////////////////
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color){

    // Step 1: sort the vertices
    if(y0>y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if(y1>y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if(y0>y1) { // may have been swapped, ensure order is respected
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if(y1 ==y2) { // there's no bottom, so just draw the flat bottom triangle passing the raw vertices
        fill_flat_bottom_triangle(x0,y0,x1,y1,x2,y2,color);
        return;
    }

    if(y0 == y1) { // same problem but reversed
        fill_flat_top_triangle(x0,y0,x1,y1,x2,y2,color);
        return;
    }

    // Step 2: Calculate the new vertex (Mx,My)
    int My = y1;
    int Mx = ((float)((x2-x0) * (float)(y1 - y0))) / (float)(y2 - y0) + x0;

    // Step 3: Draw flat-bottom triangle
    fill_flat_bottom_triangle(x0,y0,x1,y1,Mx,My, color);

    // Step 4: Draw flat-bottom triangle
    fill_flat_top_triangle(x1,y1,Mx,My,x2,y2, color);
}
