#pragma once
#include "color.h"
#include "gameMath.h"
#include "player.h"

#define GameFieldWidth	60
#define GameFieldHeight	30
#define ScreenFieldWidth  63.0f
#define ScreenFieldHeight 24.0f

const char map[GameFieldHeight][GameFieldWidth] = {
  "***********************************************************",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*            O                                            *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*        OO                                               *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "***********************************************************",
};

void fieldInit();
void fieldUpdate();
void fieldRender();
void fieldDestroy();

void renderField();
void clearField();

void drawSun(Vector3 sunCenter);
void setFieldBufferText(float x, float y, const char* text, Color color = {255, 255, 255});

Vector2* getStoneCoord();
Vector3 transformToViewCoord(Player player, Vector3 vec);
Vector3 transformToProCoord(Vector3 vec);