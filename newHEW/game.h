#pragma once
#include "gameMath.h"

void gameInit();
void gameUpdate();
void gameRender();
void gameDestroy();

bool gameCountdown();

int getGameElapsedTime();
int getCountdown();
bool showGameStart();
bool showGameEnd();