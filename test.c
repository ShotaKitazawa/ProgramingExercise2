/*
 * draw lines with OpenGL (opengl_draw_line.c)
 */
/*
gcc opengl_draw_line.c -o opengl_draw_line -lglut -lGL
*/
#include <GL/glut.h>

void display(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0); // clear color r, g, b, and alpha
  glClear(GL_COLOR_BUFFER_BIT);     // clear buffer

  glColor4f(0.0, 1.0, 1.0, 1.0);    // set current color r, g, b, and alpha
  glLineWidth(1);                   // line width
  glBegin(GL_LINES);                // begin draw line
    glVertex2f(-1.0, -1.0);         // point 0: (-1.0, -1.0) left-bottom to
    glVertex2f(+1.0, +1.0);         // point 1: (+1.0, +1.0) top-right
  glEnd();                          // end of draw line

  glColor4f(1.0, 1.0, 0.0, 1.0);    // set current color r, g, b, and alpha
  glLineWidth(8);                   // line width
  glBegin(GL_LINES);                // begin draw line
    glVertex2f(-1.0,  0.0);         //   point 0 to
    glVertex2f(+1.0,  0.0);         //   point 1
  glEnd();                          // end of draw line

  glLineWidth(1);                   // line width
  glBegin(GL_LINES);                // begin draw line
    glColor4f(0.0, 1.0, 0.0, 1.0);  //   set current color r, g, b, and alpha
    glVertex2f(-1.0,  0.0);         //   point 0 to
    glVertex2f( 0.0, +1.0);         //   point 1
                                    //   can draw multiple lines inside glBegin glEnd
    glColor4f(1.0, 0.0, 0.0, 1.0);  //   set current color r, g, b, and alpha
    glVertex2f( 0.0, -1.0);         //   point 2 to
    glVertex2f(+1.0,  0.0);         //   point 3
  glEnd();                          // end of draw line

  glFlush();                        // force execution of GL commands in finite time
}

int main(int argc, char * argv[]) {
  glutInitWindowSize(400, 400);     // set the initial window size (in pixel)
  glutInitWindowPosition(200, 200); // set the initial window position
  glutInit(&argc, argv);            // initialize the GLUT library
  glutInitDisplayMode(GLUT_RGB);    // set the initial display mode to RGB
  glutCreateWindow("Draw Lines");   // create a top-level window
  glutDisplayFunc(display);         // set the display callback for the current window
  glutMainLoop();                   // enter the GLUT event processing loop
  return 0;
}
