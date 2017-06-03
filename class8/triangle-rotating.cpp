/*
 * rotete a cube with OpenGL (cube-rotating.cpp)
 */
/*
g++ cube-rotating.cpp -o cube-rotating -lglut -lGLU -lGL
*/
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

using namespace std;

GLsizei g_width   = 640;       // window width
GLsizei g_height  = 640;       // window height

GLenum g_fillmode = GL_FILL;   // fill mode GL_FILL or GL_LINE
bool     lighting = true;      // lighting or not
bool     cullface = true;      // eliminate back-facing polygons
bool         stop = false;     // toggle between animation and not
bool  perspective = true;      // toggle between perspective and orthographic projection
bool       inside = false;     // see inside or not
GLfloat     scale = 0.7f;      // for orthographic projection scale
GLdouble     fovy = 30.0f;     // for perspective projection, view angle, in degrees, in the y direction
                               // The bigger this angle is, the more you can see of the world -
                               // but at the same time, the objects you can see will become smaller
GLfloat g_light_position[4] = {2.0f, 1.2f, 4.0f, 1.0f}; // light position

GLfloat angle_x = 50.0f;       // angle rotated around x
//GLfloat delta_x = 1.0f;        // increment, added to angle_x --- move
GLfloat delta_x = 10.0f;        // increment, added to angle_x --- move
GLfloat angle_y = 50.0f;       // angle rotated around y
//GLfloat delta_y = 1.0f;        // increment, added to angle_y --- move
GLfloat delta_y = 10.0f;        // increment, added to angle_y --- move
GLfloat angle_z = 0.0f;        // angle rotated around z
GLfloat delta_z = 0.5f;       // increment, added to angle_z --- animate
GLfloat old_delta_z;           // for key 'a'


typedef struct vertex {
  float x, y, z;
} vertex;


typedef struct vector {
  float x, y, z;
} vector;

vertex v[8] = { // local space
  {-0.50f,  0.50f, -0.50f}, // red
  { 0.00f,  0.00f,  0.50f}, // white
  {-0.25f, -0.50f, -0.50f}, // blue
  { 0.50f,  0.25f, -0.50f} // green
};

void idleFunc() {
  glutPostRedisplay(); // redisplay
}

void get_normal(vertex v1, vertex v2, vertex v3, vector* n) { // calculate normal
  vector a, b, c;
  float  length;

  a.x = v2.x-v1.x;
  a.y = v2.y-v1.y;
  a.z = v2.z-v1.z;

  b.x = v3.x-v1.x;
  b.y = v3.y-v1.y;
  b.z = v3.z-v1.z;

  c.x = a.y*b.z - a.z*b.y;
  c.y = a.z*b.x - a.x*b.z;
  c.z = a.x*b.y - a.y*b.x;

  length = sqrt(c.x * c.x + c.y * c.y + c.z * c.z);

  n->x = c.x / length;
  n->y = c.y / length;
  n->z = c.z / length;
}

