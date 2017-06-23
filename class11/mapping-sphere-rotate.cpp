#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <memory.h>
#include <jpeglib.h>

GLsizei g_width = 640;   // window width
GLsizei g_height = 640;  // window height

GLenum g_fillmode = GL_FILL;  // fill mode GL_FILL or GL_LINE
bool lighting = true;         // lighting or not
bool cullface = true;         // eliminate back-facing polygons
bool stop = false;            // toggle between animation and not
bool perspective =
    true;              // toggle between perspective and orthographic projection
bool inside = false;   // see inside or not
GLfloat scale = 0.7f;  // for orthographic projection scale
GLdouble fovy = 30.0f;  // for perspective projection, view angle, in degrees,
                        // in the y direction
// The bigger this angle is, the more you can see of the world -
// but at the same time, the objects you can see will become smaller

GLfloat angle_x = -30.0f;  // angle rotated around x
GLfloat delta_x = 1.0f;    // increment, added to angle_x --- move
GLfloat angle_y = 15.0f;   // angle rotated around y
GLfloat delta_y = 1.0f;    // increment, added to angle_y --- move
GLfloat angle_z = 0.0f;    // angle rotated around z
GLfloat delta_z = 0.05f;   // increment, added to angle_z --- animate
GLfloat old_delta_z;       // for key 'a'

GLfloat red = 1.0f;
GLfloat green = 1.0f;
GLfloat blue = 1.0f;

GLint longitude = 43;
GLint latitude = 42;

GLUquadricObj *sphere = NULL;                          // a quadrics object
//GLfloat light_position[4] = {light_x, light_y, light_z, 1.0f};  // light position
//GLfloat light_position[4] = {20.0f, 1.2f, 4.0f, 1.0f};  // light position
GLfloat light_position[4] = {20.0f, 1.2f, 4.0f, 0.0f};  // light position
GLfloat spotDirrection[] = {-0.5, 0.0, -1.0 }; //スポットライトを向ける方向

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
          GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glEnable(GL_TEXTURE_2D);                                     // enable texturing target
  glPushMatrix();                // push current matrix onto stack (save)

  angle_z += delta_z;
  glRotatef(angle_x, 1.0f, 0.0f, 0.0f);  // move    x
  glRotatef(angle_y, 0.0f, 1.0f, 0.0f);  // move    y
  glRotatef(angle_z, 0.0f, 0.0f, 1.0f);  // animate z

  //printf("angle_x: %f\n", angle_x);
  //printf("angle_y: %f\n", angle_y);
  //printf("angle_z: %f\n", angle_z);

  glColor3f(red, green, blue);  // sphere color
  // gluSphere(sphere, 1.28f, 43, 42);                          // sphere,
  // radius, lines of longitude, lines of latitude
  gluSphere(sphere, 1.28f, longitude,
            latitude);  // sphere, radius, lines of longitude, lines of latitude
  glPopMatrix();        // pop matrix from stack (restore)
  glutSwapBuffers();    // swap buffers of the current window if double buffered
}

void reshapeFunc(GLsizei w, GLsizei h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);  // set the viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  // projection:

  if (perspective)
    gluPerspective(fovy, (GLfloat)w / (GLfloat)h, 1.0f,
                   10.0f);  // create viewing frustum
  else {
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 20.0f);  // create viewing cube
    glScalef(scale, scale, scale);                   // scale little bit down
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                                   // view model:
  glTranslatef(0.0f, 0.0f, -5.0f);                    // translate

  light_position[0] += 10.0f;
  light_position[1] += 10.0f;
  //light_position[2] += 10.0f;

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // set position of light
  glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, spotDirrection ); //スポットライトの向ける方向（デフォルト (0,0,-1.0)）
}

void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
    case 'l':
      lighting = !lighting;
      if (lighting)
        glEnable(GL_LIGHTING);
      else
        glDisable(GL_LIGHTING);
      break;
    case 'k':
      cullface = !cullface;
      if (cullface)
        glEnable(GL_CULL_FACE);
      else
        glDisable(GL_CULL_FACE);
      break;
    case 'w':
      g_fillmode = (g_fillmode == GL_FILL ? GL_LINE : GL_FILL);  // fill or draw
      glPolygonMode(GL_FRONT_AND_BACK, g_fillmode);
      break;
    case 0x1b:
      exit(1);
      break;                                    // Esc
    case 'f':                                   // zoom out
      if (perspective) {                        // perspective projection
        if (fovy + 1.0f < 99.0f) fovy += 1.0f;  // view more (zoom out)
      } else {                                  // orthographic projection
        if (scale > 0.2f) {
          scale /= 1.1f;
        }  // view more (zoom out)
      }
      reshapeFunc(g_width, g_height);
      break;                                    // re-init
    case 'd':                                   // zoom in
      if (perspective) {                        // perspective projection
        if (fovy - 1.0f > 10.0f) fovy -= 1.0f;  // view less (zoom in)
      } else {                                  // orthographic projection
        if (scale < 2.0f) {
          scale *= 1.1f;
        }  // view less (zoom in)
      }
      reshapeFunc(g_width, g_height);
      break;  // re-init
    case 'x':
      angle_x -= delta_x;
      break;  // up
    case 'v':
      angle_x += delta_x;
      break;  // down
    case 'y':
      angle_y -= delta_y;
      break;  // left
    case 'i':
      angle_y += delta_y;
      break;  // right
    case 'z':
      if (!stop) delta_z -= 0.02f;
      break;  // clockwise
    case 'c':
      if (!stop) delta_z += 0.02f;
      break;  // counterclockwise
    case 'j':
      perspective = !perspective;  // toggle
      reshapeFunc(g_width, g_height);
      break;   // re-init
    case 'a':  // stop anim or not
      if (!stop) {
        old_delta_z = delta_z;  // save
        delta_z = 0.0f;         // stop
        stop = true;
      } else {
        delta_z = old_delta_z;  // restore
        stop = false;
      }
      glutSwapBuffers();
      break;  // re-display
    case 'e':
      inside = !inside;
      break;   // show back or not
    case 's':  // reset
      angle_x = 0.0f;
      delta_x = 1.0f;
      angle_y = 0.0f;
      delta_y = 1.0f;
      angle_z = 0.0f;
      delta_z = 0.05f;
      break;
    case 'r':
      if (red < 1.0f) red += 0.1f;
      break;
    case 't':
      if (red > 0.0f) red -= 0.1f;
      break;
    case 'g':
      if (green < 1.0f) green += 0.1f;
      break;
    case 'h':
      if (green > 0.0f) green -= 0.1f;
      break;
    case 'b':
      if (blue < 1.0f) blue += 0.1f;
      break;
    case 'n':
      if (blue > 0.0f) blue -= 0.1f;
      break;
    case 'o':
      if (longitude > 3) {
        longitude--;
        latitude--;
      }
      break;
    case 'p':
      longitude++;
      latitude++;
      break;
  }
}

