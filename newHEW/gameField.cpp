#define CONIOEX
#include "conioex.h"
#include "gameField.h"
#include "buffer.h"
#include "UI.h"
#include "player.h"
#include "game.h"
#include "draw.h"
#include "gameMath.h"
#include "iostream"

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

Vector2 stoneCoord[100] = {};
Vector2 coinCoord[100] = {};
int stoneNum;
int coinNum;

const int coinTurnSpd = 70;

bool isExplode = false;
Vector2f explodePos[23][55] = { 0 };
Vector2f explodeVec[23][55];
Vector2 explodedIceberg = { 0, 0 };

void fieldInit() {
  drawBorder({ 0, 0, 64, 25 });
  stoneNum = 0;
  for (int i = GameFieldWidth - 1; i >= 0 ; i--) {
	for (int j = GameFieldHeight - 1; j >= 0; j--) {
	  if (map[j][i] == 'O') {
		stoneCoord[stoneNum] = { i, j };
		stoneNum++;
	  }
	  if (map[j][i] == 'C') {
		coinCoord[coinNum] = { i, j };
		coinNum++;
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
  const Player* player = getPlayer();

  const Vector3 sunCoord = { player->pos.x, -0.3f, player->pos.y - 1.0f };

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
	Vector3 proStineCenter = transformToProCoord(viewStoneCenter);
	if (0.0f <= stoneVertex[0].z && stoneVertex[0].z <= 1.0f) {
	  //gotoxy(65, 5);
	  //std::cout << "               ";
	  //gotoxy(65, 5);
	  //std::cout << "depth: " << stoneVertex[0].z;
	  //drawSquare(stoneVertex, white);
	  drawIceberg({ (int)proStineCenter.x , (int)proStineCenter.y }, stoneVertex[0].z);
	}
  }

  for (int i = 0; i < coinNum; i++) {
	if (coinCoord[i].x == -1 && coinCoord[i].y == -1) {
	  continue;
	}
	// Transform to View Coord
	const Vector3 coinCenter = { coinCoord[i].x + 0.5f, 0.1f, coinCoord[i].y};

	Vector3 viewCoinCenter = transformToViewCoord(*player, coinCenter);
	viewCoinCenter.z += 1.0f;
	Vector3 proCoinCenter = transformToProCoord(viewCoinCenter);
	if (0.0f <= proCoinCenter.z && proCoinCenter.z <= 1.0f) {
	  drawCoin({ (int)proCoinCenter.x , (int)proCoinCenter.y }, proCoinCenter.z);
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

char getMapCoordEle(int x, int y) {
  return map[y][x];
}

void setMapCoordEle(int x, int y, char text) {
  map[y][x] = text;
}

int getStoneNum() {
  return stoneNum;
}

Vector2* getStoneCoord() {
  return stoneCoord;
}

int getCoinNum() {
  return coinNum;
}

Vector2* getCoinCoord() {
  return coinCoord;
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
  if (x < 1 || x > ScreenFieldWidth - 1 || y < 1 || y > ScreenFieldHeight - 1) {
	return;
  }
  setBufferText(x, y, text, color);
}

void drawCoin(Vector2 center, float depth) {
  if (depth < 0.6f) {
	drawCoin7x5(center);
  }
  else if (depth < 0.83f) {
	drawCoin5x4(center);
  }
  else if (depth < 0.92f) {
	drawCoin3x3(center);
  }
  else if (depth < 0.985f) {
	drawCoin3x1(center);
  }
  else if (depth < 0.99f) {
	drawCoin1x1(center);
  }
}

void drawCoin7x5(Vector2 center) {
  static int frame = 0;
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 7;
  const int height = 5;
  const char* coinFront[height][width] = {
	{" ", "Y", "Y", "Y", "Y", "Y", " "},
	{"Y", "Y", "Y", "M", "Y", "Y", "Y"},
	{"Y", "Y", "Y", "M", "Y", "Y", "Y"},
	{"Y", "Y", "Y", "M", "Y", "Y", "Y"},
	{" ", "Y", "Y", "Y", "Y", "Y", " "},
  };
  const char* coinSide[height][width] = {
	{" ", " ", " ", "Y", " ", " ", " "},
	{" ", " ", " ", "Y", " ", " ", " "},
	{" ", " ", " ", "Y", " ", " ", " "},
	{" ", " ", " ", "Y", " ", " ", " "},
	{" ", " ", " ", "Y", " ", " ", " "},
  };
  char* coinTemp[height][width];
  if (frame < coinTurnSpd) {
	memcpy(coinTemp, coinFront, sizeof(coinTemp));
  } else {
	memcpy(coinTemp, coinSide, sizeof(coinTemp));
  }

  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coinTemp[j][i] != " ") {
		if (coinTemp[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coinTemp[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
  frame++;
  if (frame == coinTurnSpd * 2) {
	frame = 0;
  }
}

void drawCoin5x4(Vector2 center) {
  static int frame = 0;
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 5;
  const int height = 4;
  const char* coinFront[height][width] = {
	{" ", "Y", "Y", "Y", " "},
	{"Y", "Y", "M", "Y", "Y"},
	{"Y", "Y", "M", "Y", "Y"},
	{" ", "Y", "Y", "Y", " "},
  };
  const char* coinSide[height][width] = {
	{" ", " ", "Y", " ", " "},
	{" ", " ", "Y", " ", " "},
	{" ", " ", "Y", " ", " "},
	{" ", " ", "Y", " ", " "},
  };
  char* coinTemp[height][width];
  if (frame < coinTurnSpd) {
	memcpy(coinTemp, coinFront, sizeof(coinTemp));
  }
  else {
	memcpy(coinTemp, coinSide, sizeof(coinTemp));
  }
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coinTemp[j][i] != " ") {
		if (coinTemp[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coinTemp[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
  frame++;
  if (frame == coinTurnSpd * 2) {
	frame = 0;
  }
}

void drawCoin3x3(Vector2 center) {
  static int frame = 0;
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 3;
  const int height = 3;
  const char* coinFront[height][width] = {
	{" ", "Y", " "},
	{"Y", "M", "Y"},
	{" ", "Y", " "},
  };
  const char* coinSide[height][width] = {
	{" ", "Y", " "},
	{" ", "Y", " "},
	{" ", "Y", " "},
  };
  char* coinTemp[height][width];
  if (frame < coinTurnSpd) {
	memcpy(coinTemp, coinFront, sizeof(coinTemp));
  }
  else {
	memcpy(coinTemp, coinSide, sizeof(coinTemp));
  }
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coinTemp[j][i] != " ") {
		if (coinTemp[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coinTemp[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
  frame++;
  if (frame == coinTurnSpd * 2) {
	frame = 0;
  }
}

void drawCoin3x2(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 3;
  const int height = 2;
  const char* coin[height][width] = {
	{"Y", "M", "Y"},
	{"Y", "M", "Y"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coin[j][i] != " ") {
		if (coin[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coin[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawCoin3x1(Vector2 center) {
  static int frame = 0;
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 3;
  const int height = 1;
  const char* coinFront[height][width] = {
	{"Y", "M", "Y"},
  };
  const char* coinSide[height][width] = {
	{" ", "Y", " "},
  };
  char* coinTemp[height][width];
  if (frame < coinTurnSpd) {
	memcpy(coinTemp, coinFront, sizeof(coinTemp));
  }
  else {
	memcpy(coinTemp, coinSide, sizeof(coinTemp));
  }
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coinTemp[j][i] != " ") {
		if (coinTemp[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coinTemp[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawCoin1x1(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 1;
  const int height = 1;
  const char* coin[height][width] = {
	{ "Y" }
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coin[j][i] != " ") {
		if (coin[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coin[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void explodeIceberg(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 55;
  const int height = 23;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*11*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
	/*12*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
	/*13*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
	/*14*/{" ", " ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*15*/{" ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*16*/{" ", " ", " ", " ", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*17*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", " ", " ", " "},
	/*18*/{" ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", " ", " ", " "},
	/*19*/{" ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*20*/{" ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*21*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*22*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };

  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  explodePos[j][i].x += explodeVec[j][i].x;
	  explodePos[j][i].y += explodeVec[j][i].y;
	}
  }

  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		int newX = centerX - width / 2 + i + explodePos[j][i].x;
		int newY = centerY - height / 2 + j + explodePos[j][i].y;
		setFieldBufferText(newX, newY, "█", color);
	  }
	}
  }
}

void drawIceberg(Vector2 center, float depth) {
  static int explodeFrame = 0;
  if (isExplode) {
	explodeIceberg(center);
	explodeFrame++;
	if (explodeFrame > 100) {
	  isExplode = false;
	  explodeFrame = 0;
	  memset(explodePos, 0, sizeof(explodePos));
	  for (int i = 0; i < stoneNum; i++) {
		if (stoneCoord[i].x == explodedIceberg.x && stoneCoord[i].y == explodedIceberg.y) {
		  stoneCoord[i].x = -1;
		  stoneCoord[i].y = -1;
		  break;
		}
	  }
	  setMapCoordEle(explodedIceberg.x, explodedIceberg.y, ' ');
	}
	return;
  }
  if (depth < 0.83f) {
	drawIceberg63x23(center);
  }
  else if (depth < 0.84f) {
	drawIceberg59x23(center);
  }
  else if (depth < 0.85f) {
	drawIceberg55x23(center);
  }
  else if (depth < 0.86f) {
	drawIceberg53x22(center);
  }
  else if (depth < 0.875f) {
	drawIceberg51x20(center);
  }
  else if (depth < 0.89f) {
	drawIceberg47x18(center);
  }
  else if (depth < 0.9f) {
	drawIceberg43x16(center);
  }
  else if (depth < 0.92f) {
	drawIceberg39x14(center);
  }
  else if (depth < 0.925f) {
	drawIceberg35x12(center);
  }
  else if (depth < 0.93f) {
	drawIceberg31x11(center);
  }
  else if (depth < 0.935f) {
	drawIceberg25x10(center);
  }
  else if (depth < 0.94f) {
	drawIceberg23x8(center);
  }
  else if (depth < 0.945f) {
	drawIceberg21x7(center);
  }
  else if (depth < 0.95f) {
	drawIceberg19x7(center);
  }
  else if (depth < 0.955f) {
	drawIceberg17x6(center);
  }
  else if (depth < 0.96f) {
	drawIceberg15x5(center);
  }
  else if (depth < 0.965f) {
	drawIceberg13x5(center);
  }
  else if (depth < 0.97f) {
	drawIceberg11x4(center);
  }
  else if (depth < 0.975f) {
	drawIceberg9x4(center);
  }
  else if (depth < 0.98f) {
	drawIceberg7x3(center);
  }
  else if (depth < 0.985f) {
	drawIceberg5x3(center);
  }
  else if (depth < 0.99f) {
	drawIceberg3x3(center);
  }
  else if (depth < 0.991f) {
	drawIceberg3x2(center);
  }
  else if (depth < 0.992f) {
	drawIceberg3x1(center);
  }
  else if (depth < 0.993f) {
	drawIceberg2x1(center);
  }
  else if (depth < 0.994f) {
	drawIceberg1x1(center);
  }
}

void drawIceberg63x23(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 63;
  const int height = 23;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54	 55	  56   57   58   59   60   61   62
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*11*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*12*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
	/*13*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
	/*14*/{" ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*15*/{" ", " ", " ", " ", " ", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*16*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", " ", " ", " "},
	/*17*/{" ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", " ", " ", " "},
	/*18*/{" ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*19*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*20*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*21*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*22*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg59x23(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 59;
  const int height = 23;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54	 55	  56   57   58   59   60   61   62
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*11*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*12*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
	/*13*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*14*/{" ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*15*/{" ", " ", " ", " ", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*16*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", " ", " ", " "},
	/*17*/{" ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", " ", " ", " "},
	/*18*/{" ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*19*/{" ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*20*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*21*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*22*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg55x23(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 55;
  const int height = 23;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
   /*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
   /*11*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
   /*12*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
   /*13*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
   /*14*/{" ", " ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
   /*15*/{" ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
   /*16*/{" ", " ", " ", " ", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
   /*17*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", " ", " ", " "},
   /*18*/{" ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", " ", " ", " "},
   /*19*/{" ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
   /*20*/{" ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
   /*21*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
   /*22*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg53x22(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 53;
  const int height = 22;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},	
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
   /*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
   /*11*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
   /*12*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
   /*13*/{" ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*14*/{" ", " ", " ", " ", " ", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*15*/{" ", " ", " ", " ", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*16*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", " ", " ", " "},
	/*17*/{" ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*18*/{" ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*19*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*20*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*21*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg51x20(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 51;
  const int height = 20;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
   /*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
   /*11*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*12*/{" ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*13*/{" ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*14*/{" ", " ", " ", " ", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", " ", " ", " "},
	/*15*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", " ", " ", " "},
	/*16*/{" ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*17*/{" ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*18*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*19*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg47x18(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 47;
  const int height = 18;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "W", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "W", "W", "W", " ",  " ", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
	/*10*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*11*/{" ", " ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*12*/{" ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*13*/{" ", " ", " ", " ", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", " ", " ", " "},
	/*14*/{" ", " ", " ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", " ", " ", " "},
	/*15*/{" ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*16*/{" ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*17*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg43x16(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 43;
  const int height = 16;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", " ", " ", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "W", "W", "W", " ", " ", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*10*/{" ", " ", " ", " ", " ", " ", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*11*/{" ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", " ", " ", " "},
	/*12*/{" ", " ", " ", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*13*/{" ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " ", " "},
	/*14*/{" ", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*15*/{"S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg39x14(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 39;
  const int height = 14;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", " ", " ", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "W", "W", "W", " ", " ", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " ", " "},
	/*8*/{" ", " ", " ", " ", " ", " ", "B", "B", "B", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*9*/{" ", " ", " ", " ", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", "W", "W", "W", "S", " ", " ", " "},
	/*10*/{" ", " ", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", " ", " "},
	/*11*/{" ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "W", "W", "W", "W", "W", "W", "W", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", " "},
	/*12*/{" ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "W", "W", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*13*/{"S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg35x12(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 35;
  const int height = 12;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "W", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "W", "W", "W", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", " ", "B", "B", "B", "B", "S", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "B", "B", "W", "W", "W", "W", "W", "W", "W", "W", " ", " ", " "},
	/*8*/{" ", " ", "S", "S", "B", "B", "B", "B", "B", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "W", "S", "S", " ", " "},
	/*9*/{" ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "S", "W", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "W", "W", "S", "S", "S", "S", "S", "S", " "},
	/*10*/{" ", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "W", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S", "S", " "},
	/*11*/{"S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg31x11(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 31;
  const int height = 11;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "B", "B", "B", "W", " ", " ", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", " ", " ", "S", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", " ", " ", " ", " ", " "},
	/*6*/{" ", " ", " ", " ", " ", "B", "S", "S", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", "W", " ", " ", " ", " "},
	/*7*/{" ", " ", " ", "B", "B", "B", "B", "B", "S", "S", "S", "W", "S", "S", "S", "S", "S", "S", "S", "S", "B", "S", "W", "W", "W", "W", "W", "W", " ", " ", " "},
	/*8*/{" ", "S", "S", "S", "S", "B", "B", "B", "B", "S", "W", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", " ", " "},
	/*9*/{" ", "S", "S", "S", "S", "B", "B", "B", "B", "B", "W", "W", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S", " "},
	/*10*/{"S", "S", "S", "S", "S", "S", "B", "B", "B", "B", "B", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg25x10(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 25;
  const int height = 10;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", " ", " ", "S", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "S", "B", "B", "W", "W", " ", " ", " ", " "},
	/*5*/{" ", " ", " ", " ", "B", "S", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", " ", " ", " "},
	/*6*/{" ", " ", "S", "B", "B", "B", "S", "S", "S", "W", "S", "S", "S", "S", "S", "B", "B", "S", "W", "W", "W", "W", "W", " ", " "},
	/*7*/{" ", "S", "S", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "B", "B", "B", "B", "S", "W", "W", "W", "S", "S", " "},
	/*8*/{" ", "S", "S", "S", "B", "B", "B", "B", "W", "W", "S", "S", "S", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S", " "},
	/*9*/{"S", "S", "S", "S", "S", "B", "B", "B", "B", "S", "S", "S", "B", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg23x8(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 23;
  const int height = 8;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
	/*0*/{" ", " ", " ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "B", "B", "B", " ", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", " ", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "S", "S", "S", "S", "S", "B", "B", "W", "W", " ", " ", " ", " "},
	/*4*/{" ", " ", " ", "B", "B", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "W", "W", "W", "W", "W", " ", " ", " "},
	/*5*/{" ", "S", "S", "B", "B", "B", "S", "W", "W", "W", "S", "S", "S", "B", "B", "B", "W", "W", "W", "W", "W", " ", " "},
	/*6*/{" ", "S", "S", "B", "B", "B", "B", "W", "W", "W", "S", "S", "S", "B", "B", "B", "S", "W", "W", "W", "S", "S", " "},
	/*7*/{"S", "S", "S", "S", "S", "B", "B", "B", "S", "S", "S", "S", "B", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg21x7(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 21;
  const int height = 7;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
	/*0*/{" ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", "S", "S", "S", "S", "S", "B", "B", "B", " ", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", "S", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", "B", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", "S", "S", "W", "W", "W", "S", "S", "S", "S", "S", "B", "W", "W", "W", " ", " ", " "},
	/*4*/{" ", " ", "B", "B", "B", "S", "S", "W", "S", "S", "S", "S", "S", "B", "S", "W", "W", "W", "W", " ", " "},
	/*5*/{" ", "S", "S", "B", "B", "B", "W", "W", "W", "W", "S", "S", "B", "B", "B", "S", "W", "W", "S", "S", " "},
	/*6*/{"S", "S", "S", "S", "S", "B", "B", "B", "S", "S", "S", "B", "B", "B", "B", "B", "S", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg19x7(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 19;
  const int height = 7;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18
	/*0*/{" ", " ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", " ", "S", "S", "S", "B", "B", "B", "B", " ", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", " ", "W", "W", "W", "W", "B", "B", "B", "B", "B", "B", " ", " ", " ", " "},
	/*3*/{" ", " ", " ", " ", "S", "S", "W", "W", "S", "S", "S", "S", "S", "S", "W", "W", " ", " ", " "},
	/*4*/{" ", " ", "B", "B", "B", "S", "S", "W", "S", "S", "S", "S", "B", "W", "W", "W", "W", " ", " "},
	/*5*/{" ", "S", "S", "B", "B", "B", "W", "W", "W", "S", "B", "B", "B", "S", "W", "W", "S", "S", " "},
	/*6*/{"S", "S", "S", "S", "B", "B", "B", "S", "S", "S", "B", "B", "B", "B", "B", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg17x6(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 17;
  const int height = 6;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
	/*0*/{" ", " ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", " ", "S", "S", "S", "B", "B", "B", "B", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", " ", "W", "W", "W", "W", "S", "S", "S", "B", "B", "W", " ", " ", " "},
	/*3*/{" ", " ", "B", "B", "S", "W", "W", "S", "S", "S", "S", "S", "W", "W", "W", " ", " "},
	/*4*/{" ", "S", "B", "B", "B", "W", "W", "S", "S", "B", "B", "B", "W", "W", "W", "S", " "},
	/*5*/{"S", "S", "S", "B", "B", "B", "S", "S", "S", "B", "B", "B", "B", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg15x5(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 15;
  const int height = 5;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14
	/*0*/{" ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", "S", "S", "S", "B", "B", "B", " ", " ", " ", " ", " "},
	/*2*/{" ", " ", " ", "S", "W", "W", "W", "S", "S", "S", "B", "W", "W", " ", " "},
	/*3*/{" ", "S", "B", "B", "S", "W", "W", "S", "B", "B", "W", "W", "W", "S", " "},
	/*4*/{"S", "S", "S", "B", "B", "S", "S", "B", "B", "B", "B", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg13x5(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 13;
  const int height = 5;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10   11   12
	/*0*/{" ", " ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", " ", "S", "S", "B", "B", "B", "B", " ", " ", " "},
	/*2*/{" ", " ", "B", "S", "W", "W", "S", "S", "S", "W", "W", " ", " "},
	/*3*/{" ", "S", "B", "B", "W", "W", "S", "B", "B", "W", "W", "S", " "},
	/*4*/{"S", "S", "S", "B", "B", "S", "S", "B", "B", "S", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg11x4(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 11;
  const int height = 4;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8    9   10
	/*0*/{" ", " ", " ", " ", "S", "S", " ", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", "W", "W", "B", "B", "B", "W", " ", " "},
	/*2*/{" ", "B", "S", "W", "S", "S", "S", "S", "W", "W", " "},
	/*3*/{"S", "S", "B", "B", "S", "S", "B", "B", "S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg9x4(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 9;
  const int height = 4;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6    7    8
	/*0*/{" ", " ", " ", " ", "S", " ", " ", " ", " "},
	/*1*/{" ", " ", " ", "W", "S", "B", "B", " ", " "},
	/*2*/{" ", "B", "S", "W", "S", "S", "W", "W", " "},
	/*3*/{"S", "S", "B", "S", "B", "B", "B", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg7x3(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 7;
  const int height = 3;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4    5    6
	/*0*/{" ", " ", " ", "S", " ", " ", " "},
	/*1*/{" ", "B", "W", "S", "S", "W", " "},
	/*2*/{"S", "B", "B", "S", "B", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg5x3(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 5;
  const int height = 3;
  const char* iceberg[height][width] = {
	//	   0    1    2    3    4
	/*0*/{" ", " ", "S", " ", " "},
	/*1*/{" ", "S", "S", "W", " "},
	/*2*/{"S", "B", "B", "B", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg3x3(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 3;
  const int height = 3;
  const char* iceberg[height][width] = {
	//	   0    1    2
	/*0*/{" ", "S", " "},
	/*1*/{"S", "S", "W"},
	/*2*/{"S", "B", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg3x2(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 3;
  const int height = 2;
  const char* iceberg[height][width] = {
	//	   0    1    2
	/*0*/{" ", "S", " "},
	/*1*/{"S", "B", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg3x1(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 3;
  const int height = 1;
  const char* iceberg[height][width] = {
	//	   0    1    2
	/*0*/{"S", "S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg2x1(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 2;
  const int height = 1;
  const char* iceberg[height][width] = {
	//	   0    1
	/*0*/{"S", "S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void drawIceberg1x1(Vector2 center) {
  const int centerX = center.x;
  const int centerY = center.y;
  const int width = 1;
  const int height = 1;
  const char* iceberg[height][width] = {
	//	   0
	/*0*/{"S"},
  };
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (iceberg[j][i] != " ") {
		if (iceberg[j][i] == "S") {
		  color = { 206, 242, 251 };
		}
		if (iceberg[j][i] == "B") {
		  color = { 151, 226, 244 };
		}
		setFieldBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
}

void setIcebergExplode(Vector2 icePos) {
  isExplode = true;
  explodedIceberg = icePos;
  // Prepare random vector
  for (int i = 0; i < 55; i++) {
	for (int j = 0; j < 23; j++) {
	  float randX = rand() % 200 - 100;
	  randX /= 50.0f;
	  float randY = rand() % 200 - 100;
	  randY /= 50.0f;
	  explodeVec[j][i] = { randX, randY };
	}
  }
}