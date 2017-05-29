/* prog02.c */
/* 青く塗りつぶしたウィンドウを開き，プリミティブを描く */
/* ESC キーで終了 */

#include <GL/glut.h>
#include <stdlib.h> /* exit() のため */

/* 描画 */
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  //glBegin(GL_POINTS);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0.0, 0.0);
  glVertex2f(0.2, 0.4);
  glVertex2f(0.4, 0.4);
  glEnd();

  glFlush();
}

/* キーボードからプログラムを終了する設定 */
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case '\33': /* \33 は ESC の ASCII コード (8進数で表示）*/
    exit(0);
  default :
    break;
  }
}

void init(void)
{
  /* 変更なし(前回の内容そのまま） */
}

int main(int argc, char** argv)
{
  /* 初期化 */
  glutInit(&argc, argv);

  /* ウィンドウの生成 */
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowPosition(200, 50); /* ウィンドの位置(x, y) */
  glutInitWindowSize(300, 300); /* ウィンドの大きさ(w, h) */
  glutCreateWindow(argv[0]);

  /* OpenGL 初期化ルーチンの呼出し */
  init();

  /* 描画ルーチンの設定 */
  glutDisplayFunc(display);

  /* 入力処理ルーチンの設定 */
  glutKeyboardFunc( keyboard );

  /* 無限ループ */
  glutMainLoop();
  return 0;
}