void displayFunc() {
  vector n;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
  glPushMatrix();                                     // push current matrix onto stack (save)

  angle_z += delta_z;
  printf("%f  %f  %f\r", angle_x, angle_y, angle_z); fflush(stdout);
  glRotatef(angle_x, 1.0f, 0.0f, 0.0f); // move    x
  glRotatef(angle_y, 0.0f, 1.0f, 0.0f); // move    y
  glRotatef(angle_z, 0.0f, 0.0f, 1.0f); // animate z

  glBegin(GL_TRIANGLES);
    get_normal(v[0], v[2], v[1], &n);
    glNormal3f(n.x, n.y, n.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(v[0].x, v[0].y, v[0].z); // red
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(v[2].x, v[2].y, v[2].z); // blue
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(v[1].x, v[1].y, v[1].z); // white
  glEnd();

// tsuyoi
  glBegin(GL_TRIANGLES);
    get_normal(v[0], v[1], v[3], &n);
    glNormal3f(n.x, n.y, n.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(v[0].x, v[0].y, v[0].z); // red
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(v[1].x, v[1].y, v[1].z); // white
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(v[3].x, v[3].y, v[3].z); // green
  glEnd();

  glBegin(GL_TRIANGLES);
    get_normal(v[0], v[3], v[2], &n);
    glNormal3f(n.x, n.y, n.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(v[0].x, v[0].y, v[0].z); // red
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(v[3].x, v[3].y, v[3].z); // green
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(v[2].x, v[2].y, v[2].z); // blue
  glEnd();

  if (!inside) {
  glBegin(GL_TRIANGLES);
    get_normal(v[1], v[2], v[3], &n);
    glNormal3f(n.x, n.y, n.z);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(v[1].x, v[1].y, v[1].z); // white
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(v[2].x, v[2].y, v[2].z); // blue
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(v[3].x, v[3].y, v[3].z); // green
  glEnd();
  }

  glPopMatrix();     // pop matrix from stack (restore)
  glutSwapBuffers(); // swap buffers of the current window if double buffered
}

void reshapeFunc(GLsizei w, GLsizei h) {
  g_width = w; g_height = h;       // save new window size
  glViewport(0, 0, w, h);          // set the viewport size

  glMatrixMode(GL_PROJECTION);     // set matrix mode to project
  glLoadIdentity();                // load identity matrix
  if (perspective)
    gluPerspective(fovy, (GLfloat) w / (GLfloat) h, 1.0f, 10.0f); // create viewing frustum
  else {
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 20.0f);               // create viewing cube
    glScalef(scale, scale, scale);                                // scale little bit down
  }

  glMatrixMode(GL_MODELVIEW);      // set matrix mode to modelview
  glLoadIdentity();                // load identity matrix
  glTranslatef(0.0f, 0.0f, -5.0f); // translate

  glLightfv(GL_LIGHT0, GL_POSITION, g_light_position);            // set position of light
}

void keyboardFunc(unsigned char key, int x, int y) {
  switch(key) {
  case 'l': lighting = !lighting; if (lighting) glEnable(GL_LIGHTING);  else glDisable(GL_LIGHTING);  break;
  case 'k': cullface = !cullface; if (cullface) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE); break;
  case 'w': g_fillmode = (g_fillmode == GL_FILL ? GL_LINE : GL_FILL); // fill or draw
            glPolygonMode(GL_FRONT_AND_BACK, g_fillmode); break;
  case  27: exit(1);                     break; // Esc
  case 'f':                                     // zoom out
    if (perspective) {                          // perspective projection
      if (fovy + 1.0f < 99.0f) fovy += 1.0f;    // view more (zoom out)
    } else {                                    // orthographic projection
      if (scale > 0.2f) {scale /= 1.1f;}        // view more (zoom out)
    } reshapeFunc(g_width, g_height);    break; // re-init
  case 'd':                                     // zoom in
    if (perspective) {                          // perspective projection
      if (fovy - 1.0f > 10.0f) fovy -= 1.0f;    // view less (zoom in)
    } else {                                    // orthographic projection
      if (scale < 2.0f) {scale *= 1.1f;}        // view less (zoom in)
    } reshapeFunc(g_width, g_height);    break; // re-init
  case 'x': angle_x -= delta_x;          break; // up
  case 'v': angle_x += delta_x;          break; // down
  case 'y': angle_y -= delta_y;          break; // left
  case 'i': angle_y += delta_y;          break; // right
  case 'z': if (!stop) delta_z -= 0.02f; break; // clockwise
  case 'c': if (!stop) delta_z += 0.02f; break; // counterclockwise
  case 'p': perspective = !perspective;         // toggle
         reshapeFunc(g_width, g_height); break; // re-init
  case 'a':                                     // stop anim or not
    if (!stop) {
      old_delta_z = delta_z;                    // save
      delta_z  = 0.0f;                          // stop
      stop = true;
    } else {
      delta_z = old_delta_z;                    // restore
      stop = false;
    } glutSwapBuffers();                 break; // re-display
  case 'e': inside = !inside;            break; // show back or not
  case 's':                                     // reset
    angle_x  = 0.0f;
    delta_x  = 1.0f;
    angle_y  = 0.0f;
    delta_y  = 1.0f;
    angle_z  = 0.0f;
    delta_z  = 0.05f;                    break;
  }
}

void specialKeyboardfunc(int key, int x, int y) {              // special key event
  switch(key) {
  case GLUT_KEY_LEFT:  if (!stop) delta_z -= 0.015625f; break; // rotate left
  case GLUT_KEY_RIGHT: if (!stop) delta_z += 0.015625f; break; // rotate right
  case GLUT_KEY_UP:    angle_x -= delta_x;              break; // move   up
  case GLUT_KEY_DOWN:  angle_x += delta_x;              break; // move   down
  }
}

void initialize() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);              // set GL clear color
  glLineWidth(1.0f);                                 // set line width
  glShadeModel(GL_SMOOTH);                           // select smooth shading
  glEnable(GL_DEPTH_TEST);                           // do depth comparisons and update depth buffer
  glFrontFace(GL_CCW);                               // draw front-facing polygons in counterclockwise order
  glPolygonMode(GL_FRONT_AND_BACK, g_fillmode);      // set fill mode (fill or draw wire)
  glEnable(GL_CULL_FACE);                            // eliminate back-facing polygons
  glEnable(GL_LIGHTING);                             // enable lighting
  glLightModeli(GL_FRONT, GL_TRUE);                  // enable lighting for front
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // do ambient and diffuse lighting for front-facing polygons
  glEnable(GL_COLOR_MATERIAL);                       // enable color
  glEnable(GL_LIGHT0);                               // use light source 0 (n: 0 - 7)
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // highest quality
  printf("\n--------------------------------------------------------------------------\n");
  printf("Right hand coordinate system:\n");
  printf("      y\n");
  printf("      |\n");
  printf("      |\n");
  printf("      +--- x\n");
  printf("     / \n");
  printf("    z\n");
  printf("--------------------------------------------------------------------------\n");
  printf("Try keys:\n");
  printf("    x v - rotate around x axis ............................. x: x axis\n");
  printf("    y i - rotate around y axis ............................. y: y axis\n");
  printf("    z c - rotate around z axis ............................. z: z axis\n");
  printf("    d f - zoom in / zoom out ............................... f: far\n");
  printf("    l   - toggle light...................................... l: light\n");
  printf("    k   - toggle backface culling .......................... k: back\n");
  printf("    w   - toggle wireframe / fill draw mode ................ w: wire\n");
  printf("    a   - toggle animate / stop ............................ a: animate\n");
  printf("    e   - toggle to see inside ............................. e: inside\n");
  printf("    p   - toggle perspective / orthographic projection...... p: projection\n");
  printf("    s   - reset ............................................ s: reset\n");
  printf("    Esc - quit ........................................... esc: escape\n");
  printf("    Up  / Down Arrow - rotate around x axis (same as x, v)\n");
  printf("    Left/Right Arrow - rotate around z axis (same as z, c)\n");
  printf("--------------------------------------------------------------------------\n");
}

int main(int argc, char ** argv) {
  glutInit(&argc, argv);                                    // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // use double buffer, rgb color, depth buffer
  glutInitWindowSize(g_width, g_height);                    // initialize window size
  glutInitWindowPosition(100,100);                          // set window postion
  glutCreateWindow(argv[0]);                                // create window
  glutIdleFunc(idleFunc);                                   // called when events are not being received
  glutDisplayFunc(displayFunc);                             // install a display callback
  glutReshapeFunc(reshapeFunc);                             // set reshape function
  glutKeyboardFunc(keyboardFunc);                           // set keyboard function
  glutSpecialFunc(specialKeyboardfunc);                     // set special keyboard function
  initialize();                                             // initialization
  glutMainLoop();                                           // enter GLUT event processing loop
  return 0;
}
