#include <GL/glut.h>

void display(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0); // clear color r, g, b, and alpha
  glClear(GL_COLOR_BUFFER_BIT);     // clear buffer

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill
  glBegin(GL_QUADS);        // quads
    glColor3f(0.2f, 0.2f, 0.2f); glVertex2f(-0.6f, -0.7f); // dark gray
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.2f, -0.7f); // white
    glColor3f(0.2f, 0.2f, 0.2f); glVertex2f(-0.2f, -0.3f); // dark gray
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.6f, -0.3f); // white
  glEnd();

  glBegin(GL_TRIANGLES);    // triangles
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f( 0.1f, -0.2f); // red
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f( 0.7f, -0.2f); // green
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f( 0.4f, -0.7f); // blue
  glEnd();

  glBegin(GL_TRIANGLE_FAN); // fans
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.5f,  0.4f); // white
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.4f,  0.2f); // red
    glColor3f(1.0f, 1.0f, 0.0f); glVertex2f(-0.6f,  0.2f); // yellow
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(-0.7f,  0.4f); // green
    glColor3f(0.0f, 1.0f, 1.0f); glVertex2f(-0.6f,  0.6f); // cyan
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(-0.4f,  0.6f); // blue
    glColor3f(1.0f, 0.0f, 1.0f); glVertex2f(-0.3f,  0.4f); // magenta
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.4f,  0.2f); // red repeated
  glEnd();

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw
  glBegin(GL_POLYGON);      // polygon
    glColor3f(1.0f, 0.0f, 0.0f); glVertex2f( 0.4f,  0.2f); // red
    glColor3f(1.0f, 1.0f, 0.0f); glVertex2f( 0.6f,  0.2f); // yellow
    glColor3f(0.0f, 1.0f, 0.0f); glVertex2f( 0.7f,  0.4f); // green
    glColor3f(0.0f, 1.0f, 1.0f); glVertex2f( 0.6f,  0.6f); // cyan
    glColor3f(0.0f, 0.0f, 1.0f); glVertex2f( 0.4f,  0.6f); // blue
    glColor3f(1.0f, 0.0f, 1.0f); glVertex2f( 0.3f,  0.4f); // magenta
  glEnd();

  glFlush();
}

void main(int argc, char * argv[]) {
  glutInitWindowPosition(200, 200);          // set the initial window position
  glutInitWindowSize(400, 400);              // set the initial window size
  glutInit(&argc, argv);                     // initialize the GLUT library
  glutInitDisplayMode(GLUT_RGB);             // set the initial display mode to RGB
  glutCreateWindow("Draw/Fill Polygons ");   // create a top-level window
  glutDisplayFunc(display);                  // set the display callback for the current window
  glutMainLoop();                            // enter the GLUT event processing loop
}
