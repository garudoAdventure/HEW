#include "console.h"
#include "gameMath.h"
#include <stdio.h>
#include <windows.h>

HANDLE hOut;
int screenWidth;
int screenHeight;
CHAR_INFO* bufScreen;

void consoleInit(int width, int height, int fontSize) {
  hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleOutputCP(65001);
}

void consoleEnd() {
  disableVTMode();
  resetConsoleTitle();
  resetTextColor();
  resetTextBGColor();
  resetConsoleWindowSize();
  resetScreenBufferSize();
  resetConsoleWindowPosition();
  resetConsoleFontType();
  resetConsoleFontSize();
}

void enableVTMode() {
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
}

void disableVTMode() {
  DWORD dwMode = 1;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
}

void resetConsoleTitle() {
  printf("\x1b]0;\x07");
}

void setTextColor(Color color) {
  printf("\x1b[38;2;%d;%d;%dm", color.r, color.g, color.b);
}

void setTextBGColor(Color color) {
  printf("\x1b[48;2;%d;%d;%dm", color.r, color.g, color.b);
}

void resetTextColor() {
  printf("\x1b[39m");
}

void resetTextBGColor() {
  printf("\x1b[49m");
}

void setConsoleWindowSize(int width, int height) {
  SMALL_RECT windowSize = { 0, 0, (short)width - 1, (short)height - 1 };
  if (!SetConsoleWindowInfo(hOut, TRUE, &windowSize)) {
	printf("NO window size");
  }
}

void resetConsoleWindowSize() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hOut, &csbi);
  SMALL_RECT windowSize = { 0, 0, csbi.dwSize.X - 1, csbi.dwSize.Y - 1 };
  SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

void setScreenBufferSize(int width, int height) {
  COORD newSize = { (short)width, (short)height };
  if (!SetConsoleScreenBufferSize(hOut, newSize)) {
	printf("NO buffer size");
  }
}

void resetScreenBufferSize() {
  COORD defaultSize = { 80, 25 }; // デフォルトのサイズに設定
  SetConsoleScreenBufferSize(hOut, defaultSize);
}

void setConsoleWindowPosition(int x, int y) {
  HWND hwnd = GetConsoleWindow();
  SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void resetConsoleWindowPosition() {
  HWND hwnd = GetConsoleWindow();
  SetWindowPos(hwnd, 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void setConsoleFontType(const wchar_t* fontName) {
  CONSOLE_FONT_INFOEX cfi;
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
  wcscpy_s(cfi.FaceName, LF_FACESIZE, fontName);
  SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

void resetConsoleFontType() {
  setConsoleFontType(L"Consolas");
}

void setConsoleFontSize(int size) {
  CONSOLE_FONT_INFOEX cfi;
  GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  cfi.nFont = 0;
  cfi.dwFontSize.Y = size;
  cfi.FontFamily = FF_DONTCARE;
  cfi.FontWeight = FW_NORMAL;
  SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

void resetConsoleFontSize() {
  setConsoleFontSize(16); // デフォルトのフォントサイズに設定
}

void displayConsoleSettings() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hOut, &csbi);
  printf("Screen Buffer Size: %d x %d\n", csbi.dwSize.X, csbi.dwSize.Y);
  printf("Window Size: %d x %d\n", csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
  printf("Window Position: (%d, %d)\n", csbi.srWindow.Left, csbi.srWindow.Top);
}

void setConsoleBuffer(int x, int y, short c, short col) {
  if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
	bufScreen[y * screenWidth + x].Char.UnicodeChar = c;
	bufScreen[y * screenWidth + x].Attributes = col;
  }
}

void renderConsoleBuffer() {
  SMALL_RECT windowSize = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
  WriteConsoleOutput(hOut, bufScreen, { (short)screenWidth, (short)screenHeight }, { 0,0 }, &windowSize);
}

void clearConsoleBuffer() {
  for (int x = 0; x < screenWidth; x++) {
	for (int y = 0; y < screenHeight; y++) {
	  setConsoleBuffer(x, y, 0x0020, 0x0000);
	}
  }
}