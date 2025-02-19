#define CONIOEX
#include "conioex.h"
#include "coin.h"
#include "gameField.h"
#include "player.h"

CoinList* coinList;

const int coinTurnSpd = 70;

void coinInit() {
  coinList = (CoinList*)malloc(sizeof(CoinList));
  coinList->next = NULL;

  CoinNode* lastCoin = NULL;
  for (int i = GameFieldWidth - 1; i >= 0; i--) {
	for (int j = GameFieldHeight - 1; j >= 0; j--) {
	  if (getMapCoordEle(i, j) == 'C') {
		CoinNode* coin = (CoinNode*)malloc(sizeof(CoinNode));
		coin->pos = { i, j };
		coin->next = NULL;
		if (coinList->next == NULL) {
		  coinList->next = coin;
		}
		else {
		  lastCoin->next = coin;
		}
		lastCoin = coin;
	  }
	}
  }
}

void renderCoin() {
  const Player* player = getPlayer();
  CoinNode* coinNode = coinList->next;
  while (coinNode != NULL) {
	// Transform to View Coord
	Vector3 coinCenter = { coinNode->pos.x + 0.5f, 0.1f, coinNode->pos.y };
	Vector3 viewCoinCenter = transformToViewCoord(*player, coinCenter);
	viewCoinCenter.z += 1.0f;
	// Transform to Projection Coord
	Vector3 proCoinCenter = transformToProCoord(viewCoinCenter);
	if (0.0f <= proCoinCenter.z && proCoinCenter.z <= 1.0f) {
	  drawCoin({ (int)proCoinCenter.x , (int)proCoinCenter.y }, proCoinCenter.z);
	}
	coinNode = coinNode->next;
  }
}

CoinList* getCoinList() {
  return coinList;
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