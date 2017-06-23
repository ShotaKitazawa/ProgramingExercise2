/*
 * mapping sphere (mapping-sphere.cpp)
 */
/*
g++ mapping-sphere.cpp -o mapping-sphere -lglut -lGLU -lGL -ljpeg
*/
#include <GL/glut.h>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <memory.h>
#include <jpeglib.h>

GLUquadricObj *sphere = NULL;                          // a quadrics object
GLfloat light_position[4] = {2.0f, 1.2f, 4.0f, 1.0f};  // light position

typedef struct ImageData {  // texture info:
  GLsizei texture_width;    // texture width
  GLsizei texture_height;   // texture height
  GLenum format;  // GL_RGB, GL_RGBA, or GL_LUMINANCE (internalFormat == 1)
  GLint internalFormat;  // 1: gray; color otherwise
  GLubyte *image;  // texture in byte format, image: pointer to the texture
} ImageData;

ImageData *imgData;                    // texture info
char filename[128] = "world-map.jpg";  // default texture file

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
    fprintf(stderr, "usage: cube-jpeg-mapping <filename>.jpg\n");
    return NULL;
  }
  jpeg_create_decompress(&cinfo);  // create and configure decompressor
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);  // read header and prepare for decompression
  jpeg_start_decompress(&cinfo);
  texinfo = (ImageData *)malloc(
      sizeof(ImageData));  // initialize image's member variables
  texinfo->texture_width = cinfo.image_width;
  texinfo->texture_height = cinfo.image_height;
  texinfo->internalFormat = cinfo.num_components;
  if (cinfo.num_components == 1)
    texinfo->format = GL_LUMINANCE;  // gray intensity
  else if (cinfo.num_components == 4)
    texinfo->format = GL_RGBA;  // r, g, b, alpha
  else
    texinfo->format = GL_RGB;  // r, g, b
  texinfo->image =
      (GLubyte *)malloc(sizeof(GLubyte) * texinfo->texture_width *
                        texinfo->texture_height * texinfo->internalFormat);
  for (i = 0; i < texinfo->texture_height;
       i++) {  // extract each scanline of image
    j = (texinfo->image + ((texinfo->texture_height - (i + 1)) *
                           texinfo->texture_width * texinfo->internalFormat));
    jpeg_read_scanlines(&cinfo, &j, 1);
  }
  jpeg_finish_decompress(&cinfo);  // finish decompression and release memory
  jpeg_destroy_decompress(&cinfo);
  fclose(fp);
  return texinfo;
}

void idleFunc() {
  glutPostRedisplay();  // redisplay
}

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);         // clear color and depth buffers
  glEnable(GL_TEXTURE_2D);              // enable texturing target
  glPushMatrix();                       // push current matrix onto stack (save)
  glRotatef(-80.0f, 1.0f, 0.0f, 0.0f);  // rotate -80 around x
  glRotatef(15.0f, 0.0f, 1.0f, 0.0f);   // rotate  15 around y
  glRotatef(140.0f, 0.0f, 0.0f, 1.0f);  // rotate 140 around z
  glColor3f(1.0f, 1.0f, 1.0f);          // sphere color
  gluSphere(sphere, 1.28f, 43,
            42);      // sphere, radius, lines of longitude, lines of latitude
  glPopMatrix();      // pop matrix from stack (restore)
  glutSwapBuffers();  // swap buffers of the current window if double buffered
}

void reshapeFunc(GLsizei w, GLsizei h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);  // set the viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                             // projection:
  gluPerspective(35.0f, (GLfloat)w / (GLfloat)h, 1.0f, 10.0f);  // perspective
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                                   // view model:
  glTranslatef(0.0f, 0.0f, -5.0f);                    // translate
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // set position of light
}

void initialize() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // set GL clear color
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // GL_LINE or GL_FILL
  glEnable(GL_DEPTH_TEST);      // do depth comparisons and update depth buffer
  glEnable(GL_CULL_FACE);       // eliminate back-facing polygons
  glEnable(GL_LIGHTING);        // enable lighting
  glEnable(GL_COLOR_MATERIAL);  // enable color
  glEnable(GL_LIGHT0);          // use light source 0 (n: 0 - 7)
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  sphere = gluNewQuadric();                       // generate sphere
  gluQuadricNormals(sphere, (GLenum)GLU_SMOOTH);  // smooth quadric normals
  gluQuadricTexture(sphere, GL_TRUE);     // do texture mapping to quadric
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // set pixel storage mode
  imgData = jpegLoad(filename);           // read texture from jpeg file
  if (imgData != NULL) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(                 // specify a 2D texture image
        GL_TEXTURE_2D,            // target 2D
        0,                        // level
        imgData->internalFormat,  // internal formats
        imgData->texture_width,   // texture width
        imgData->texture_height,  // texture height
        0,                        // border
        imgData->format,          // format
        GL_UNSIGNED_BYTE,         // unsigned byte type
        imgData->image);          // pointer to image data in memory
    free(imgData->image);         // OpenGL has its own copy of texture data
    free(imgData);
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);                                     // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // use double
                                                             // buffer, rgb
                                                             // color, depth
                                                             // buffer
  glutInitWindowSize(640, 640);        // initialize window size
  glutInitWindowPosition(100, 100);    // set window postion
  glutCreateWindow("Mapping Sphere");  // create window
  glutIdleFunc(idleFunc);        // called when events are not being received
  glutDisplayFunc(displayFunc);  // install a display callback
  glutReshapeFunc(reshapeFunc);  // set reshape function
  if (argc > 1) strcpy(filename, argv[1]);
  initialize();    // initialization
  glutMainLoop();  // enter GLUT event processing loop
  return 0;
}
