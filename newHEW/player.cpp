#define CONIOEX
#include "conioex.h"
#include "player.h"
#include "buffer.h"
#include "gameField.h"

Player player;

void playerInit() {
  player.pos = { 8.0f, 1.0f };
  player.viewAngle = 0.0f;
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
  if (inport(PK_UP)) {
	player.pos.x += sinf(player.viewAngle) * 0.01f;
	player.pos.y += cosf(player.viewAngle) * 0.01f;
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
  }
  if (inport(PK_LEFT)) {
	player.viewAngle -= 0.005f;
  }
  if (inport(PK_RIGHT)) {
	player.viewAngle += 0.005f;
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
		setBufferFgColor(startX + j, startY + i, brown);
		setBufferText(startX + j, startY + i, "█");
	  }
	  if (ship[i][j] == "D") {
		setBufferFgColor(startX + j, startY + i, lightBrown);
		setBufferText(startX + j, startY + i, "█");
	  }
	  if (ship[i][j] == "H") {
		setBufferFgColor(startX + j, startY + i, brown);
		setBufferText(startX + j, startY + i, "▅");
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