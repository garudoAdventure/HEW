#define CONIOEX
#include "conioex.h"
#include "scene.h"
#include "gameTitle.h"
#include "draw3D.h"

void titleInit() {
}

void titleUpdate() {
  static int frame = 0;
  if (frame < 100) {
	frame++;
	return;
  }
  if (inport(PK_ENTER)) {
	setScene(SCENE::Game);
	frame = 0;
  }
}

void titleRender() {
  cubeDemo();
}

void titleDestroy() {

}