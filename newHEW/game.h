#pragma once
#include "gameMath.h"

enum ControlMode {
  MIC,
  KEYBOARD
};

void gameInit();
void gameUpdate();
void gameRender();
void gameDestroy();

ControlMode getControlMode();
void setControlMode(ControlMode mode);

bool gameCountdown();

int getGameElapsedTime();
int getCountdown();
bool showGameStart();
bool showGameEnd();