#define CONIOEX
#include "conioex.h"
#include "player.h"
#include "buffer.h"
#include "gameField.h"
#include "mic.h"
#include "coin.h"

Player player;
BoatWave boatWave = BoatWave::BOAT_DOWN;

const float minV = 0.0f;
const float resistance = 0.000001f;

bool hitWall = false;
float velocity = 0.0f;
float rotateAngle = 0.0f;

int collectCoinNum = 0;
int coinSound;

void playerInit() {
  player.pos = { 32.0f, 28.0f };
  player.viewAngle = 0.0f;
  player.dir = { sinf(0.0f), -cosf(0.0f) };
  coinSound = opensound((char*)"./Sound/coin.wav");
}

void playerUpdate() {
  drawSpray();
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

float getPlayerVelocity() {
  return velocity;
}

void playerMove() {
  static int frame = 0;
  if (frame < 200) {
	frame++;
  } else {
	frame = 0;
	boatWave = boatWave == BoatWave::BOAT_DOWN ? BoatWave::BOAT_UP : BoatWave::BOAT_DOWN;
  }

  // Hit wall anim
  static int backwardCount = 0;
  if (hitWall) {
	player.pos.x -= player.dir.x * 0.05f;
	player.pos.y -= player.dir.y * 0.05f;
	backwardCount++;
	if (backwardCount == 18) {
	  hitWall = false;
	  velocity = 0.0f;
	  backwardCount = 0;
	}
	return;
  }

  // Wall Test
  float newX = player.pos.x + player.dir.x * velocity;
  float newY = player.pos.y + player.dir.y * velocity;
  int mapX = (int)newX;
  int mapY = (int)newY;
  bool nextIsBlock = getMapCoordEle(mapX, mapY) == 'O' || getMapCoordEle(mapX, mapY) == '*';
  bool nextIsCoin = getMapCoordEle(mapX, mapY) == 'C';
  float centerX = (float)mapX + 0.5f;
  float centerY = (float)mapY + 0.5f;
  float length = (centerX - newX) * (centerX - newX) + (centerY - newY) * (centerY - newY);
  bool inBlockZone = length <= 0.5f;
  bool inCoinZone = length <= 0.02f;
  CoinList* coinList = getCoinList();
  if (nextIsBlock && inBlockZone) {
	hitWall = true;
	return;
  }
  if (nextIsCoin && inCoinZone) {
	collectCoinNum++;
	playsound(coinSound, 0);
	CoinNode* coinNode = coinList->next;
	CoinNode* prevNode = NULL;
	while (coinNode != NULL) {
	  if (coinNode->pos.x == mapX && coinNode->pos.y == mapY) {
		if (prevNode == NULL) {
		  coinList->next = coinNode->next;
		}
		else {
		  prevNode->next = coinNode->next;
		}
		free(coinNode);
		setMapCoordEle(mapX, mapY, ' ');
		break;
	  }
	  prevNode = coinNode;
	  coinNode = coinNode->next;
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
  if (player.pos.y > GameFieldHeight - 1.0f) {
	player.pos.y = GameFieldHeight - 1.0f;
  }
  velocity = velocity - resistance < minV ? minV : velocity - resistance;

  // Rotate
  const float dirX = cosf(rotateAngle) * player.dir.x - sinf(rotateAngle) * player.dir.y;
  const float dirY = sinf(rotateAngle) * player.dir.x + cosf(rotateAngle) * player.dir.y;
  player.viewAngle += rotateAngle;
  player.dir.x = dirX;
  player.dir.y = dirY;
  rotateAngle = 0.0f;
}

void drawMyBoat() {
  int startY = 23 + boatWave;
  for (int i = 0; i < 35; i++) {
	setFieldBufferText(14 + i, startY, "█", darkBrown);
  }
  for (int i = 0; i < 31; i++) {
	setFieldBufferText(16 + i, startY, "█", brown);
	setFieldBufferText(16 + i, startY - 1, "█", darkBrown);
  }
  for (int i = 0; i < 21; i++) {
	setFieldBufferText(21 + i, startY, "█", lightBrown);
	setFieldBufferText(21 + i, startY - 1, "█", brown);
	setFieldBufferText(21 + i, startY - 2, "█", darkBrown);
  }
  for (int i = 0; i < 11; i++) {
	setFieldBufferText(26 + i, startY - 1, "█", lightBrown);
	setFieldBufferText(26 + i, startY - 2, "█", brown);
	setFieldBufferText(26 + i, startY - 3, "█", darkBrown);
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

BoatWave getBoatWave() {
  return boatWave;
}

void drawSpray() {
  static int frame = 0;
  static int sprayIdx[4] = { 0, 0, 0, 0 };
  const int startY = 23;
  const int currentV = velocity * 20000;
  const int maxFrame = 180 - currentV;

  if (currentV < 1.0f) {
	return;
  }

  if (frame % (maxFrame / 18) == 0) {
	sprayIdx[0] = (sprayIdx[0] + 1) % 19;
  }
  if (frame % (maxFrame / 16) == 0) {
	sprayIdx[1] = (sprayIdx[1] + 1) % 17;
  }
  if (frame % (maxFrame / 12) == 0) {
	sprayIdx[2] = (sprayIdx[2] + 1) % 13;
  }
  if (frame % (maxFrame / 8) == 0) {
	sprayIdx[3] = (sprayIdx[3] + 1) % 8;
  }
  frame++;
  if (frame > maxFrame) {
	frame = 0;
	memset(sprayIdx, 0, sizeof(sprayIdx));
  }

  for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 3; j++) {
	  setFieldBufferText(31 + j + sprayIdx[i], startY - i, "█", white);
	  setFieldBufferText(31 - j - sprayIdx[i], startY - i, "█", white);
	}
  }
}