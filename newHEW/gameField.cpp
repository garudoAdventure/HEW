#define CONIOEX
#include "conioex.h"
#include "gameField.h"
#include "buffer.h"
#include "UI.h"
#include "player.h"
#include "game.h"
#include "draw.h"
#include "gameMath.h"

Vector2 stoneCoord[100] = {};
int stoneNum;

void fieldInit() {
  drawBorder({ 0, 0, 64, 25 });
  stoneNum = 0;
  for (int i = GameFieldWidth - 1; i >= 0 ; i--) {
	for (int j = GameFieldHeight - 1; j >= 0; j--) {
	  if (map[j][i] == 'O') {
		stoneCoord[stoneNum] = { i, j };
		stoneNum++;
	  }
	}
  }
}

void fieldUpdate() {
  static int frameCount = 0;
  frameCount = (frameCount + 1) % 200;
  if (inport(PK_BS) && frameCount > 50) {
	setGameScene(GameScene::MAP);
	frameCount = 0;
	return;
  }
  clearField();
  renderField();
}

void fieldRender() {
}

void fieldDestroy() {
  clearField();
}

void renderField() {
  for (int x = 1; x < ScreenFieldWidth; x++) {
	for (int y = 1; y < ScreenFieldHeight; y++) {
	  if (y <= 12) {
		setBufferBgColor(x, y, skyBlue);
	  }
	  if (y >= 13) {
		setBufferBgColor(x, y, seaBlue);
	  }
	}
  }
  const Player* player = getPlayer();
  for (int i = 0; i < stoneNum; i++) {
	if (stoneCoord[i].x == -1 && stoneCoord[i].y == -1) {
	  continue;
	}
	// Transform to View Coord
	const Vector3 shipCenter = { stoneCoord[i].x + 0.5f, 0.01f, stoneCoord[i].y + 0.5f };
	const float squareWidth = 0.5f;
	const float squareHeight = 0.5f;
	const Vector3 shipSquare[4] = {
	  { -squareWidth, -squareHeight, 0.0f }, { squareWidth, -squareHeight, 0.0f },
	  { -squareWidth, squareHeight, 0.0f }, { squareWidth, squareHeight, 0.0f },
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
	  temp[i].z = tempCenter.z + shipSquare[i].z - 0.5f;
	}

	for (int i = 0; i < 4; i++) {
	  temp[i] = matrixMultiple(temp[i], projectArray());
	  temp[i].x = ScreenFieldWidth / 2 * (temp[i].x + 1);
	  temp[i].y = ScreenFieldHeight / 2 * (temp[i].y + 1);
	}

	if (0.0f <= temp[0].z && temp[0].z <= 1.0f) {
	  Color color = { 255, 255, 255 };
	  /*if (temp[0].z < 0.65f) {
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
	  }*/
	  drawSquare(temp, color);
	}
  }
}

void clearField() {
  for (int i = 1; i < ScreenFieldWidth; i++) {
	for (int j = 1; j < ScreenFieldHeight; j++) {
	  setBufferText(i, j, " ");
	}
  }
}

void drawSmallBoat(int x, int y) {
  setFieldBuffer(x, y, "▀", darkBrown);
  setFieldBuffer(x + 1, y, "▀", darkBrown);
}

void drawMiddleBoat(int x, int y) {
  setFieldBuffer(x + 1, y - 2, "▅", brown);
  setFieldBuffer(x + 2, y - 2, "▅", black);

  setFieldBuffer(x, y - 1, "▅", darkBrown);
  setFieldBuffer(x + 1, y - 1, "█", brown);
  setFieldBuffer(x + 2, y - 1, "▅", lightBrown);
  setFieldBuffer(x + 3, y - 1, "▅", darkBrown);

  setFieldBuffer(x + 1, y, "▀", darkBrown);
  setFieldBuffer(x + 2, y, "▀", darkBrown);
}

void drawBigBoat(int x, int y) {
  setFieldBuffer(x + 3, y - 5, "█", brown);
  setFieldBuffer(x + 4, y - 5, "█", black);
  setFieldBuffer(x + 5, y - 5, "█", black);

  setFieldBuffer(x + 2, y - 4, "▅", darkBrown);
  setFieldBuffer(x + 3, y - 4, "█", brown);
  setFieldBuffer(x + 4, y - 4, "▅", darkBrown);
  setFieldBuffer(x + 5, y - 4, "▅", darkBrown);

  setFieldBuffer(x + 1, y - 3, "█", darkBrown);
  setFieldBuffer(x + 2, y - 3, "█", lightBrown);
  setFieldBuffer(x + 3, y - 3, "█", brown);
  setFieldBuffer(x + 4, y - 3, "█", lightBrown);
  setFieldBuffer(x + 5, y - 3, "█", lightBrown);
  setFieldBuffer(x + 6, y - 3, "█", darkBrown);

  setFieldBuffer(x, y - 2, "█", darkBrown);
  setFieldBuffer(x + 1, y - 2, "█", lightBrown);
  setFieldBuffer(x + 2, y - 2, "█", lightBrown);
  setFieldBuffer(x + 3, y - 2, "█", lightBrown);
  setFieldBuffer(x + 4, y - 2, "█", lightBrown);
  setFieldBuffer(x + 5, y - 2, "█", lightBrown);
  setFieldBuffer(x + 6, y - 2, "█", lightBrown);
  setFieldBuffer(x + 7, y - 2, "█", darkBrown);

  setFieldBuffer(x + 1, y - 1, "█", darkBrown);
  setFieldBuffer(x + 2, y - 1, "█", lightBrown);
  setFieldBuffer(x + 3, y - 1, "█", lightBrown);
  setFieldBuffer(x + 4, y - 1, "█", lightBrown);
  setFieldBuffer(x + 5, y - 1, "█", lightBrown);
  setFieldBuffer(x + 6, y - 1, "█", darkBrown);

  setFieldBuffer(x + 2, y, "▀", darkBrown);
  setFieldBuffer(x + 3, y, "█", darkBrown);
  setFieldBuffer(x + 4, y, "█", darkBrown);
  setFieldBuffer(x + 5, y, "▀", darkBrown);
}

void setFieldBuffer(int x, int y, const char* shape, Color color) {
  if (x < 0 || x > ScreenFieldWidth - 1.0f || y < 0 || y > ScreenFieldHeight - 1.0f) {
	return;
  }
  setBufferTextAndColor(x, y, shape, color);
}

Vector2* getStoneCoord() {
  return stoneCoord;
}