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
  "*       O  O                      O                       *",
  "*                                                         *",
  "*                                                         *",
  "*                 O      O                                *",
  "*                                            O            *",
  "*                                                         *",
  "*   O O                 C                                 *",
  "*                                                         *",
  "*           C                                             *",
  "*                                    O                    *",
  "*                                    O                    *",
  "*                                                         *",
  "*                                                         *",
  "*                                                         *",
  "*                     OO                                  *",
  "*                                           O             *",
  "*                                                         *",
  "*                                 C                       *",
  "*                                                         *",
  "*                    C                                    *",
  "*                                        O                *",
  "*                                                         *",
  "*                                                         *",
  "*                                                C        *",
  "*             C                                           *",
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

void drawCoin(Vector2 center);
void drawIceberg(Vector2 center, float depth);

void drawIceberg63x23(Vector2 center);
void drawIceberg59x23(Vector2 center);
void drawIceberg55x23(Vector2 center);
void drawIceberg53x22(Vector2 center);
void drawIceberg51x20(Vector2 center);
void drawIceberg47x18(Vector2 center);
void drawIceberg43x16(Vector2 center);
void drawIceberg39x14(Vector2 center);
void drawIceberg35x12(Vector2 center);
void drawIceberg31x11(Vector2 center);
void drawIceberg25x10(Vector2 center);
void drawIceberg23x8(Vector2 center);
void drawIceberg21x7(Vector2 center);
void drawIceberg19x7(Vector2 center);
void drawIceberg17x6(Vector2 center);
void drawIceberg15x5(Vector2 center);
void drawIceberg13x5(Vector2 center);
void drawIceberg11x4(Vector2 center);
void drawIceberg9x4(Vector2 center);
void drawIceberg7x3(Vector2 center);
void drawIceberg5x3(Vector2 center);
void drawIceberg3x3(Vector2 center);
void drawIceberg3x2(Vector2 center);
void drawIceberg3x1(Vector2 center);
void drawIceberg2x1(Vector2 center);
void drawIceberg1x1(Vector2 center);