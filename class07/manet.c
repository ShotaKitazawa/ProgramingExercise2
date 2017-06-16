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
#include <unistd.h>  // for usleep()
#include <stdio.h>

#define no_nodes 150

typedef struct node {  // nodes
  float x;             // x coordinate
  float y;             // y coordinate
  int dx;              // x direction to move
  int dy;              // y direction to move
  int c;               // counter, keep same direction
} node;

node nodes[no_nodes];   // no_nodes nodes
double r = 0.3;         // radius of wave
double r_max = 1;
double r_min = 0.01;
double r_displacement = 0.05;
useconds_t ms = 10000;  // sleep 10000ms
useconds_t ms_max = 30000;
useconds_t ms_min = 1000;
useconds_t ms_displacement = 1000;

void idleFunc() {
  usleep(ms);           // suspend execution for microsecond intervals
  glutPostRedisplay();  // callback from GLUT, render the scene
}

void specialKeyboardfunc(int key, int x, int y) {     // *** special key
  switch(key) { // arrow keys
  case GLUT_KEY_LEFT:  if (ms < ms_max) ms += ms_displacement; break;
  case GLUT_KEY_DOWN:  if (r > r_min) r -= r_displacement; break;
  case GLUT_KEY_RIGHT: if (ms > ms_min) ms -= ms_displacement; break;
  case GLUT_KEY_UP:    if (r < r_max) r += r_displacement; break;}
  glutPostRedisplay(); // re-display
}

void display(void) {
  float n;    // how many times moving dx or dy
  int i, j, xy;  // xy: move x or y
  float dis; // nodes.x ^2 + nodes.y ^2

  glClearColor(0.0, 0.0, 0.0, 1.0);           // clear color r, g, b, and alpha
  glClear(GL_COLOR_BUFFER_BIT);               // clear buffer
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // fill

  for (i = 0; i < no_nodes; i++) {  // for all nodes
    xy = rand() % 2;                // move x or y
    n = (rand() % 101) /
        5000.0;  // move x by dx or y by dy (0 ~ 100) / 5000.0 times

    if (xy == 0) {                                // x
      nodes[i].x += n * nodes[i].dx;              // x movement
      if (nodes[i].x < -1.0) nodes[i].x = +0.99;  // join in opposite place
      if (nodes[i].x > +1.0) nodes[i].x = -0.99;  // join in opposite place
    } else {                                      // y
      nodes[i].y += n * nodes[i].dy;              // y movement
      if (nodes[i].y < -1.0) nodes[i].y = +0.99;  // join in opposite place
      if (nodes[i].y > +1.0) nodes[i].y = -0.99;  // join in opposite place
    }
    for (j = 0; j < no_nodes; j++){
      dis = (nodes[i].x-nodes[j].x)*(nodes[i].x-nodes[j].x) + (nodes[i].y-nodes[j].y)*(nodes[i].y-nodes[j].y);
      if (dis <= r*r){
        glColor4f(0.0, 1.0, 0.0, 1.0);  // set current color r, g, b, and alpha
        glBegin(GL_LINES);                // begin draw line
          glVertex2f(nodes[i].x, nodes[i].y);
          glVertex2f(nodes[j].x, nodes[j].y);
        glEnd();                          // end of draw line
      }
    }

    glColor4f(1.0, 0.0, 0.0, 1.0);  // set current color r, g, b, and alpha
    glRectf(nodes[i].x - 0.010, nodes[i].y - 0.010,   // draw (fill) node
            nodes[i].x + 0.010, nodes[i].y + 0.010);  // with little rectangle
    if (nodes[i].c-- == 0) {                          // counter--
      nodes[i].dx = rand() % 3 - 1;  // 0, 1, 2 - 1 = -1, 0, 1: direction
      nodes[i].dy = rand() % 3 - 1;  // 0, 1, 2 - 1 = -1, 0, 1: direction
      nodes[i].c = rand() % 100;     // 0 ~ 99 same direction
    }
  }

  glutSwapBuffers();  // swap buffers of the current window
}

void print_commands(){
  printf("--------------------------+-----------------------------------\n");
  printf("keys:   Esc         - exit\n");
  printf("        left key    - speed down\n");
  printf("        down key    - reduce circle\n");
  printf("        up key      - expand circle\n");
  printf("        right key   - speed up\n");
  printf("--------------------------+-----------------------------------\n");
}

void main(int argc, char* argv[]) {
  int i;
  print_commands();
  srand(time(NULL));                            // set seed for rand()
  for (i = 0; i < no_nodes; i++) {              // initialize all nodes
    nodes[i].x = ((rand() % 101) - 50) / 50.0;  // x of node i, x: [-1.0 ~ +1.0]
    nodes[i].y = ((rand() % 101) - 50) / 50.0;  // y of node i, y: [-1.0 ~ +1.0]
    nodes[i].dx = rand() % 3 - 1;  // 0, 1, 2 - 1 = -1, 0, 1: x direction
    nodes[i].dy = rand() % 3 - 1;  // 0, 1, 2 - 1 = -1, 0, 1: y direction
    nodes[i].c = rand() % 100;     // counter 0 ~ 99, keep same direction
  }
  glutInitWindowPosition(200, 200);  // set the initial window position
  glutInitWindowSize(400, 400);      // set the initial window size
  glutInit(&argc, argv);             // initialize the GLUT library
  glutInitDisplayMode(GLUT_DOUBLE);  // use double buffer
  glutInitDisplayMode(GLUT_RGB);     // set the initial display mode to RGB
  glutCreateWindow("Mobile Nodes");  // create a top-level window
  glutIdleFunc(idleFunc);            // set the idle function - called when idle
  glutSpecialFunc(specialKeyboardfunc);         // set the keyboard function - called on keyboard events
  glutDisplayFunc(display);  // set the display callback for the current window
  glutMainLoop();            // enter the GLUT event processing loop
}
