#include "conioex.h"
#include "scene.h"
#include "console.h"
#include "iostream"

/* ===================== HEW Library ========================== */
#ifndef _DEBUG
#pragma comment(lib, "./Library/HEW_x86_Release.lib")
#else
#pragma comment(lib, "./Library/HEW_x86_Debug.lib")
#endif
// Function prototype declarations
bool HEWStartup();
bool HEWCleanup();
/* ============================================================ */

#define FPS	30

void init();
void update();
void render();
void destroy();
void showFPS(float start, float end);

int main() {
  system("cls");
  srand((unsigned int)time(NULL));
  setcursortype(NOCURSOR);

  float start, now;
  timeBeginPeriod(1);
  start = now = timeGetTime();

  /* ====== HEW Startup ======*/
  bool bResurt = 0;
  bResurt = HEWStartup();
  if (bResurt != 1) {
	perror("HEWStartup error");
	return 0;
  }
  /* =========================*/

  init();
  while (true) {
	update();
	render();

	checkScene();

	// Delay ŒvŽZ
	now = timeGetTime();
	if (now - start < 1000 / FPS) {
	  Sleep(1);
	  now = timeGetTime();
	  continue;
	}
	showFPS(start, now);
	start = now;
  }
  destroy();

  /* ====== HEW Cleanup ======*/
  bResurt = HEWCleanup();
  if (bResurt != 1) {
	perror("HEWCleanup error");
	return 0; // FALSE
  }
  /* =========================*/

  timeEndPeriod(1);
}

void init() {
  SetConsoleOutputCP(65001);
  sceneInit();
}

void update() {
  sceneUpdate();
}

void render() {
  sceneRender();
}

void destroy() {
  sceneDestroy();
  consoleEnd();
}

void showFPS(float start, float now) {
  WCHAR fpsStr[256] = { 0 };
  float nowFPS = 1000 / (now - start);
  swprintf(fpsStr, 256, L"FPS: %lf", nowFPS);
  SetConsoleTitle(fpsStr);
}