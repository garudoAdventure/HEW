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
  /*7*/  "*                                                        *",
  /*8*/  "*                                                         *",
  /*9*/  "*                                                        *",
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