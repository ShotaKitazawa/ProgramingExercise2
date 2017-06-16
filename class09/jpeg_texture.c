/*
 * jpeg texture mapping on cube (jpeg_texture.c)
 */
/*
gcc jpeg_texture.c -o jpeg_texture -lglut -lGLU -lGL -ljpeg
 */
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <memory.h>
#include <jpeglib.h>

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

typedef struct ImageData { // texture info:
  GLsizei  texture_width;  // texture width
  GLsizei  texture_height; // texture height
  GLenum   format;         // GL_RGB, GL_RGBA, or GL_LUMINANCE (internalFormat == 1)
  GLint    internalFormat; // 1: gray; color otherwise
  GLubyte *image;          // texture in byte format, image: pointer to the texture
} ImageData;

ImageData *imgData;              // texture info
char filename[128] = "wood.jpg"; // default texture file

ImageData *jpegLoad(const char * filename) {            // get texture from jpeg file
  ImageData *texinfo = NULL;
  FILE *fp = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW j;
  int i;
  fp = fopen (filename, "rb");                          // open image file
  if (!fp) {
    fprintf(stderr, "\nerror: can not open \"%s\"!\n", filename);
    fprintf(stderr, "usage: jpeg_texture <filename>.jpg\n");
    return NULL;
  }
  jpeg_create_decompress (&cinfo);                      // create and configure decompressor
  cinfo.err = jpeg_std_error (&jerr);
  jpeg_stdio_src (&cinfo, fp);
  jpeg_read_header (&cinfo, TRUE);                      // read header and prepare for decompression
  jpeg_start_decompress (&cinfo);
  texinfo = (ImageData *) malloc (sizeof (ImageData));  // initialize image's member variables
  texinfo->texture_width = cinfo.image_width;
  texinfo->texture_height = cinfo.image_height;
  texinfo->internalFormat = cinfo.num_components;
  if (cinfo.num_components == 1)
    texinfo->format = GL_LUMINANCE;                     // gray intensity
  else if (cinfo.num_components == 4)
    texinfo->format = GL_RGBA;                          // r, g, b, alpha
  else
    texinfo->format = GL_RGB;                           // r, g, b
  texinfo->image = (GLubyte *) malloc (sizeof (GLubyte)
                                             * texinfo->texture_width
                                             * texinfo->texture_height
                                             * texinfo->internalFormat);
  for (i = 0; i < texinfo->texture_height; i++) {       // extract each scanline of image
    j = (texinfo->image +
         ((texinfo->texture_height - (i + 1))
          * texinfo->texture_width
          * texinfo->internalFormat));
    jpeg_read_scanlines (&cinfo, &j, 1);
  }
  jpeg_finish_decompress (&cinfo);                      // finish decompression and release memory
  jpeg_destroy_decompress(&cinfo);
  fclose (fp);
  return texinfo;
}

void initialize(void) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // set pixel storage mode, 1: byte; 4: int
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // select modulate to mix texture with color
  imgData = jpegLoad(filename);           // read texture from jpeg file
  if (imgData != NULL) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(                         // specify a 2D texture image
                 GL_TEXTURE_2D,           // target 2D
                 0,                       // level
                 imgData->internalFormat, // internal formats
                 imgData->texture_width,  // texture width
                 imgData->texture_height, // texture height
                 0,                       // border
                 imgData->format,         // format
                 GL_UNSIGNED_BYTE,        // unsigned byte type
                 imgData->image);         // pointer to image data in memory
  }
}

void display(void) {
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

void reshape(int w, int h) {
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
  glutCreateWindow("Cube JPEG-Texture Mapping");

  if (argc > 1) strcpy(filename, argv[1]);

  initialize();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
