#define CONIOEX
#include "conioex.h"
#include "draw.h"
#include "buffer.h"
#include "console.h"

void drawLine(Vector2 pt1, Vector2 pt2, const char* shape, Color color) {
  bool steep = abs(pt2.y - pt1.y) > abs(pt2.x - pt1.x);
  int temp;
  if (steep) {
	temp = pt1.x;
	pt1.x = pt1.y;
	pt1.y = temp;
	temp = pt2.x;
	pt2.x = pt2.y;
	pt2.y = temp;
  }
  if (pt1.x > pt2.x) {
	temp = pt1.x;
	pt1.x = pt2.x;
	pt2.x = temp;
	temp = pt1.y;
	pt1.y = pt2.y;
	pt2.y = temp;
  }
  int dx = pt2.x - pt1.x;
  int dy = abs(pt2.y - pt1.y);
  float error = dx / 2;
  int yStep = pt2.y > pt1.y ? 1 : -1;
  int y = pt1.y;
  for (int x = pt1.x; x <= pt2.x; x++) {
	if (steep) {
	  // setConsoleBuffer(y, x, c);
	  setBufferText(y, x, shape);
	}
	else {
	  // setConsoleBuffer(x, y, c);
	  setBufferText(x, y, shape);
	}
	error -= dy;
	if (error < 0) {
	  y += yStep;
	  error += dx;
	}
  }
}

void drawCircle(int r, Vector2 center, const char* shape, Color fgColor, Color bgColor) {
  if (r < 1) {
	return;
  }
  const int radius = r + 0.5;
  float x = radius;
  float y = 0 + 0.5;
  while (y < x) {
	drawCirclePt(x - 0.5, y - 0.5, { center.x, center.y }, shape, fgColor, bgColor);
	y++;
	if ((x - 0.5) * (x - 0.5) + y * y > radius * radius) {
	  x--;
	}
  }
}

void drawCirclePt(int x, int y, Vector2 center, const char* shape, Color fgColor, Color bgColor) {
  Vector2 coor[4] = {
	{x, y}, {x, -y}, {-x, -y}, {-x, y}
  };
  for (int i = 0; i < 4; i++) {
	setBuffer(coor[i].x + center.x, coor[i].y + center.y, shape, fgColor, bgColor);
	setBuffer(coor[i].y + center.x, coor[i].x + center.y, shape, fgColor, bgColor);
  }
}

void drawBgCircle(int r, Vector2 center, Color bgColor) {
  if (r < 1) {
	return;
  }
  const int radius = r + 0.5;
  float x = radius;
  float y = 0 + 0.5;
  while (y < x) {
	drawCircleBgPt(x - 0.5, y - 0.5, { center.x, center.y }, bgColor);
	y++;
	if ((x - 0.5) * (x - 0.5) + y * y > radius * radius) {
	  x--;
	  drawCircleBgPt(x - 0.5, y - 0.5 - 1, { center.x, center.y }, bgColor);
	}
  }
}

void drawCircleBgPt(int x, int y, Vector2 center, Color bgColor) {
  Vector2 coor[4] = {
	{x, y}, {x, -y}, {-x, -y}, {-x, y}
  };
  for (int i = 0; i < 4; i++) {
	setConsoleBuffer(coor[i].x + center.x, coor[i].y + center.y);
	setConsoleBuffer(coor[i].y + center.x, coor[i].x + center.y);
  }
}

CirclePtList* getCirclePt(int r, Vector2 center) {
  CirclePtList* circlePtList = (CirclePtList*)malloc(sizeof(CirclePtList));
  circlePtList->next = NULL;
  if (r < 1) {
	free(circlePtList);
	return NULL;
  }
  const int radius = r + 0.5;
  float x = radius;
  float y = 0 + 0.5;
  while (y < x) {
	int ptX = x - 0.5;
	int ptY = y - 0.5;
	insertCirclePt(circlePtList, createCirclePt(ptX * 2 + center.x, ptY + center.y));
	insertCirclePt(circlePtList, createCirclePt(ptX * 2 + center.x, -ptY + center.y));
	insertCirclePt(circlePtList, createCirclePt(-ptX * 2 + center.x, -ptY + center.y));
	insertCirclePt(circlePtList, createCirclePt(-ptX * 2 + center.x, ptY + center.y));
	insertCirclePt(circlePtList, createCirclePt(ptY * 2 + center.x, ptX + center.y));
	insertCirclePt(circlePtList, createCirclePt(ptY * 2 + center.x, -ptX + center.y));
	insertCirclePt(circlePtList, createCirclePt(-ptY * 2 + center.x, -ptX + center.y));
	insertCirclePt(circlePtList, createCirclePt(-ptY * 2 + center.x, ptX + center.y));
	y++;
	if ((x - 0.5) * (x - 0.5) + y * y > radius * radius) {
	  x--;
	}
  }
  return circlePtList;
}

CirclePt* createCirclePt(int x, int y) {
  CirclePt* circlePt = (CirclePt*)malloc(sizeof(CirclePt));
  circlePt->next = NULL;
  circlePt->x = x;
  circlePt->y = y;
  return circlePt;
}

void insertCirclePt(CirclePtList* list, CirclePt* point) {
  if (list->next == NULL) {
	list->next = point;
  }
  else {
	point->next = list->next;
	list->next = point;
  }
}

void emptyCircleList(CirclePtList* list) {
  CirclePt* point = list->next;
  while (point != NULL) {
	CirclePt* nextPoint = point->next;
	free(point);
	point = nextPoint;
  }
  free(list);
}