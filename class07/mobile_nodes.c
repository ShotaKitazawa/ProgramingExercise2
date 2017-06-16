/*
 * a mobile ad hoc network (mobile_nodes.c)
 */
/*
gcc mobile_nodes.c -o mobile_nodes -lglut -lGL
*/
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h> 
#include <time.h>
#include <unistd.h> // for usleep()
#include <stdio.h>

#define no_nodes 6

typedef struct node {                                // nodes
  float x;                                           // x coordinate
  float y;                                           // y coordinate
  int  dx;                                           // x direction to move
  int  dy;                                           // y direction to move
  int   c;                                           // counter, keep same direction
} node;

node nodes[no_nodes];                                // no_nodes nodes
double      r = 0.3;                                 // radius of wave
useconds_t ms = 10000;                               // sleep 10000ms

void idleFunc() {
  usleep(ms);                                        // suspend execution for microsecond intervals
  glutPostRedisplay();                               // callback from GLUT, render the scene
}

void display(void) {
  float  n;                                          // how many times moving dx or dy
  int    i, xy;                                      // xy: move x or y

  glClearColor(0.0, 0.0, 0.0, 1.0);                  // clear color r, g, b, and alpha
  glClear(GL_COLOR_BUFFER_BIT);                      // clear buffer
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);         // fill

  for (i = 0; i < no_nodes; i++) {                   // for all nodes
    xy = rand() % 2;                                 // move x or y
    n = (rand() % 101) / 5000.0;                     // move x by dx or y by dy (0 ~ 100) / 5000.0 times

    if (xy == 0) {                                   // x
      nodes[i].x += n * nodes[i].dx;                 // x movement
      if (nodes[i].x < -1.0) nodes[i].x = +0.99;     // join in opposite place
      if (nodes[i].x > +1.0) nodes[i].x = -0.99;     // join in opposite place
    } else {                                         // y
      nodes[i].y += n * nodes[i].dy;                 // y movement
      if (nodes[i].y < -1.0) nodes[i].y = +0.99;     // join in opposite place
      if (nodes[i].y > +1.0) nodes[i].y = -0.99;     // join in opposite place
    }

    glColor4f(1.0, 0.0, 0.0, 1.0);                   // set current color r, g, b, and alpha
    glRectf(nodes[i].x - 0.015, nodes[i].y - 0.015,  // draw (fill) node
      nodes[i].x + 0.015, nodes[i].y + 0.015); // with little rectangle
    if (nodes[i].c-- == 0) {                         // counter--
      nodes[i].dx = rand() % 3 - 1;                  // 0, 1, 2 - 1 = -1, 0, 1: direction
      nodes[i].dy = rand() % 3 - 1;                  // 0, 1, 2 - 1 = -1, 0, 1: direction
      nodes[i].c  = rand() % 100;                    // 0 ~ 99 same direction
     }
  }

  glutSwapBuffers();                                 // swap buffers of the current window
}

void main(int argc, char * argv[]) {
  int i;
  srand(time(NULL));                                 // set seed for rand()
  for (i = 0; i < no_nodes; i++) {                   // initialize all nodes
    nodes[i].x  = ((rand() % 101) - 50) / 50.0;      // x of node i, x: [-1.0 ~ +1.0]
    nodes[i].y  = ((rand() % 101) - 50) / 50.0;      // y of node i, y: [-1.0 ~ +1.0]
    nodes[i].dx =   rand() % 3 - 1;                  // 0, 1, 2 - 1 = -1, 0, 1: x direction
    nodes[i].dy =   rand() % 3 - 1;                  // 0, 1, 2 - 1 = -1, 0, 1: y direction
    nodes[i].c  =   rand() % 100;                    // counter 0 ~ 99, keep same direction
  }
  glutInitWindowPosition(200, 200);                  // set the initial window position
  glutInitWindowSize(400, 400);                      // set the initial window size
  glutInit(&argc, argv);                             // initialize the GLUT library
  glutInitDisplayMode(GLUT_DOUBLE);                  // use double buffer
  glutInitDisplayMode(GLUT_RGB);                     // set the initial display mode to RGB
  glutCreateWindow("Mobile Nodes");                  // create a top-level window
  glutIdleFunc(idleFunc);                            // set the idle function - called when idle
  glutDisplayFunc(display);                          // set the display callback for the current window
  glutMainLoop();                                    // enter the GLUT event processing loop
}
