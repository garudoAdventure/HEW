#pragma once
#include "color.h"
#include "gameMath.h"
#include "player.h"

#define GameFieldWidth	60
#define GameFieldHeight	30
#define ScreenFieldWidth  63.0f
#define ScreenFieldHeight 24.0f

void fieldInit();
void fieldUpdate();
void fieldRender();
void fieldDestroy();

void renderField();
void clearField();

Vector3 transformToViewCoord(Player player, Vector3 vec);
Vector3 transformToProCoord(Vector3 vec);

char getMapCoordEle(int x, int y);
void setMapCoordEle(int x, int y, char text);

void renderBoundary();
void renderSun();
void drawSun(Vector3 sunCenter);

