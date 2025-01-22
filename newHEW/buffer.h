#pragma once
#include "mathStruct.h"

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
void setBufferText(float x, float y, const char* text);
void setBufferFgColor(float x, float y, Color color);
void setBufferBgColor(float x, float y, Color color);