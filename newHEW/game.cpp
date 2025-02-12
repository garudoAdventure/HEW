#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "player.h"
#include "gameField.h"
#include "gameMap.h"
#include "buffer.h"
#include "draw3D.h"
#include "draw.h"
#include "fan.h"
#include "compass.h"

bool isPause;

GameScene gameScene = GameScene::FIELD;

void gameInit() {
  /*
  renderBorder(64, 16, 16, 8);
  */

  isPause = false;
  gameSceneInit();
  playerInit();
  fanInit();
  compassInit();
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
  switch (gameScene) {
	case GameScene::FIELD:
	  fieldUpdate();
	  break;
	case GameScene::MAP:
	  mapUpdate();
	  break;
  }
  
  Player* player = getPlayer();
  
  
  for (int x = 1; x < ScreenFieldWidth; x++) {
	const float rayAngle = (player->viewAngle - fov / 2) + x / ScreenFieldWidth * fov;
	const float rayX = sinf(rayAngle);
	const float rayY = -cosf(rayAngle);
	float distanceToWall = 0.0f;
	bool hitWall = false;
	bool hitStone = false;
	int testX;
	int testY;

	while (!hitWall && distanceToWall < 10.0f) {
	  distanceToWall += 0.1f;
	  testX = player->pos.x + rayX * distanceToWall;
	  testY = player->pos.y + rayY * distanceToWall;
	  if (testX < 0 || testX >= GameFieldWidth - 1.0f || testY < 0 || testY > GameFieldHeight) {
		hitWall = true;
		continue;
	  }
	  if (map[testY][testX] != ' ') {
		hitWall = true;
		if (map[testY][testX] == 'O') {
		  hitStone = true;
		}
		continue;
	  }
	  if (gameScene == GameScene::MAP) {
		setBufferText(testX, testY, " ");
		setBufferBgColor(testX, testY, { 50, 50, 50 });
	  }
	}

	if (hitStone && gameScene == GameScene::FIELD) {
	  // Transform to View Coord
	  Vector3 shipCenter = { testX + 0.5f, 0.01f, testY + 0.5f };
	  Vector3 shipSquare[4] = {
		{ -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f },
		{ -0.5f, 0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f },
	  };
	  Vector3 tempCenter;
	  Vector3 temp[4];
	  tempCenter.x = shipCenter.x - player->pos.x;
	  tempCenter.y = shipCenter.y - 0.0f;
	  tempCenter.z = shipCenter.z - player->pos.y;
	  tempCenter = matrixMultiple(tempCenter, rotateYArray(player->viewAngle));
	  for (int i = 0; i < 4; i++) {
		temp[i].x = tempCenter.x + shipSquare[i].x;
		temp[i].y = tempCenter.y + shipSquare[i].y;
		temp[i].z = tempCenter.z + shipSquare[i].z - 0.1f;
	  }

	  for (int i = 0; i < 4; i++) {
		temp[i] = matrixMultiple(temp[i], projectArray());
		temp[i].x = ScreenFieldWidth / 2 * (temp[i].x + 1);
		temp[i].y = ScreenFieldHeight / 2 * (temp[i].y + 1);
	  }

	  if (0.0f <= temp[0].z && temp[0].z <= 1.0f) {
		Color color = {255, 255, 255};
		if (temp[0].z < 0.65f) {
		  color = { 255, 255, 255 };
		}
		else if (temp[0].z < 0.8f) {
		  color = { 200, 200, 200 };
		}
		else if (temp[0].z < 0.86f) {
		  color = { 175, 175, 175 };
		}
		else if (temp[0].z < 0.98f) {
		  color = { 125, 125, 125 };
		}
		else if (temp[0].z < 0.99f) {
		  color = { 100, 100, 100 };
		}
		else if (temp[0].z < 1.0f) {
		  color = { 50, 50, 50 };
		}
		drawSquare(temp, color);
	  }
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

float dotProduct(Vector3 vec1, Vector3 vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 crossProduct(Vector3 vec1, Vector3 vec2) {
  float x = vec1.y * vec2.z - vec2.y * vec1.z;
  float y = vec1.z * vec2.x - vec2.z * vec1.x;
  float z = vec1.x * vec2.y - vec2.x * vec1.y;
  return { x, y, z };
}

float vectorLength(Vector3 vec) {
  float len = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
  len = sqrtf(len);
  return len;
}

Vector3 vectorUnitlize(Vector3 vec) {
  float x = vec.x / vectorLength(vec);
  float y = vec.y / vectorLength(vec);
  float z = vec.z / vectorLength(vec);
  return { x, y, z };
}