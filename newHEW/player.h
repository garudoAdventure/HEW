#pragma once
#include "mathStruct.h"

struct Player {
  int hp;
  Vector2f pos;
  Vector2f prePos;
  Color color;
};

void playerInit();
void playerUpdate();
void playerRender();
void playerDestroy();

Player* getPlayer();
void playerMove();