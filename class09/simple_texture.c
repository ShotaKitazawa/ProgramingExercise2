/*
 * simple texture mapping on cube (simple_texture.c)
 */
/*
gcc simple_texture.c -o simple_texture -lglut -lGLU -lGL
 */
#include <GL/glut.h>
#include <GL/glu.h>

#define texture_width  40 // 5 * 8
#define texture_height 40 // 5 * 8
static GLubyte image[texture_height][texture_width][3]; // 3: r, g, b

/*
     1 +---------+ 3
      /         /|
     /         / |
  5 +---------+ 7|
    |         |  |
    | 0       |  + 2
    |         | /
    |         |/
  4 +---------+ 6

 */

typedef struct vertex {
  float x, y, z;
} vertex;

vertex v[8] = {          // local space
  {-0.6f, -0.6f, -0.6f}, // vertex 0 (x, y, z)
  {-0.6f,  0.6f, -0.6f}, // vertex 1 (x, y, z)
  { 0.6f, -0.6f, -0.6f}, // vertex 2 (x, y, z)
  { 0.6f,  0.6f, -0.6f}, // vertex 3 (x, y, z)
  {-0.6f, -0.6f,  0.6f}, // vertex 4 (x, y, z)
  {-0.6f,  0.6f,  0.6f}, // vertex 5 (x, y, z)
  { 0.6f, -0.6f,  0.6f}, // vertex 6 (x, y, z)
  { 0.6f,  0.6f,  0.6f}, // vertex 7 (x, y, z)
};

void generate_texture_image(void) { // generate a 2D texture image
  int i, j, c;
  for (i = 0; i < texture_height; i++) {
    for (j = 0; j < texture_width; j++) {
      c = (((i & 8) == 0) ^ ((j & 8) == 0)) * 255; // 0 or 255
      image[i][j][0] = (GLubyte) c/2;              // red
      image[i][j][1] = (GLubyte) c/2;              // green
      image[i][j][2] = (GLubyte) 255-c;            // blue
    }
  }
}

void initialize(void) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set pixel storage mode, 1: byte; 4: int
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // select modulate to mix texture with color
  generate_texture_image();              // call for generating texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(                          // specify a 2D texture image
               GL_TEXTURE_2D,            // target 2D
               0,                        // level
               GL_RGB,                   // base internal formats
               texture_width,            // texture width
               texture_height,           // texture height
               0,                        // border
               GL_RGB,                   // format
               GL_UNSIGNED_BYTE,         // unsigned byte type
               image);                   // pointer to image data in memory
}

void displayFunc(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D); // enable texturing target

  glPushMatrix();                      // push matrix to stack (save)
  glRotatef( 25.0f, 1.0f, 0.0f, 0.0f); // rotate around (x, y, z) = (1, 0, 0)
  glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // rotate around (x, y, z) = (0, 1, 0)
  glRotatef(  0.0f, 0.0f, 0.0f, 1.0f); // rotate around (x, y, z) = (0, 0, 1)
  glBegin(GL_QUADS);
    // texture   u     v      vertex          x       y       z   // right
    glTexCoord2f(0.0f, 1.0f); glVertex3f(v[7].x, v[7].y, v[7].z); // 7
    glTexCoord2f(0.0f, 0.0f); glVertex3f(v[6].x, v[6].y, v[6].z); // 6
    glTexCoord2f(1.0f, 0.0f); glVertex3f(v[2].x, v[2].y, v[2].z); // 2
    glTexCoord2f(1.0f, 1.0f); glVertex3f(v[3].x, v[3].y, v[3].z); // 3
    // texture   u     v      vertex          x       y       z   // top
    glTexCoord2f(0.0f, 1.0f); glVertex3f(v[5].x, v[5].y, v[5].z); // 5
    glTexCoord2f(0.0f, 0.0f); glVertex3f(v[7].x, v[7].y, v[7].z); // 7
    glTexCoord2f(1.0f, 0.0f); glVertex3f(v[3].x, v[3].y, v[3].z); // 3
    glTexCoord2f(1.0f, 1.0f); glVertex3f(v[1].x, v[1].y, v[1].z); // 1
    // texture   u     v      vertex          x       y       z   // front
    glTexCoord2f(0.0f, 1.0f); glVertex3f(v[5].x, v[5].y, v[5].z); // 5
    glTexCoord2f(0.0f, 0.0f); glVertex3f(v[4].x, v[4].y, v[4].z); // 4
    glTexCoord2f(1.0f, 0.0f); glVertex3f(v[6].x, v[6].y, v[6].z); // 6
    glTexCoord2f(1.0f, 1.0f); glVertex3f(v[7].x, v[7].y, v[7].z); // 7
  glEnd();
  glPopMatrix();                       // pop matrix from stack (restore)

  glFlush();
  glDisable(GL_TEXTURE_2D);            // disable texturing target
}

void reshapeFunc(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);                    // view port

  glMatrixMode(GL_PROJECTION); glLoadIdentity();               // projection:
  gluPerspective(30.0f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f); // perspective

  glMatrixMode(GL_MODELVIEW);  glLoadIdentity();               // view model:
  glTranslatef(0.0f, 0.0f, -5.0f);                             // translate
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640, 640);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Simple Cube Texture Mapping");
  initialize();
  glutDisplayFunc(displayFunc);
  glutReshapeFunc(reshapeFunc);
  glutMainLoop();
  return 0;
}
