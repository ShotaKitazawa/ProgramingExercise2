/*
 * an uncompleted room (room-underconstruction.cpp)
 */
/*
g++ room-underconstruction.cpp -o room-underconstruction -lglut -lGLU -lGL
-ljpeg
*/
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <stdio.h>
#include <memory.h>
#include <jpeglib.h>

GLfloat WHITE[] = {1.0f, 1.0f, 1.0f, 1.0f};

typedef struct ImageData {  // OpenGL texture info:
  GLsizei width;            // texture width
  GLsizei height;           // texture height
  GLenum format;  // GL_RGB, GL_RGBA, or GL_LUMINANCE (internalFormat == 1)
  GLint internalFormat;  // 1: gray; color otherwise
  GLubyte *texture_ptr;  // texture in byte format, texture_ptr: pointer to the
                         // texture
} ImageData;

#define NO_TEXTURES 2             // number of textures
ImageData *imgData[NO_TEXTURES];  // OpenGL texture infos, wood floor and paper
                                  // wall textures
GLuint img[NO_TEXTURES];          // texture names
char filename[][128] = {"wood_floor.jpg", "wall_paper.jpg"};

ImageData *jpegLoad(const char *filename) {  // get texture from jpeg file
  ImageData *texinfo = NULL;
  FILE *fp = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW j;
  int i;
  fp = fopen(filename, "rb");  // open image file
  if (!fp) {
    fprintf(stderr, "\nerror: can not open \"%s\"!\n", filename);
    fprintf(stderr, "usage: room-underconstruction <filename>.jpg\n");
    return NULL;
  }
  jpeg_create_decompress(&cinfo);  // create and configure decompressor
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);  // read header and prepare for decompression
  jpeg_start_decompress(&cinfo);
  texinfo = (ImageData *)malloc(
      sizeof(ImageData));  // initialize image's member variables
  texinfo->width = cinfo.image_width;
  texinfo->height = cinfo.image_height;
  texinfo->internalFormat = cinfo.num_components;
  if (cinfo.num_components == 1)
    texinfo->format = GL_LUMINANCE;  // gray intensity
  else if (cinfo.num_components == 4)
    texinfo->format = GL_RGBA;  // r, g, b, alpha
  else
    texinfo->format = GL_RGB;  // r, g, b
  texinfo->texture_ptr =
      (GLubyte *)malloc(sizeof(GLubyte) * texinfo->width * texinfo->height *
                        texinfo->internalFormat);
  for (i = 0; i < texinfo->height; i++) {  // extract each scanline of image
    j = (texinfo->texture_ptr + ((texinfo->height - (i + 1)) * texinfo->width *
                                 texinfo->internalFormat));
    jpeg_read_scanlines(&cinfo, &j, 1);
  }
  jpeg_finish_decompress(&cinfo);  // finish decompression and release memory
  jpeg_destroy_decompress(&cinfo);
  fclose(fp);
  return texinfo;
}

class Woodfloor {  // a woodfloor class
  int width;
  int depth;

 public:
  Woodfloor(int width, int depth) : width(width), depth(depth) {}
  double centerX() { return width / 2.0f; }
  double centerZ() { return depth / 2.0f; }
  void draw() {
    GLfloat lightPosition[] = {3.5f, 3.0f, 5.5f, 1.0f};  // light position
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glBegin(GL_QUADS);
    glNormal3d(0.0f, 1.0f, 0.0f);  // normal of woodfloor
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, WHITE);
    // texture   u     v    ; vertex     x      y  z
    glTexCoord2f(0.0f, 0.0f);
    glVertex3d(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3d(0, 0, depth);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3d(width, 0, depth);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3d(width, 0, 0);
    glEnd();
  }
};

/* wall (right hand order)
 x0y0z0 +--+ x3y3z3
        |  |
 x1y1z1 +--+ x2y2z2
 */
class Wall {            // a wall class
  GLdouble x0, y0, z0;  // vertex 0
  GLdouble x1, y1, z1;  // vertex 1
  GLdouble x2, y2, z2;  // vertex 2
  GLdouble x3, y3, z3;  // vertex 3
  GLdouble vx, vy, vz;  // redundant normal
 public:
  Wall(GLdouble x0, GLdouble y0, GLdouble z0, GLdouble x1, GLdouble y1,
       GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble x3,
       GLdouble y3, GLdouble z3, GLdouble vx, GLdouble vy, GLdouble vz)
      : x0(x0),
        y0(y0),
        z0(z0),
        x1(x1),
        y1(y1),
        z1(z1),
        x2(x2),
        y2(y2),
        z2(z2),
        x3(x3),
        y3(y3),
        z3(z3),
        vx(vx),
        vy(vy),
        vz(vz) {}
  void draw() {
    glBegin(GL_QUADS);
    glNormal3d(vx, vy, vz);  // normal
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, WHITE);
    // texture   u     v    ; vertex     x      y  z
    glTexCoord2f(0.0f, 1.0f);
    glVertex3d(x0, y0, z0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3d(x1, y1, z1);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3d(x2, y2, z2);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3d(x3, y3, z3);
    glEnd();
  }
};

