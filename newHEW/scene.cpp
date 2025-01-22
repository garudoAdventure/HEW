#define CONIOEX
#include "conioex.h"
#include "scene.h"
#include "buffer.h"
#include "audio.h"
#include "console.h"
#include "gameTitle.h"
#include "game.h"
#include "gameResult.h"

SCENE g_scene = SCENE::Title;
SCENE g_nextSecne = SCENE::Title;

void sceneInit() {
  bufferInit();
  audioInit();
  switch (g_scene) {
  case SCENE::Title:
	titleInit();
	break;
  case SCENE::Game:
	gameInit();
	break;
  case SCENE::Result:
	resultInit();
	break;
  }
}

void sceneUpdate() {
  audioUpdate();
  switch (g_scene) {
	case SCENE::Title:
	  titleUpdate();
	  break;
	case SCENE::Game:
	  gameUpdate();
	  break;
	case SCENE::Result:
	  resultUpdate();
	  break;
	}
}

void sceneRender() {
  bufferRender();
  switch (g_scene) {
  case SCENE::Title:
	titleRender();
	break;
  case SCENE::Game:
	gameRender();
	break;
  case SCENE::Result:
	resultRender();
	break;
  }
}

void sceneDestroy() {
  switch (g_scene) {
  case SCENE::Title:
	titleDestroy();
	break;
  case SCENE::Game:
	gameDestroy();
	break;
  case SCENE::Result:
	resultDestroy();
	break;
  }
}

void setScene(SCENE scene) {
  g_nextSecne = scene;
}

void checkScene() {
  if (g_scene != g_nextSecne) {
	sceneDestroy();
	g_scene = g_nextSecne;

	clrscr();

	sceneInit();
  }
}