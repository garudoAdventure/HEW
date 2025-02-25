#pragma once
#include "gameMath.h"

enum GameItem {
  FAN,
  COMPASS,
  GUN
};

struct ViewData {
  Vector2 visionData[62] = { 0 };
  float distanceToObj[62] = { 0 };
};

void gameInit();
void gameUpdate();
void gameRender();
void gameDestroy();

void setGameItemActive();
int getGameElapsedTime();
bool showGameStart();
bool showGameEnd();