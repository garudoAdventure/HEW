#include "conioex.h"
#include "scene.h"
#include "console.h"
#include "iostream"

#define FPS	60

void init();
void update();
void render();
void destroy();
void showFPS(float start, float end);

int main() {
  srand((unsigned int)time(NULL));
  setcursortype(NOCURSOR);

  float start, now;
  timeBeginPeriod(1);
  start = now = timeGetTime();

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

  timeEndPeriod(1);
}

void init() {
  consoleInit(80, 25, 25);
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