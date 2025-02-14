#pragma once
#include "color.h"
#include "gameMath.h"

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

void drawSmallBoat(int x, int y);
void drawMiddleBoat(int x, int y);
void drawBigBoat(int x, int y);

void setFieldBuffer(int x, int y, const char* shape, Color color);
Vector2* getStoneCoord();