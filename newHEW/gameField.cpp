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

  const Vector3 sunCoord = { player->pos.x , -0.3f, player->pos.y - 1.0f };

  const float squareWidth = 0.5f;
  const float squareHeight = 0.5f;
  const Vector3 stoneSquare[4] = {
	{ -squareWidth, -squareHeight, 0.0f }, { squareWidth, -squareHeight, 0.0f },
	{ -squareWidth, squareHeight, 0.0f }, { squareWidth, squareHeight, 0.0f },
  };

  Vector3 sunViewCoord = transformToViewCoord(*player, sunCoord);
  Vector3 sunProCoord = transformToProCoord(sunViewCoord);
  if (0.0f <= sunProCoord.z && sunProCoord.z <= 1.0f) {
	drawSun(sunProCoord);
  }

  for (int i = 0; i < stoneNum; i++) {
	if (stoneCoord[i].x == -1 && stoneCoord[i].y == -1) {
	  continue;
	}
	// Transform to View Coord
	const Vector3 stoneCenter = { stoneCoord[i].x + 0.5f, 0.01f, stoneCoord[i].y + 0.5f };
	
	Vector3 viewStoneCenter = transformToViewCoord(*player, stoneCenter);
	Vector3 stoneVertex[4];
	for (int i = 0; i < 4; i++) {
	  stoneVertex[i].x = viewStoneCenter.x + stoneSquare[i].x;
	  stoneVertex[i].y = viewStoneCenter.y + stoneSquare[i].y;
	  stoneVertex[i].z = viewStoneCenter.z + stoneSquare[i].z - 0.5f;
	  stoneVertex[i] = transformToProCoord(stoneVertex[i]);
	}
	if (0.0f <= stoneVertex[0].z && stoneVertex[0].z <= 1.0f) {
	  drawSquare(stoneVertex, white);
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

Vector2* getStoneCoord() {
  return stoneCoord;
}

Vector3 transformToViewCoord(Player player, Vector3 vec) {
  Vector3 viewVec;
  viewVec.x = vec.x - player.pos.x;
  viewVec.y = vec.y - 0.0f;
  viewVec.z = vec.z - player.pos.y;
  viewVec = matrixMultiple(viewVec, rotateYArray(player.viewAngle));
  return viewVec;
}

Vector3 transformToProCoord(Vector3 vec) {
  Vector3 proVec;
  proVec = matrixMultiple(vec, projectArray());
  proVec.x = ScreenFieldWidth / 2 * (proVec.x + 1);
  proVec.y = ScreenFieldHeight / 2 * (proVec.y + 1);
  return proVec;
}

void drawSun(Vector3 sunCenter) {
  const char* sun[5][9] = {
	{" ", " ", "▄", "▄", "█", "▄", "▄", " ", " "},
	{" ", "█", "█", "█", "█", "█", "█", "█", " "},
	{"█", "█", "█", "█", "█", "█", "█", "█", "█"},
	{" ", "█", "█", "█", "█", "█", "█", "█", " "},
	{" ", " ", "▀", "▀", "█", "▀", "▀", " ", " "},
  };
  
  for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = yellow;
	  if (j == 0 || j == 4 || i == 0 || i == 8 ||
		((i == 1 || i == 7) && (j == 1 || j == 3))
	  ) {
		color = orange;
	  }
	  setFieldBufferText(sunCenter.x - 4 + i, sunCenter.y - 2 + j, sun[j][i], color);
	}
  }
}

void setFieldBufferText(float x, float y, const char* text, Color color) {
  if (x < 1 || x > ScreenFieldWidth || y < 1 || y > ScreenFieldHeight) {
	return;
  }
  setBufferText(x, y, text, color);
}