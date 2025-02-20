#define CONIOEX
#include "conioex.h"
#include "gameField.h"
#include "buffer.h"
#include "UI.h"
#include "player.h"
#include "game.h"
#include "draw.h"
#include "gameMath.h"
#include "iceberg.h"
#include "coin.h"

char map[GameFieldHeight][GameFieldWidth] = {
  //	  0         1         2         3         4         5  
  //	  01234567890123456789012345678901234567890123456789012345678
  /*0*/  "***********************************************************",
  /*1*/  "*                                                         *",
  /*2*/  "*                                                         *",
  /*3*/  "*                                                         *",
  /*4*/  "*                                                         *",
  /*5*/  "*                                                         *",
  /*6*/  "*                                                         *",
  /*7*/  "*                                                         *",
  /*8*/  "*                                                         *",
  /*9*/  "*                                                         *",
  /*0*/  "*                                                         *",
  /*1*/  "*                                                         *",
  /*2*/  "*                                                         *",
  /*3*/  "*                                                         *",
  /*4*/  "*                                                         *",
  /*5*/  "*                                                         *",
  /*6*/  "*                                                         *",
  /*7*/  "*                                                         *",
  /*8*/  "*                                                         *",
  /*9*/  "*                                                         *",
  /*0*/  "*                                                         *",
  /*1*/  "*                                                         *",
  /*2*/  "*                                                         *",
  /*3*/  "*                                                         *",
  /*4*/  "*                                                         *",
  /*5*/  "*                                                         *",
  /*6*/  "*                                                         *",
  /*7*/  "*                                                         *",
  /*8*/  "*                                                         *",
  /*9*/  "***********************************************************",
};

void fieldInit() {
  drawBorder({ 0, 0, 64, 25 });
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
  showGetCoinNum(getCollectCoinNum());
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
	  if (13 <= y && y <= 14) {
		setBufferBgColor(x, y, seaBlueLight);
	  }
	  if (y == 15) {
		setBufferBgColor(x, y, seaBlue58);
	  }
	  if (16 <= y && y <= 18) {
		setBufferBgColor(x, y, seaBlue50);
	  }
	  if (y == 19) {
		setBufferBgColor(x, y, seaBlue41);
	  }
	  if (20 <= y && y <= 23) {
		setBufferBgColor(x, y, seaBlueDark);
	  }
	}
  }
  renderBoundary();
  renderSun();
  renderIceberg();
  renderCoin();
}

void clearField() {
  for (int i = 1; i < ScreenFieldWidth; i++) {
	for (int j = 1; j < ScreenFieldHeight; j++) {
	  setBufferText(i, j, " ");
	}
  }
}

char getMapCoordEle(int x, int y) {
  return map[y][x];
}

void setMapCoordEle(int x, int y, char text) {
  map[y][x] = text;
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

void renderBoundary() {
  const Player* player = getPlayer();
  for (int x = 1; x < ScreenFieldWidth; x++) {
	const float radiant = FOV / 180 * PI;
	const float rayAngle = (player->viewAngle - radiant / 2) + x / ScreenFieldWidth * radiant;
	const float rayX = sinf(rayAngle);
	const float rayY = -cosf(rayAngle);
	float distanceToWall = 0.0f;
	bool hitBoundary = false;
	int testX;
	int testY;

	while (!hitBoundary && distanceToWall < 10.0f) {
	  distanceToWall += 0.1f;
	  testX = player->pos.x + rayX * distanceToWall;
	  testY = player->pos.y + rayY * distanceToWall;
	  if (getMapCoordEle(testX, testY) == '*') {
		hitBoundary = true;
		break;
	  }
	}
	for (int y = 1; y < ScreenFieldHeight; y++) {
	  int ceil = 12;
	  int floor = 12;
	  Color color = { 206, 242, 251 };
	  if (distanceToWall < 1.0f) {
		ceil = 0;
		floor = 21;
	  }
	  else if (distanceToWall < 1.2f) {
		color = { 199, 237, 247 };
		ceil = 2;
		floor = 21;
	  }
	  else if (distanceToWall < 1.5f) {
		color = { 191, 232, 243 };
		ceil = 3;
		floor = 21;
	  }
	  else if (distanceToWall < 1.8f) {
		color = { 183, 227, 239 };
		ceil = 4;
		floor = 20;
	  }
	  else if (distanceToWall < 2.2f) {
		color = { 175, 222, 234 };
		ceil = 5;
		floor = 19;
	  }
	  else if (distanceToWall < 2.7f) {
		color = { 167, 217, 230 };
		ceil = 6;
		floor = 18;
	  }
	  else if (distanceToWall < 3.3f) {
		color = { 160, 212, 226 };
		ceil = 7;
		floor = 17;
	  }
	  else if (distanceToWall < 4.0f) {
		color = { 152, 207, 221 };
		ceil = 8;
		floor = 16;
	  }
	  else if (distanceToWall < 5.0f) {
		color = { 144, 202, 217 };
		ceil = 9;
		floor = 15;
	  }
	  else if (distanceToWall < 7.0f) {
		color = { 136, 197, 213 };
		ceil = 10;
		floor = 14;
	  }
	  else if (distanceToWall < 10.0f) {
		color = { 128, 192, 208 };
		ceil = 11;
		floor = 13;
	  }
	  if (ceil < y && y < floor) {
		setFieldBufferText(x, y, "█", color);
	  }
	}
  }
}

void renderSun() {
  const Player* player = getPlayer();

  const Vector3 sunCoord = { player->pos.x, -0.3f, player->pos.y - 1.0f };
  Vector3 sunViewCoord = transformToViewCoord(*player, sunCoord);
  Vector3 sunProCoord = transformToProCoord(sunViewCoord);
  if (0.0f <= sunProCoord.z && sunProCoord.z <= 1.0f) {
	drawSun(sunProCoord);
  }
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