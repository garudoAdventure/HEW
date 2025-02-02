#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "player.h"
#include "gameField.h"
#include "gameMap.h"
#include "buffer.h"
#include "UI.h"

bool isPause;

GameScene gameScene = GameScene::FIELD;

void gameInit() {
  renderBorder(64, 0, 16, 8);
  renderBorder(64, 8, 16, 8);
  renderBorder(64, 16, 16, 8);

  isPause = false;
  gameSceneInit();
  playerInit();
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
  switch (gameScene) {
	case GameScene::FIELD:
	  fieldUpdate();
	  break;
	case GameScene::MAP:
	  mapUpdate();
	  break;
  }

  Player* player = getPlayer();
  if (gameScene == GameScene::MAP) {
	float cameraPlaneVecX = cosf(player->viewAngle);
	float cameraPlaneVecY = -sinf(player->viewAngle);
	for (int i = 0; i < 5; i++) {
	  int coorX = player->pos.x + cameraPlaneVecX * i;
	  int coorY = player->pos.y + cameraPlaneVecY * i;
	  setBufferText(coorX, coorY, " ");
	  setBufferBgColor(coorX, coorY, { 125, 125, 0 });

	  coorX = player->pos.x - cameraPlaneVecX * i;
	  coorY = player->pos.y - cameraPlaneVecY * i;
	  setBufferText(coorX, coorY, " ");
	  setBufferBgColor(coorX, coorY, { 125, 125, 0 });
	}
  }
  
  for (int x = 1; x < ScreenFieldWidth; x++) {
	const float rayAngle = (player->viewAngle - fov / 2) + x / ScreenFieldWidth * fov;
	const float rayX = sinf(rayAngle);
	const float rayY = cosf(rayAngle);
	float distanceToWall = 0.0f;
	float distanceToCameraPlane = 0.0f;
	bool hitWall = false;
	while (!hitWall) {
	  distanceToWall += 0.1f;
	  int testX = player->pos.x + rayX * distanceToWall;
	  int testY = player->pos.y + rayY * distanceToWall;
	  distanceToCameraPlane = distanceToWall * cosf(rayAngle - player->viewAngle);
	  if (testX < 0 || testX >= GameFieldWidth - 1.0f || testY < 0 || testY > GameFieldHeight || map[testY][testX] != ' ') {
		hitWall = true;
		continue;
	  }
	  if (gameScene == GameScene::MAP) {
		// Field of View
		setBufferText(testX, testY, " ");
		setBufferBgColor(testX, testY, { 50, 50, 50 });
	  }
	}
	if (gameScene == GameScene::MAP) {
	  continue;
	}
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