#pragma once

#include "color.h"

void titleInit();
void titleUpdate();
void titleRender();
void titleDestroy();

bool titleSceneFadeOut();

void boatForwardByWind();

void drawTitle();
void drawTitleBackground();
void drawHo(int x, int y, Color color);
void drawTsu(int x, int y, Color color);
void drawKa(int x, int y, Color color);
void drawI(int x, int y, Color color);
void drawTo(int x, int y);
void drawRe(int x, int y);
void drawJi(int x, int y);
void drawYa(int x, int y);
void drawNobashibo(int x, int y);
void drawSmallBoat();
void drawTitleCoin(int x, int y);
void showStartHint();