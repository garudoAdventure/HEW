#pragma once
#include "gameMath.h"

enum GameItem {
  FAN,
  COMPASS,
  GUN
};

void gameInit();
void gameUpdate();
void gameRender();
void gameDestroy();

void setGameItemActive();
int getGameElapsedTime();
bool showGameStart();
bool showGameEnd();