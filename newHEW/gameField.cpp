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
  /*3*/  "*       O  O                      O                       *",
  /*4*/  "*                                                         *",
  /*5*/  "*                                                         *",
  /*6*/  "*                 O      O                                *",
  /*7*/  "*         C                                  O            *",
  /*8*/  "*    C                                                    *",
  /*9*/  "*   O O         C       C  C  C                           *",
  /*0*/  "*                                                         *",
  /*1*/  "*         C                                               *",
  /*2*/  "*         C                          O   C                *",
  /*3*/  "*         C                          O                    *",
  /*4*/  "*         C                                               *",
  /*5*/  "*         C                                               *",
  /*6*/  "*         C                                               *",
  /*7*/  "*         C           OO                                  *",
  /*8*/  "*         C                                 O             *",
  /*9*/  "*         C                                               *",
  /*0*/  "*         P                       C                       *",
  /*1*/  "*                                                         *",
  /*2*/  "*                    C                                    *",
  /*3*/  "*                                        O                *",
  /*4*/  "*                                                         *",
  /*5*/  "*                                                         *",
  /*6*/  "*                                                C        *",
  /*7*/  "*             C                                           *",
  /*8*/  "*                                                         *",
  /*9*/  "***********************************************************",
};

void fieldInit() {
  drawBorder({ 0, 0, 64, 25 });
  icebergInit();
  coinInit();
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
	  if (y >= 13) {
		setBufferBgColor(x, y, seaBlueLight);
	  }
	  if (y >= 17) {
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

void setFieldBufferText(float x, float y, const char* text, Color color) {
  if (x < 1 || x > ScreenFieldWidth - 1 || y < 1 || y > ScreenFieldHeight - 1) {
	return;
  }
  setBufferText(x, y, text, color);
}