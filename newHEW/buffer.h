#pragma once
#include "gameMath.h"
#include "color.h"

struct Buffer {
  const char* text;
  Color fgColor;
  Color bgColor;
};

void bufferInit();
void bufferUpdate();
void bufferRender();
void bufferDestroy();

void setBuffer(float x, float y, const char* text, Color fgColor, Color bgColor);
void setBufferText(float x, float y, const char* text, Color color = {255, 255, 255});
void setBufferTextAndColor(float x, float y, const char* text, Color color);
void setBufferFgColor(float x, float y, Color color);
void setBufferBgColor(float x, float y, Color color);