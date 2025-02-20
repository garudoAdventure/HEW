#define CONIOEX
#include "conioex.h"
#include "buffer.h"
#include "console.h"
#include "iostream"
#include "gameMath.h"
#include "gameField.h"

const int bufferXSize = FIELD_W;
const int bufferYSize = FIELD_H;

Buffer bufDisplay[bufferYSize][bufferXSize];
Buffer bufRender[bufferYSize][bufferXSize];

void bufferInit() {
  for (int i = 0; i < bufferYSize; i++) {
	for (int j = 0; j < bufferXSize; j++) {
	  bufRender[i][j].text = "  ";
	  bufRender[i][j].fgColor = white;
	  bufRender[i][j].bgColor = black;
	  bufDisplay[i][j].text = bufRender[i][j].text;
	  bufDisplay[i][j].fgColor = bufRender[i][j].fgColor;
	  bufDisplay[i][j].bgColor = bufRender[i][j].bgColor;
	}
  }
}

void bufferUpdate() {

}

void bufferRender() {
  for (int i = 0; i < bufferYSize; i++) {
	for (int j = 0; j < bufferXSize; j++) {
	  if (
		bufDisplay[i][j].text == bufRender[i][j].text &&
		bufDisplay[i][j].fgColor.r == bufRender[i][j].fgColor.r &&
		bufDisplay[i][j].fgColor.g == bufRender[i][j].fgColor.g &&
		bufDisplay[i][j].fgColor.b == bufRender[i][j].fgColor.b &&
		bufDisplay[i][j].bgColor.r == bufRender[i][j].bgColor.r &&
		bufDisplay[i][j].bgColor.g == bufRender[i][j].bgColor.g &&
		bufDisplay[i][j].bgColor.b == bufRender[i][j].bgColor.b
		) {
		continue;
	  }
	  gotoxy(j + 1, i + 1);
	  setTextColor(bufRender[i][j].fgColor);
	  setTextBGColor(bufRender[i][j].bgColor);
	  std::cout << bufRender[i][j].text;
	  bufDisplay[i][j] = bufRender[i][j];

	  setTextColor(white);
	  setTextBGColor(black);
	}
  }
}

void bufferDestroy() {
  for (int i = 0; i < bufferYSize; i++) {
	for (int j = 0; j < bufferXSize; j++) {
	  bufRender[i][j].text = "  ";
	  bufRender[i][j].fgColor = { 255, 255, 255 };
	  bufRender[i][j].bgColor = { 0, 0, 0 };
	  bufDisplay[i][j].text = bufRender[i][j].text;
	  bufDisplay[i][j].fgColor = bufRender[i][j].fgColor;
	  bufDisplay[i][j].bgColor = bufRender[i][j].bgColor;
	}
  }
  setTextColor(white);
  setTextBGColor(black);
}

void setBuffer(float x, float y, const char* text, Color fgColor, Color bgColor) {
  int coorX = (int)x;
  int coorY = (int)y;
  if (
	coorX < 0 ||
	coorX >= bufferXSize ||
	coorY < 0 ||
	coorY >= bufferYSize
	) {
	return;
  }
  bufRender[coorY][coorX].text = text;
  bufRender[coorY][coorX].fgColor = fgColor;
  bufRender[coorY][coorX].bgColor = bgColor;
}

void setBufferText(float x, float y, const char* text, Color color) {
  int coorX = (int)x;
  int coorY = (int)y;
  if (
	coorX < 0 ||
	coorX >= bufferXSize ||
	coorY < 0 ||
	coorY >= bufferYSize
	) {
	return;
  }
  bufRender[coorY][coorX].fgColor = color;
  bufRender[coorY][coorX].text = text;
}

void setBufferTextAndColor(float x, float y, const char* text, Color color) {
  int coorX = (int)x;
  int coorY = (int)y;
  if (
	coorX < 0 ||
	coorX >= bufferXSize ||
	coorY < 0 ||
	coorY >= bufferYSize
	) {
	return;
  }
  bufRender[coorY][coorX].fgColor = color;
  bufRender[coorY][coorX].text = text;
}

void setBufferFgColor(float x, float y, Color color) {
  int coorX = (int)x;
  int coorY = (int)y;
  if (
	coorX < 0 ||
	coorX > bufferXSize ||
	coorY < 0 ||
	coorY > bufferYSize
	) {
	return;
  }
  bufRender[coorY][coorX].fgColor = color;
}

void setBufferBgColor(float x, float y, Color color) {
  int coorX = (int)x;
  int coorY = (int)y;
  if (
	coorX < 0 ||
	coorX >= bufferXSize ||
	coorY < 0 ||
	coorY >= bufferYSize
	) {
	return;
  }
  bufRender[coorY][coorX].bgColor = color;
}

void setFieldBufferText(float x, float y, const char* text, Color color) {
  if (x < 1 || x > ScreenFieldWidth - 1 || y < 1 || y > ScreenFieldHeight - 1) {
	return;
  }
  setBufferText(x, y, text, color);
}