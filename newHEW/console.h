#pragma once

#include "mathStruct.h"

void consoleInit(int width, int height, int fontSize);
void consoleEnd();

void enableVTMode();
void disableVTMode();
void resetConsoleTitle();
void setTextColor(Color color);
void setTextBGColor(Color color);
void resetTextColor();
void resetTextBGColor();
void setConsoleWindowSize(int width, int height);
void resetConsoleWindowSize();
void setScreenBufferSize(int width, int height);
void resetScreenBufferSize();
void setConsoleWindowPosition(int x, int y);
void resetConsoleWindowPosition();
void setConsoleFontType(const wchar_t* fontName);
void resetConsoleFontType();
void setConsoleFontSize(int size);
void resetConsoleFontSize();
void displayConsoleSettings();

void setConsoleBuffer(int x, int y, short c = 0x2588, short col = 0x000F);
void clearConsoleBuffer();
void renderConsoleBuffer();