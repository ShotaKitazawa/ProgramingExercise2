#include <GL/glut.h>

void display(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0);          // clear color r, g, b, and alpha
  glClear(GL_COLOR_BUFFER_BIT);              // clear buffer
  // draw rectangle - left up cyan color
  glColor4f(0.0, 1.0, 1.0, 1.0);             // set current color r, g, b, and alpha
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw
  glRectf(-0.8, +0.8, -0.2, +0.2);           // draw rectangle - left up
  // fill rectangle - right up magenta color
  glColor4f(1.0, 0.0, 1.0, 1.0);             // set current color r, g, b, and alpha
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill
  glBegin(GL_QUADS);                         // fill rectangle - right up
    glVertex2f(+0.2, +0.8);                  // point 0
    glVertex2f(+0.8, +0.8);                  // point 1
    glVertex2f(+0.8, +0.2);                  // point 2
    glVertex2f(+0.2, +0.2);                  // point 3
  glEnd();
  // fill rectangle - left bottom yellow color
  glColor4f(1.0, 1.0, 0.0, 1.0);             // set current color r, g, b, and alpha
  glRectf(-0.8, -0.2, -0.2, -0.8);           // fill rectangle - left bottom
  // draw rectangle - right bottom white color
  glColor4f(1.0, 1.0, 1.0, 1.0);             // set current color r, g, b, and alpha
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw
  glRectf(+0.2, -0.2, +0.8, -0.8);           // draw rectangle - right bottom
  glFlush();                                 // force execution of GL commands in finite time
}

void main(int argc, char * argv[]) {
  glutInitWindowPosition(200, 200);          // set the initial window position
  glutInitWindowSize(400, 400);              // set the initial window size
  glutInit(&argc, argv);                     // initialize the GLUT library
  glutInitDisplayMode(GLUT_RGB);             // set the initial display mode to RGB
  glutCreateWindow("Draw/Fill Rectangles "); // create a top-level window
  glutDisplayFunc(display);                  // set the display callback for the current window
  glutMainLoop();                            // enter the GLUT event processing loop
}
