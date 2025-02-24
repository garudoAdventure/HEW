#pragma once
#include "gameMath.h"
#include "color.h"

void drawBorder(Rect4 rect, Color color = {255, 255, 255});
void drawBracketBorder(Rect4 rect, Color color = { 255, 255, 255 });
void showGetCoinNum(int num);
void showLifeBar(int countdown, int max);