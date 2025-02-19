#pragma once

#include "gameMath.h"

struct CoinNode {
  Vector2 pos;
  CoinNode* next;
};

struct CoinList {
  CoinNode* next;
};

void coinInit();
void renderCoin();

CoinList* getCoinList();

void drawCoin(Vector2 center, float depth);
void drawCoin7x5(Vector2 center);
void drawCoin5x4(Vector2 center);
void drawCoin3x3(Vector2 center);
void drawCoin3x2(Vector2 center);
void drawCoin3x1(Vector2 center);
void drawCoin1x1(Vector2 center);