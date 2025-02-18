#define CONIOEX
#include "conioex.h"
#include "player.h"
#include "buffer.h"
#include "gameField.h"
#include "iostream"
#include "mic.h"

Player player;

const float minV = 0.0f;
const float maxV = 0.01f;
const float acceleration = 0.00002f;
const float resistance = 0.00001f;

bool hitWall = false;
float velocity = 0.0f;
float rotateAngle = 0.0f;

int collectCoinNum = 0;

void playerInit() {
  player.pos = { 10.0f, 20.0f };
  player.viewAngle = 0.0f;
  player.dir = { sinf(0.0f), -cosf(0.0f) };
}

void playerUpdate() {
  drawMyBoat();
  playerMove();
}

void playerRender() {

}

void playerDestroy() {

}

Player* getPlayer() {
  return &player;
}

void playerMove() {
  // Hit wall anim
  static int backwardCount = 0;
  if (hitWall) {
	player.pos.x -= player.dir.x * 0.05f;
	player.pos.y -= player.dir.y * 0.05f;
	backwardCount++;
	if (backwardCount == 20) {
	  hitWall = false;
	  velocity = 0.0f;
	  backwardCount = 0;
	}
	return;
  }

  /**********************For TEST**************************************/
  if (inport(PK_UP)) {
	velocity = 0.01f;
  }
  if (inport(PK_DOWN)) {
	velocity = -0.01f;
  }
  if (inport(PK_LEFT)) {
	rotateAngle = -0.1 * PI / 180.0f;
  }
  if (inport(PK_RIGHT)) {
	rotateAngle = 0.1 * PI / 180.0f;
  }
  /********************************************************************/

  // Wall Test
  float newX = player.pos.x + player.dir.x * velocity;
  float newY = player.pos.y + player.dir.y * velocity;
  int mapX = (int)newX;
  int mapY = (int)newY;
  bool nextIsBlock = getMapCoordEle(mapX, mapY) == 'O';
  bool nextIsCoin = getMapCoordEle(mapX, mapY) == 'C';
  float centerX = (float)mapX + 0.5f;
  float centerY = (float)mapY + 0.5f;
  float length = (centerX - newX) * (centerX - newX) + (centerY - newY) * (centerY - newY);
  bool inZone = length <= 0.5f;
  Vector2* coinCoord = getCoinCoord();
  if (inZone) {
	if (nextIsBlock) {
	  hitWall = true;
	  return;
	}
	if (nextIsCoin) {
	  collectCoinNum++;
	  for (int i = 0; i < getCoinNum(); i++) {
		if (coinCoord[i].x == mapX && coinCoord[i].y == mapY) {
		  coinCoord[i].x = -1;
		  coinCoord[i].y = -1;
		  setMapCoordEle(mapX, mapY, ' ');
		}
	  }
	}
  }

  // Move
  player.pos.x += player.dir.x * velocity;
  player.pos.y += player.dir.y * velocity;
  if (player.pos.x < 1.0f) {
	player.pos.x = 1.0f;
  }
  if (player.pos.y < 1.0f) {
	player.pos.y = 1.0f;
  }
  if (player.pos.x > GameFieldWidth - 2.0f) {
	player.pos.x = GameFieldWidth - 2.0f;
  }
  if (player.pos.y > GameFieldHeight - 2.0f) {
	player.pos.y = GameFieldHeight - 2.0f;
  }
  velocity = 0.0f; // velocity = velocity < minV ? minV : velocity - resistance;

  // Rotate
  const float dirX = cosf(rotateAngle) * player.dir.x - sinf(rotateAngle) * player.dir.y;
  const float dirY = sinf(rotateAngle) * player.dir.x + cosf(rotateAngle) * player.dir.y;
  player.viewAngle += rotateAngle;
  player.dir.x = dirX;
  player.dir.y = dirY;
  rotateAngle = 0.0f;
}

void drawMyBoat() {
  for (int i = 0; i < 31; i++) {
	setBufferText(16 + i, 23, "█", darkBrown);
  }
  for (int i = 0; i < 21; i++) {
	setBufferText(21 + i, 22, "█", darkBrown);
	setBufferText(21 + i, 23, "█", brown);
  }
  for (int i = 0; i < 11; i++) {
	setBufferText(26 + i, 21, "█", darkBrown);
	setBufferText(26 + i, 22, "█", brown);
	setBufferText(26 + i, 23, "█", lightBrown);
  }
}

void showPlayerPos() {
  setBufferText(player.pos.x, player.pos.y, "P");
  for (int i = 1; i < 10; i++) {
	setBufferText(player.pos.x + player.dir.x * i, player.pos.y + player.dir.y * i, " ");
	setBufferBgColor(player.pos.x + player.dir.x * i, player.pos.y + player.dir.y * i, { 125, 125, 125 });
  }
}

void setPlayerVelocity(float v) {
  velocity = v;
}

void setPlayerRotate(float angle) {
  rotateAngle = angle;
}

bool isHitwall() {
  return hitWall;
}

int getCollectCoinNum() {
  return collectCoinNum;
}