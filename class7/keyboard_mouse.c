#include <GL/glut.h>
#include <stdio.h>

#if !defined(GLUT_WHEEL_UP)
#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4
#endif

unsigned w = 400;  // window width
unsigned h = 400;  // window height
float    u = 0.0;  // x origin of triangle
float    v = 0.0;  // y origin of triangle
float    s = 1.0;  // scale
int  clear = 1;    // clear buffer or not

void keyboardFunc(unsigned char key, int x, int y) {  // *** key
  switch(key) {
  case  27: exit(1);                              break;  // Esc
  case 'i': if (s < 8.95) s *= 1.1;               break;  // zoom in
  case 'o': if (s > 0.03) s /= 1.1;               break;  // zoom out
  case 'r': u = 0.0; v = 0.0; s = 1.0; clear = 1; break;  // reset
  case 'c': clear ^= 1;                           break;} // toggle clear
  glutPostRedisplay(); // re-display
}

void specialKeyboardfunc(int key, int x, int y) {     // *** special key
  switch(key) { // arrow keys
  case GLUT_KEY_LEFT:  if (u > -1.0) u -= 0.05; break;
  case GLUT_KEY_DOWN:  if (v > -1.0) v -= 0.05; break;
  case GLUT_KEY_RIGHT: if (u < +1.0) u += 0.05; break;
  case GLUT_KEY_UP:    if (v < +1.0) v += 0.05; break;}
  glutPostRedisplay(); // re-display
}

void mouseFunc(int button, int state, int x, int y) { // *** mouse button
  printf("mouse pointer: %4d %4d\r", x, y); fflush(stdout);
  float uu = 2.0 * ((float)x - (float)w/2.0) / (float)w;
  float vv = 2.0 * ((float)y - (float)h/2.0) / (float)h;
  switch(button) {
  case GLUT_LEFT_BUTTON:  if (state == GLUT_DOWN) {u = +uu; v = -vv;}          break;  // left  button
  case GLUT_RIGHT_BUTTON: if (state == GLUT_DOWN) {u = 0.0; v = 0.0; s = 1.0;} break;  // right button
  case GLUT_WHEEL_UP:     if (v < +1.0) v += 0.05;                             break;  // scroll up
  case GLUT_WHEEL_DOWN:   if (v > -1.0) v -= 0.05;                             break;} // scroll down
  glutPostRedisplay(); // re-display
}

void motionFunc(int x, int y) {                       // *** mouse motion with drag
  printf("mouse pointer: %4d %4d\r", x, y); fflush(stdout);
  u = +2.0 * ((float)x - (float)w/2.0) / (float)w;
  v = -2.0 * ((float)y - (float)h/2.0) / (float)h;
  glutPostRedisplay(); // re-display
}

void passiveMotionFunc(int x, int y) {                // *** mouse motion without drag
  printf("mouse pointer: %4d %4d\r", x, y); fflush(stdout);
}

void display(void) {
  if (clear) {
    glClearColor(0.0, 0.0, 0.0, 1.0);           // clear color r, g, b, and alpha
    glClear(GL_COLOR_BUFFER_BIT);               // clear buffer
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // fill
  glBegin(GL_TRIANGLES);                        // each set of 3 vertices form a triangle
  glColor3f(1.0f, 0.0f, 0.0f);                  // red
  glVertex2f(u-0.3*s, v+0.2*s);                 // vertex 1
  glColor3f(0.0f, 1.0f, 0.0f);                  // green
  glVertex2f(u+0.3*s, v+0.2*s);                 // vertex 2
  glColor3f(0.0f, 0.0f, 1.0f);                  // blue
  glVertex2f(u,       v-0.3*s);                 // vertex 3
  glEnd();
  glutSwapBuffers();                            // swap buffers of the current window
}

void main(int argc, char * argv[]) {
  printf("--------------------------+-----------------------------------\n");
  printf("keys:                 Esc - exit\n");
  printf("                        i - zoom in\n");
  printf("                        o - zoom out\n");
  printf("                        r - reset\n");
  printf("                        c - toggle clear buffer\n");
  printf("                   arrows - move triangle\n");
  printf("--------------------------+-----------------------------------\n");
  printf("mouse: left  button click - move triangle to the pointer place\n");
  printf("       right button click - move triangle to the window center\n");
  printf("       scroll wheel roll  - move triangle up / down\n");
  printf("       button drag        - move triangle\n");
  printf("--------------------------+-----------------------------------\n");
  glutInitWindowSize(w, h);                 // set the initial window size
  glutInitWindowPosition(100, 100);             // set the initial window position
  glutInit(&argc, argv);                        // initialize the GLUT library
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);  // set the display mode to RGB, enable double buffer
  glutCreateWindow("Keyboard Mouse Functions"); // create a top-level window
  glutMouseFunc(mouseFunc);                     // set the mouse function - called on mouse stuff
  glutMotionFunc(motionFunc);                   // set mouse motion function (drag)
  glutPassiveMotionFunc(passiveMotionFunc);     // set mouse motion function
  glutKeyboardFunc(keyboardFunc);               // set the keyboard function - called on keyboard events
  glutSpecialFunc(specialKeyboardfunc);         // set the keyboard function - called on keyboard events
  glutDisplayFunc(display);                     // set the display callback for the current window
  glutMainLoop();                               // enter the GLUT event processing loop
}

