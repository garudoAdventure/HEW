#define CONIOEX
#include "conioex.h"
#include "scene.h"
#include "buffer.h"
#include "mic.h"
#include "console.h"
#include "gameTitle.h"
#include "game.h"
#include "gameResult.h"

Scene scene = Scene::GAME;
Scene nextSecne = Scene::GAME;

void sceneInit() {
  bufferInit();
  micInit();
  switch (scene) {
  case Scene::TITLE:
	titleInit();
	break;
  case Scene::GAME:
	gameInit();
	break;
  case Scene::RESULT:
	resultInit();
	break;
  }
}

void sceneUpdate() {
  micUpdate();
  switch (scene) {
	case Scene::TITLE:
	  titleUpdate();
	  break;
	case Scene::GAME:
	  gameUpdate();
	  break;
	case Scene::RESULT:
	  resultUpdate();
	  break;
	}
}

void sceneRender() {
  bufferRender();
  switch (scene) {
  case Scene::TITLE:
	titleRender();
	break;
  case Scene::GAME:
	gameRender();
	break;
  case Scene::RESULT:
	resultRender();
	break;
  }
}

void sceneDestroy() {
  switch (scene) {
  case Scene::TITLE:
	titleDestroy();
	break;
  case Scene::GAME:
	gameDestroy();
	break;
  case Scene::RESULT:
	resultDestroy();
	break;
  }
}

void setScene(Scene scene) {
  nextSecne = scene;
}

void checkScene() {
  if (scene != nextSecne) {
	sceneDestroy();
	scene = nextSecne;

	clrscr();

	sceneInit();
  }
}