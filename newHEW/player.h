#pragma once
#include "mathStruct.h"

struct Player {
  Vector2f pos;
  float viewAngle;
};

void playerInit();
void playerUpdate();
void playerRender();
void playerDestroy();

Player* getPlayer();
void playerMove();
void drawMyBoat();
void showPlayerPos();