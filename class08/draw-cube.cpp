/*
 * draw a cube with OpenGL (draw-cube.cpp)
 */
/*
g++ draw-cube.cpp -o draw-cube -lglut -lGLU -lGL
*/
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>

GLsizei  width   = 640;     // window width
GLsizei  height  = 640;     // window height

GLenum  fillmode = GL_FILL; // fill mode GL_FILL or GL_LINE
bool    lighting = true;    // lighting or not
bool    cullface = true;    // eliminate back-facing polygons
bool perspective = true;    // toggle between perspective and orthographic projection
GLfloat    scale = 0.82f;   // for orthographic projection scale
GLdouble    fovy = 30.0f;   // for perspective projection, view angle, in degrees, in the y direction
                            // The bigger this angle is, the more you can see of the world -
                            // but at the same time, the objects you can see will become smaller
GLfloat light_position[4] = {2.0f, 1.2f, 4.0f, 1.0f}; // light position

GLfloat  angle_x = -45.0f;  // angle rotated around x
GLfloat  angle_y =  30.0f;  // angle rotated around y
GLfloat  angle_z =  20.0f;  // angle rotated around z

/*
     1 +---------+ 3
      /|        /|
     / |       / |
  5 +---------+ 7|
    |  |      |  |
    |0 +------|--+ 2
    | /       | /
    |/        |/
  4 +---------+ 6

 */

typedef struct vertex {
  float x, y, z;
} vertex;

vertex v[8] = {          // local space
  {-0.5f, -0.5f, -0.5f}, // vertex 0 (x, y, z)
  {-0.5f,  0.5f, -0.5f}, // vertex 1 (x, y, z)
  { 0.5f, -0.5f, -0.5f}, // vertex 2 (x, y, z)
  { 0.5f,  0.5f, -0.5f}, // vertex 3 (x, y, z)
  {-0.5f, -0.5f,  0.5f}, // vertex 4 (x, y, z)
  {-0.5f,  0.5f,  0.5f}, // vertex 5 (x, y, z)
  { 0.5f, -0.5f,  0.5f}, // vertex 6 (x, y, z)
  { 0.5f,  0.5f,  0.5f}, // vertex 7 (x, y, z)
};

void idleFunc() {
  glutPostRedisplay(); // redisplay
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffers
  glPushMatrix();                                     // push current matrix onto stack (save)
  printf("%f  %f  %f\r", angle_x, angle_y, angle_z); fflush(stdout);
  glRotatef(angle_x, 1.0f, 0.0f, 0.0f); // rotate angle_x around x
  glRotatef(angle_y, 0.0f, 1.0f, 0.0f); // rotate angle_y around y
  glRotatef(angle_z, 0.0f, 0.0f, 1.0f); // rotate angle_z around z

  glBegin(GL_QUADS); // quads begin

    // right 7 6 2 3
    glColor3f(0.0f, 0.0f, 1.0f);        // blue
    glNormal3f(1.0f, 0.0f, 0.0f);       // normal x =  1 for 4 vertices
    glVertex3f(v[7].x, v[7].y, v[7].z); // 7
    glVertex3f(v[6].x, v[6].y, v[6].z); // 6
    glVertex3f(v[2].x, v[2].y, v[2].z); // 2
    glVertex3f(v[3].x, v[3].y, v[3].z); // 3

    // left 5 1 0 4
    glColor3f(1.0f, 0.0f, 1.0f);        // magenta
    glNormal3f(-1.0f, 0.0f, 0.0f);      // normal x = -1 for 4 vertices
    glVertex3f(v[5].x, v[5].y, v[5].z); // 5
    glVertex3f(v[1].x, v[1].y, v[1].z); // 1
    glVertex3f(v[0].x, v[0].y, v[0].z); // 0
    glVertex3f(v[4].x, v[4].y, v[4].z); // 4
    
    // top 5 7 3 1
    glColor3f(1.0f, 0.0f, 0.0f);        // red
    glNormal3f(0.0f, 1.0f, 0.0f);       // normal y =  1 for 4 vertices
    glVertex3f(v[5].x, v[5].y, v[5].z); // 5
    glVertex3f(v[7].x, v[7].y, v[7].z); // 7
    glVertex3f(v[3].x, v[3].y, v[3].z); // 3
    glVertex3f(v[1].x, v[1].y, v[1].z); // 1

    // bottom 4 0 2 6
    glColor3f(0.0f, 1.0f, 1.0f);        // cyan
    glNormal3f(0.0f, -1.0f, 0.0f);      // normal y = -1 for 4 vertices
    glVertex3f(v[4].x, v[4].y, v[4].z); // 4
    glVertex3f(v[0].x, v[0].y, v[0].z); // 0
    glVertex3f(v[2].x, v[2].y, v[2].z); // 2
    glVertex3f(v[6].x, v[6].y, v[6].z); // 6

    // front 5 4 6 7
    glColor3f(0.0f, 1.0f, 0.0f);        // green
    glNormal3f(0.0f, 0.0f, 1.0f);       // normal z =  1 for 4 vertices
    glVertex3f(v[5].x, v[5].y, v[5].z); // 5
    glVertex3f(v[4].x, v[4].y, v[4].z); // 4
    glVertex3f(v[6].x, v[6].y, v[6].z); // 6
    glVertex3f(v[7].x, v[7].y, v[7].z); // 7

    // back 1 3 2 0
    glColor3f(1.0f, 1.0f, 0.0f);        // yellow
    glNormal3f(0.0f, 0.0f, -1.0f);      // normal z = -1 for 4 vertices
    glVertex3f(v[1].x, v[1].y, v[1].z); // 1
    glVertex3f(v[3].x, v[3].y, v[3].z); // 3
    glVertex3f(v[2].x, v[2].y, v[2].z); // 2
    glVertex3f(v[0].x, v[0].y, v[0].z); // 0

  glEnd();           // quads end

  glPopMatrix();     // pop matrix from stack (restore)
  glutSwapBuffers(); // swap buffers of the current window if double buffered
}

