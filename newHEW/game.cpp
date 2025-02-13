#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "gameField.h"
#include "gameMap.h"
#include "player.h"
#include "buffer.h"
#include "fan.h"
#include "compass.h"
#include "gun.h"

bool isPause;

GameScene gameScene = GameScene::FIELD;

void gameInit() {
  isPause = false;
  gameSceneInit();
  playerInit();
  fanInit();
  compassInit();
  gunInit();
}

void gameSceneInit() {
  switch (gameScene) {
	case GameScene::FIELD:
	  fieldInit();
	  break;
	case GameScene::MAP:
	  mapInit();
	  break;
	}
}

void gameUpdate() {
  fanUpdate();
  compassUpdate();
  gunUpdate();
  switch (gameScene) {
	case GameScene::FIELD:
	  fieldUpdate();
	  break;
	case GameScene::MAP:
	  mapUpdate();
	  break;
  }
  playerUpdate();
  if (gameScene == GameScene::MAP) {
	showPlayerPos();
  }
}

void gameRender() {
  bufferRender();
}

void gameDestroy() {
  playerDestroy();
}

void gameSceneDestroy() {
  switch (gameScene) {
	case GameScene::FIELD:
	  fieldDestroy();
	  break;
	case GameScene::MAP:
	  mapDestroy();
	  break;
  }
}

void setGameScene(GameScene scene) {
  gameSceneDestroy();
  gameScene = scene;
  gameSceneInit();
}