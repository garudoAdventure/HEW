#define CONIOEX
#include "conioex.h"
#include "gameField.h"
#include "buffer.h"
#include "UI.h"
#include "player.h"
#include "draw.h"
#include "iceberg.h"
#include "coin.h"
#include "game.h"

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

SeaObjList* seaObjList;
const int starNum = 50;
Vector2 starCoord[starNum];

void fieldInit() {
  drawBorder({ 0, 0, 64, 25 });
  icebergInit();
  coinInit();
  seaObjList = (SeaObjList*)malloc(sizeof(SeaObjList));
  seaObjList->next = NULL;
  
  for (int i = 0; i < starNum; i++) {
	int randX = rand() % 160 - 50;
	int randY = rand() % 5 + 1;
	starCoord[i] = { randX, randY };
  }
}

void fieldUpdate() {
  clearField();
  renderField();
}

void fieldRender() {
}

void fieldDestroy() {
  clearSeaObjList();
  icebergDestroy();
  coinDestroy();
  for (int i = 1; i < GameFieldWidth - 2; i++) {
	for (int j = 1; j < GameFieldHeight - 1; j++) {
	  setMapCoordEle(i, j, ' ');
	}
  }
 }

void renderField() {
  renderFieldBg();
  renderSun();
  renderBoundary();
  renderIceberg();
  renderCoin();

  SeaObjNode* node = seaObjList->next;
  while (seaObjList->next != NULL) {
	SeaObjNode* prevNode = NULL;
	SeaObjNode* maxZNode = node;
	SeaObjNode* maxZPrevNode = NULL;
	while (node != NULL) {
	  if (node->vec.z > maxZNode->vec.z) {
		maxZNode = node;
		maxZPrevNode = prevNode;
	  }
	  prevNode = node;
	  node = node->next;
	}
	if (maxZNode->type == ObjectType::ICE) {
	  drawIceberg({ (int)maxZNode->vec.x, (int)maxZNode->vec.y }, maxZNode->vec.z);
	}
	else {
	  drawCoin({ (int)maxZNode->vec.x, (int)maxZNode->vec.y }, maxZNode->vec.z);
	}
	// Free Node
	if (maxZPrevNode == NULL) {
	  seaObjList->next = maxZNode->next;
	} else {
	  maxZPrevNode->next = maxZNode->next;
	}
	free(maxZNode);
	node = seaObjList->next;
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

void renderFieldBg() {
  for (int x = 1; x < ScreenFieldWidth; x++) {
	for (int y = 1; y < ScreenFieldHeight; y++) {
	  if (y <= 12) {
		setBufferBgColor(x, y, getSkyBlue());
	  }
	  if (13 <= y && y <= 14) {
		setBufferBgColor(x, y, getSeaBlueLight());
	  }
	  if (y == 15) {
		setBufferBgColor(x, y, getSeaBlue58());
	  }
	  if (16 <= y && y <= 18) {
		setBufferBgColor(x, y, getSeaBlue50());
	  }
	  if (y == 19) {
		setBufferBgColor(x, y, getSeaBlue41());
	  }
	  if (20 <= y && y <= 23) {
		setBufferBgColor(x, y, getSeaBlueDark());
	  }
	}
  }
}

void renderSun() {
  const Player * player = getPlayer();
  if (getCountdown() > 20) {
	const float sunHeight = -0.3f + float((60 - getCountdown()) / 5) * 0.05f;
	const Vector3 sunCoord = { player->pos.x, sunHeight, player->pos.y - 1.0f };
	Vector3 sunViewCoord = transformToViewCoord(*player, sunCoord);
	Vector3 sunProCoord = transformToProCoord(sunViewCoord);
	if (0.0f <= sunProCoord.z && sunProCoord.z <= 1.0f) {
	  drawSun(sunProCoord);
	}
  }
  if (getCountdown() < 10) {
	for (int i = 0; i < starNum; i++) {
	  int starMove = sinf(player->viewAngle / 2) * 50;
	  int coordX = starCoord[i].x - starMove;
	  if (1 <= coordX && coordX <= ScreenFieldWidth - 1) {
		setBuffer(coordX, starCoord[i].y, "·", white, getSkyBlue());
	  }
	}
  }
  
}

void drawSun(Vector3 sunCenter) {
  const char* sun[5][9] = {
	{" ", " ", "▄", "▄", "█", "▄", "▄", " ", " "},
	{" ", "█", "█", "█", "█", "█", "█", "█", " "},
	{"█", "█", "█", "█", "█", "█", "█", "█", "█"},
	{" ", "█", "█", "█", "█", "█", "█", "█", " "},
	{" ", " ", "▀", "▀", "█", "▀", "▀", " ", " "},
  };
  
  for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = yellow;
	  if (j == 0 || j == 4 || i == 0 || i == 8 ||
		((i == 1 || i == 7) && (j == 1 || j == 3))
		 ) {
		color = orange;
	  }
	  if (sunCenter.y - 2 + j < 13) {
		setFieldBufferText(sunCenter.x - 4 + i, sunCenter.y - 2 + j, sun[j][i], color, getSkyBlue());
	  }
	}
  }
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

void insertToSeaObjList(SeaObjNode* node) {
  if (seaObjList->next == NULL) {
	seaObjList->next = node;
  } else {
	seaObjList->last->next = node;
  }
  seaObjList->last = node;
}

SeaObjNode* createSeaObjNode(ObjectType objType, Vector3 objVec) {
  SeaObjNode* node = (SeaObjNode*)malloc(sizeof(SeaObjNode));
  node->type = objType;
  node->vec = objVec;
  node->next = NULL;
  return node;
}

void clearSeaObjList() {
  SeaObjNode* node = seaObjList->next;
  while (node != NULL) {
	SeaObjNode* nextNode = node->next;
	free(node);
	node = nextNode;
  }
  free(seaObjList);
}