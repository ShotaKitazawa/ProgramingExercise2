#include <GL/glut.h>
#include <math.h>                   // needed for sin, cos
#define PI 3.14159265f

GLfloat radius = 0.5f;              // radius of the bouncing ball
GLfloat x = 0.0f;                   // ball's center (x,y) position
GLfloat y = 0.0f;
GLfloat x_max, x_min, y_max, y_min; // ball's center (x, y) bounds
GLfloat x_speed = 0.02f;            // ball's speed in x and y directions
GLfloat y_speed = 0.007f;
int     refresh_millis = 30;        // refresh period in milliseconds
GLfloat clip_left, clip_right;      // projection clipping area: left right
GLfloat clip_bottom, clip_top;      // projection clipping area: bottom top

void FillCircle(float cx, float cy, float r, int num_segments, bool fill) {
  float theta = 2 * PI / float(num_segments);
  float c = cosf(theta); // precalculate the sine
  float s = sinf(theta); // precalculate the cosine
  float t;               // temp viable
  float x = r;           // start at angle = 0
  float y = 0;
  fill ? glBegin(GL_TRIANGLE_FAN) : glBegin(GL_LINE_LOOP);
  for(int ii = 0; ii < num_segments; ii++) {
    glVertex2f(x + cx, y + cy); // output vertex
    t = x; // apply the rotation matrix
    x = c * x - s * y;
    y = s * t + c * y;
  }
  glEnd();
}

void display() { // callback handler for window re-paint event
  glClearColor(0.0, 0.0, 0.0, 1.0);        // set background (clear) color to black
  glClear(GL_COLOR_BUFFER_BIT);            // clear the color buffer
  glMatrixMode(GL_MODELVIEW);              // to operate on the model-view matrix
  glLoadIdentity();                        // reset model-view matrix
  glTranslatef(x, y, 0.0f);                // translate to (x, y)
  glColor3f(0.0f, 0.0f, 1.0f);             // blue
  FillCircle(0.0, 0.0, radius, 500, true); // fill circle
  glutSwapBuffers(); // swap front and back buffers (of double buffered mode)
  x += x_speed; // animation control
  y += y_speed;
  // check if the ball exceeds the edges
  if (x > x_max) {
    x = x_max;
    x_speed = -x_speed;
  } else if (x < x_min) {
    x = x_min;
    x_speed = -x_speed;
  }
  if (y > y_max) {
    y = y_max;
    y_speed = -y_speed;
  } else if (y < y_min) {
    y = y_min;
    y_speed = -y_speed;
  }
}

void reshape(GLsizei width, GLsizei height) { // call back when the windows is re-sized
  if (height == 0) height = 1;     // to prevent divide by 0
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height); // set the viewport to cover the new window
  glMatrixMode(GL_PROJECTION);     // to operate on the projection matrix
  glLoadIdentity();                // reset the projection matrix

  if (width >= height) { // for defining a 2D orthographic projection matrix
    clip_left   = -1.0 * aspect;
    clip_right  =  1.0 * aspect;
    clip_bottom = -1.0;
    clip_top    =  1.0;
  } else {
    clip_left   = -1.0;
    clip_right  =  1.0;
    clip_bottom = -1.0 / aspect;
    clip_top    =  1.0 / aspect;
  }
  gluOrtho2D(clip_left, // define a 2D orthographic projection matrix
       clip_right,
       clip_bottom,
       clip_top);

  x_min = clip_left   + radius; // ball's center (x, y) bounds
  x_max = clip_right  - radius;
  y_min = clip_bottom + radius;
  y_max = clip_top    - radius;
}

void timer(int value) { // called back when the timer expired
  glutPostRedisplay();  // post a paint request to activate display()
  glutTimerFunc(refresh_millis, timer, 0); // subsequent timer call at milliseconds
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);             // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE);  // enable double buffered mode
  glutInitWindowSize(640, 480);      // initial window width and height
  glutInitWindowPosition(100, 100);  // initial window top-left corner (x, y)
  glutCreateWindow("Bouncing Ball"); // create window with given title
  glutDisplayFunc(display);          // register callback handler for window re-paint
  glutReshapeFunc(reshape);          // register callback handler for window re-shape
  glutTimerFunc(0, timer, 0);        // first timer call immediately
  glutMainLoop();                    // enter event-processing loop
  return 0;
}
