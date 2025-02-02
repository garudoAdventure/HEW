#pragma once
#include "mathStruct.h"
#include "color.h"

struct CirclePt {
  int x;
  int y;
  CirclePt* next;
};

struct CirclePtList {
  CirclePt* next;
};

void drawLine(Vector2 pt1, Vector2 pt2, const char* shape, Color color = { 255, 255, 255 });

void drawCircle(int r, Vector2 c, const char* shape, Color fgColor, Color bgColor);
void drawCirclePt(int x, int y, Vector2 center, const char* shape, Color fgColor, Color bgColor);
void drawBgCircle(int r, Vector2 center, Color bgColor);
void drawCircleBgPt(int x, int y, Vector2 center, Color bgColor);

CirclePtList* getCirclePt(int r, Vector2 center);
CirclePt* createCirclePt(int x, int y);
void insertCirclePt(CirclePtList* list, CirclePt* point);
void emptyCircleList(CirclePtList* list);