void specialKeyboardfunc(int key, int x, int y) {  // special key event
  switch (key) {
    case GLUT_KEY_LEFT:
      angle_y -= delta_y;
      break;  // rotate left
    case GLUT_KEY_RIGHT:
      angle_y += delta_y;
      break;  // rotate right
    case GLUT_KEY_UP:
      angle_x -= delta_x;
      break;  // move   up
    case GLUT_KEY_DOWN:
      angle_x += delta_x;
      break;  // move   down
  }
}

void initialize() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // set GL clear color
  glPolygonMode(GL_FRONT_AND_BACK, g_fillmode);  // GL_LINE or GL_FILL
  glEnable(GL_DEPTH_TEST);      // do depth comparisons and update depth buffer
  glEnable(GL_CULL_FACE);       // eliminate back-facing polygons
  glEnable(GL_LIGHTING);        // enable lighting
  glEnable(GL_COLOR_MATERIAL);  // enable color
  glEnable(GL_LIGHT0);          // use light source 0 (n: 0 - 7)
  sphere = gluNewQuadric();     // generate sphere
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
  printf("Try keys:\n");
  printf(
      "------------------------------------------------------------------------"
      "--\n");
  printf("Right hand coordinate system:\n");
  printf("      y\n");
  printf("      |\n");
  printf("      |\n");
  printf("      +--- x\n");
  printf("     / \n");
  printf("    z\n");
  printf(
      "------------------------------------------------------------------------"
      "--\n");
  printf(
      "      x v - rotate around x axis ............................. x: x "
      "axis\n");
  printf(
      "      y i - rotate around y axis ............................. y: y "
      "axis\n");
  printf(
      "      z c - rotate around z axis (animation speed & direction) z; z "
      "axis\n");
  printf(
      "      r t - inc / dec red   color ............................ r: "
      "red\n");
  printf(
      "      g h - inc / dec green color ............................ g: "
      "green\n");
  printf(
      "      b n - inc / dec blue  color ............................ b: "
      "blue\n");
  printf(
      "      o p - dec / inc lines of longitude and latitude ........ p: "
      "plus\n");
  printf(
      "      d f - zoom in / zoom out ............................... f: "
      "far\n");
  printf(
      "      l   - toggle lighting .................................. l: "
      "lighting\n");
  printf(
      "      k   - toggle backface culling .......................... k: "
      "back\n");
  printf(
      "      w   - toggle wireframe / fill draw mode ................ w: "
      "wire\n");
  printf(
      "      a   - toggle animate / stop ............................ a: "
      "animate\n");
  printf(
      "      j   - toggle perspective / orthographic projection ..... j: "
      "projection \n");
  printf(
      "      Esc - quit ........................................... esc: "
      "escape\n");
  printf("      Up  / Down Arrow - rotate around x axis (same as x, v)\n");
  printf("      Left/Right Arrow - rotate around z axis (same as z, c)\n");
  printf("\n");
  printf(
      "------------------------------------------------------------------------"
      "--\n");
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);                                     // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // use double
                                                             // buffer, rgb
                                                             // color, depth
                                                             // buffer
  glutInitWindowSize(640, 640);      // initialize window size
  glutInitWindowPosition(100, 100);  // set window postion
  glutCreateWindow("Draw Earth");   // create window
  glutIdleFunc(idleFunc);          // called when events are not being received
  glutDisplayFunc(displayFunc);    // install a display callback
  glutReshapeFunc(reshapeFunc);    // set reshape function
  glutKeyboardFunc(keyboardFunc);  // set keyboard function
  glutSpecialFunc(specialKeyboardfunc);  // set special keyboard function
  if (argc > 1) strcpy(filename, argv[1]);
  initialize();                          // initialization
  glutMainLoop();                        // enter GLUT event processing loop
  return 0;
}