void reshapeFunc(GLsizei w, GLsizei h) {
  width = w; height = h;           // save new window size
  glViewport(0, 0, w, h);          // set the viewport size

  glMatrixMode(GL_PROJECTION);     // set matrix mode to project
  glLoadIdentity();                // load identity matrix
  if (perspective)
    gluPerspective(fovy, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);   // create viewing frustum
  else {
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 20.0f);               // create viewing cube
    glScalef(scale, scale, scale);                                // scale little bit down
  }
  
  glMatrixMode(GL_MODELVIEW);      // set matrix mode to modelview
  glLoadIdentity();                // load identity matrix
  glTranslatef(0.0f, 0.0f, -5.0f); // translate

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);              // set position of light
}

void keyboardFunc(unsigned char key, int x, int y) {
  switch(key) {
  case 'l': lighting = !lighting; if (lighting) glEnable(GL_LIGHTING);  else glDisable(GL_LIGHTING);  break;
  case 'k': cullface = !cullface; if (cullface) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE); break;
  case 'w': fillmode = (fillmode == GL_FILL ? GL_LINE : GL_FILL); // fill or draw
            glPolygonMode(GL_FRONT_AND_BACK, fillmode); break;
  case  27: exit(1);                     break; // Esc
  case 'x': angle_x -= 1.0f;             break; // up
  case 'v': angle_x += 1.0f;             break; // down
  case 'y': angle_y -= 1.0f;             break; // left
  case 'i': angle_y += 1.0f;             break; // right
  case 'z': angle_z -= 1.0f;             break; // clockwise
  case 'c': angle_z += 1.0f;             break; // counterclockwise
  case 'p': perspective = !perspective;         // toggle projection
         reshapeFunc(width, height);     break; // re-init
  }
}

void initialize() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);              // set GL clear color
  glLineWidth(1.0f);                                 // set line width
  glShadeModel(GL_SMOOTH);                           // select smooth shading
  glEnable(GL_DEPTH_TEST);                           // do depth comparisons and update depth buffer
  glFrontFace(GL_CCW);                               // draw front-facing polygons in counterclockwise order
  glPolygonMode(GL_FRONT_AND_BACK, fillmode);        // set fill mode (fill or draw wire)
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
  printf("    l   - toggle light ..................................... l: light\n");
  printf("    k   - toggle backface culling .......................... k: back\n");
  printf("    w   - toggle wireframe / fill draw mode ................ w: wire\n");
  printf("    p   - toggle perspective / orthographic projection ..... p: projection\n");
  printf("    Esc - quit ........................................... esc: escape\n");
  printf("--------------------------------------------------------------------------\n");
}

int main(int argc, char ** argv) {
  glutInit(&argc, argv);                                    // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // use double buffer, rgb color, depth buffer
  glutInitWindowSize(width, height);                        // initialize window size
  glutInitWindowPosition(100,100);                          // set window postion
  glutCreateWindow(argv[0]);                                // create window
  glutIdleFunc(idleFunc);                                   // called when events are not being received
  glutDisplayFunc(displayFunc);                             // install a display callback
  glutReshapeFunc(reshapeFunc);                             // set reshape function
  glutKeyboardFunc(keyboardFunc);                           // set keyboard function
  initialize();                                             // initialization
  glutMainLoop();                                           // enter GLUT event processing loop
  return 0;
}