class Camera {   // a camera
  double theta;  // angle, determines the x and z positions
  double delta;  // increment in theta
  double y;      // the current y position
  double dy;     // increment in y
 public:
  Camera() : theta(1.0f), delta(0.05f), y(5.5f), dy(0.1f) {}
  double getX() { return 3.0f * cos(theta) + 4.0f; }  // radius = 3
  double getY() { return y; }
  double getZ() { return 3.0f * sin(theta) + 4.0f; }  // radius = 3
  void moveRight() { theta += delta; }
  void moveLeft() { theta -= delta; }
  void moveUp() {
    if (y < 4.9f) y += dy;
  }
  void moveDown() {
    if (y > dy) y -= dy;
  }
};

Woodfloor woodfloor(8, 8);  // global floor
Camera camera;              // global camera
Wall walls[] = {
    Wall(0, 5, 8, 0, 0, 8, 0, 0, 0, 0, 5, 0, 1.0f, 0.0f, 0.0f),  // left wall
    Wall(0, 5, 0, 0, 0, 0, 8, 0, 0, 8, 5, 0, 0.0f, 0.0f, 1.0f)   // front wall
};

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(camera.getX(), camera.getY(),
            camera.getZ(),  // the position of the eye point
            woodfloor.centerX(), 2.5f,
            woodfloor.centerZ(),         // the position of the reference point
            0.0f, 1.0f, 0.0f);           // the direction of the up vector
  glEnable(GL_TEXTURE_2D);               // enable texturing target (2D)
  glBindTexture(GL_TEXTURE_2D, img[0]);  // bind a named texture to a texturing
                                         // target, texture 0
  woodfloor.draw();                      // draw floor
  glBindTexture(GL_TEXTURE_2D, img[1]);  // bind a named texture to a texturing
                                         // target, texture 1
  for (int i = 0; i < sizeof walls / sizeof(Wall); i++) {
    walls[i].draw();  // update walls
  }
  glDisable(GL_TEXTURE_2D);  // disable texturing target (2D)
  glutSwapBuffers();
}

void reshapeFunc(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, GLfloat(w) / GLfloat(h), 1.0, 50.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void specialFunc(int key, int, int) {
  switch (key) {
    case GLUT_KEY_LEFT:
      camera.moveLeft();
      break;
    case GLUT_KEY_RIGHT:
      camera.moveRight();
      break;
    case GLUT_KEY_UP:
      camera.moveUp();
      break;
    case GLUT_KEY_DOWN:
      camera.moveDown();
      break;
  }
  glutPostRedisplay();
}

void initialize() {
  glPixelStorei(GL_UNPACK_ALIGNMENT,
                1);  // set pixel storage mode, 1: byte; 4: int
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
            GL_MODULATE);  // select modulate to mix texture with color
  glGenTextures(NO_TEXTURES,
                img);  // generate texture names and store them to img
  for (int i = 0; i < NO_TEXTURES; i++) {
    imgData[i] = jpegLoad(filename[i]);  // read texture from jpeg file
    if (imgData[i] != NULL) {
      glBindTexture(GL_TEXTURE_2D,
                    img[i]);  // bind a named texture to a texturing target
      glTexParameteri(
          GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
          GL_NEAREST);  // texture minifying if there are more than one pixel
      glTexParameteri(
          GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
          GL_NEAREST);    // texture magnificating if less pixel than required
      glTexImage2D(       // specify a two-dimensional texture image
          GL_TEXTURE_2D,  // target 2D
          0,              // level
          imgData[i]->internalFormat,  // internal format
          imgData[i]->width,           // texture width
          imgData[i]->height,          // texture height
          0,                           // border
          imgData[i]->format,        // format: GL_RGB, GL_RGBA, or GL_LUMINANCE
          GL_UNSIGNED_BYTE,          // type: unsigned byte
          imgData[i]->texture_ptr);  // pointer to image data in memory
      free(imgData[i]->texture_ptr);  // OpenGL has its own copy of texture data
      free(imgData[i]);
    }
  }
  glEnable(GL_DEPTH_TEST);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
  glMaterialf(GL_FRONT, GL_SHININESS, 30);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_CULL_FACE);  // enable backface culling
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 600);
  glutCreateWindow("A Room Underconstruction");
  glutDisplayFunc(displayFunc);
  glutReshapeFunc(reshapeFunc);
  glutSpecialFunc(specialFunc);
  for (int i = 0; i < NO_TEXTURES; i++) {
    if (argc > i + 1) strcpy(filename[i], argv[i + 1]);
  }
  initialize();
  glutMainLoop();
  return 0;
}
