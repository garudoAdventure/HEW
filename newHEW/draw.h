#pragma once
#include "mathStruct.h"

struct CirclePt {
  int x;
  int y;
  CirclePt* next;
};

struct CirclePtList {
  CirclePt* next;
};

void drawLine(Vector pt1, Vector pt2, const char* shape, Color color = { 255, 255, 255 });

void drawCircle(int r, Vector c, const char* shape, Color fgColor, Color bgColor);
void drawCirclePt(int x, int y, Vector center, const char* shape, Color fgColor, Color bgColor);
void drawBgCircle(int r, Vector center, Color bgColor);
void drawCircleBgPt(int x, int y, Vector center, Color bgColor);

CirclePtList* getCirclePt(int r, Vector center);
CirclePt* createCirclePt(int x, int y);
void insertCirclePt(CirclePtList* list, CirclePt* point);
void emptyCircleList(CirclePtList* list);