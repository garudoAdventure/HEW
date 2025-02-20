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
#include "inputKey.h"
#include "coin.h"
#include "iceberg.h"
#include "UI.h"

bool isPause;
GameScene gameScene = GameScene::FIELD;
GameItem items[3] = { GameItem::FAN, GameItem::COMPASS, GameItem::GUN };
int nowItemKey = 0;

void gameInit() {
  isPause = false;
  fieldInit();
  playerInit();
  fanInit();
  compassInit();
  gunInit();
  icebergInit();
  coinInit();
}

void gameSceneInit() {
  switch (gameScene) {
	case GameScene::FIELD:
	  drawBorder({ 0, 0, 64, 25 });
	  break;
	case GameScene::MAP:
	  mapInit();
	  break;
	}
}

void gameUpdate() {
  inputKeyUpdate();
  setGameItemActive();

  switch (gameScene) {
	case GameScene::FIELD:
	  fieldUpdate();
	  break;
	case GameScene::MAP:
	  mapUpdate();
	  break;
  }
  playerUpdate();
  fanUpdate();
  compassUpdate();
  gunUpdate();

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

void setGameItemActive() {
  if (getKeydown(KeyType::UP)) {
	nowItemKey = nowItemKey - 1 < 0 ? 0 : nowItemKey - 1;
  }
  if (getKeydown(KeyType::DOWN)) {
	nowItemKey = nowItemKey + 1 > 2 ? 2 : nowItemKey + 1;
  }
  switch (items[nowItemKey]) {
	case GameItem::FAN:
	  setFanActive(true);
	  setCompassActive(false);
	  setGunActive(false);
	  break;
	case GameItem::COMPASS:
	  setFanActive(false);
	  setCompassActive(true);
	  setGunActive(false);
	  break;
	case GameItem::GUN:
	  setFanActive(false);
	  setCompassActive(false);
	  setGunActive(true);
	  break;
	}
}