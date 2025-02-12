#define CONIOEX
#include "conioex.h"
#include "player.h"
#include "buffer.h"
#include "gameField.h"
#include "cmath"
#include "iostream"
#include "mic.h"

Player player;
float velocity = 0.0f;
const float minV = 0.0f;
const float maxV = 0.01f;
const float acceleration = 0.00002f;
const float resistance = 0.00001f;

bool hitWall = false;

void playerInit() {
  player.pos = { 10.5f, 20.0f };
  player.viewAngle = 0.0f;
  player.dir = { sinf(0.0f), -cosf(0.0f) };
}

void playerUpdate() {
  // drawMyBoat();
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
  static int backwardCount = 0;
  if (hitWall) {
	player.pos.x -= player.dir.x * 0.01f;
	player.pos.y -= player.dir.y * 0.01f;
	backwardCount++;
	if (backwardCount == 15) {
	  hitWall = false;
	  velocity = 0.0f;
	  backwardCount = 0;
	}
	return;
  }

  // velocity = velocity < minV ? minV : velocity - resistance;
  velocity = 0.0f;
  if (inport(PK_UP)) {
	// velocity = velocity > maxV ? maxV : velocity + acceleration;
	velocity = 0.01f;
  }

  if (velocity > 0.0f) {
	float newX = player.pos.x + player.dir.x * velocity;
	float newY = player.pos.y + player.dir.y * velocity;
	int mapX = (int)newX;
	int mapY = (int)newY;
	bool nextIsBlock = map[mapY][mapX] == 'O';
	float centerX = (float)mapX + 0.5f;
	float centerY = (float)mapY + 0.5f;
	float length = (centerX - newX) * (centerX - newX) + (centerY - newY) * (centerY - newY);
	bool inZone = length <= 0.03f;
	if (nextIsBlock && inZone) {
	  hitWall = true;
	} else {
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
	}
  }


  if (inport(PK_DOWN)) {
	player.pos.x -= player.dir.x * 0.01f;
	player.pos.y -= player.dir.y * 0.01f;
	if (player.pos.x < 1.0f) {
	  player.pos.x = 1.0f;
	}
	if (player.pos.y < 1.0f) {
	  player.pos.y = 1.0f;
	}
	if (player.pos.x > GameFieldWidth - 2.0f) {
	  player.pos.x = GameFieldWidth - 2.0f;
	}
  }
  if (inport(PK_LEFT)) {
	float angle = -0.1 * PI / 180.0f;
	player.viewAngle += angle;
	player.dir.x = cosf(angle) * player.dir.x - sinf(angle) * player.dir.y;
	player.dir.y = sinf(angle) * player.dir.x + cosf(angle) * player.dir.y;
  }
  if (inport(PK_RIGHT)) {
	float angle = 0.1 * PI / 180.0f;
	player.viewAngle += angle;
	player.dir.x = cosf(angle) * player.dir.x - sinf(angle) * player.dir.y;
	player.dir.y = sinf(angle) * player.dir.x + cosf(angle) * player.dir.y;
  }
}

void drawMyBoat() {
  const int shipWidth = 12;
  const int shipHeight = 5;
  const char* ship[shipHeight][shipWidth] = {
	{" ", " ", " ", " ", " ", "H", "H", " ", " ", " ", " ", " "},
	{" ", " ", " ", " ", "B", "D", "D", "B", " ", " ", " ", " "},
	{" ", " ", " ", "B", "D", "D", "D", "D", "B", " ", " ", " "},
	{" ", " ", "B", "D", "D", "D", "D", "D", "D", "B", " ", " "},
	{" ", " ", "B", "D", "D", "D", "D", "D", "D", "B", " ", " "},
  };
  const int startX = 27;
  const int startY = 19;
  for (int i = 0; i < shipHeight; i++) {
	for (int j = 0; j < shipWidth; j++) {
	  if (ship[i][j] == "B") {
		setBufferText(startX + j, startY + i, "█", brown);
	  }
	  if (ship[i][j] == "D") {
		setBufferText(startX + j, startY + i, "█", lightBrown);
	  }
	  if (ship[i][j] == "H") {
		setBufferText(startX + j, startY + i, "▅", brown);
	  }
	  if (ship[i][j] == " ") {
		continue;
	  }
	}
  }
}

void showPlayerPos() {
  setBufferText(player.pos.x, player.pos.y, "P");
}