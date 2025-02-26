#pragma once

#include "gameMath.h"

struct IceNode {
  Vector2 pos;
  Vector3 proCoord;
  IceNode* next;
};

struct IceList {
  IceNode* next;
};

void icebergInit();
void icebergDestroy();
void renderIceberg();

IceList* getIceList();
int getDestroyIceNum();

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

void explodeIceberg();
void drawExplodeIceberg(Vector2 center);
void setIcebergExplode(IceNode* iceNode);