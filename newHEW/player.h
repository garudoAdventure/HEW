#pragma once
#include "gameMath.h"

enum BoatWave {
  BOAT_UP,
  BOAT_DOWN
};

struct Player {
  Vector2f pos;
  Vector2f dir;
  float viewAngle;
};

void playerInit();
void playerUpdate();
void playerRender();
void playerDestroy();

Player* getPlayer();
float getPlayerVelocity();
void playerMove();
void drawMyBoat();
void showPlayerPos();
void setPlayerVelocity(float v);
void setPlayerRotate(float angle);
bool isHitwall();
int getCollectCoinNum();
BoatWave getBoatWave();
void drawSpray();