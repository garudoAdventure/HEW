#include "UI.h"
#include "buffer.h"

void drawBorder(Rect4 rect, Color color) {
  for (int i = 0; i < rect.w; i++) {
	for (int j = 0; j < rect.h; j++) {
	  if (i != 0 && i != rect.w - 1 && j != 0 && j != rect.h - 1) {
		continue;
	  }
	  if (i == 0) {
		if (j == 0) {
		  setBufferText(i + rect.x, j + rect.y, "╔", color);
		}
		else if (j == rect.h - 1) {
		  setBufferText(i + rect.x, j + rect.y, "╚", color);
		}
		else {
		  setBufferText(i + rect.x, j + rect.y, "║", color);
		}
	  }
	  else if (i == rect.w - 1) {
		if (j == 0) {
		  setBufferText(i + rect.x, j + rect.y, "╗", color);
		}
		else if (j == rect.h - 1) {
		  setBufferText(i + rect.x, j + rect.y, "╝", color);
		}
		else {
		  setBufferText(i + rect.x, j + rect.y, "║", color);
		}
	  }
	  else {
		setBufferText(i + rect.x, j + rect.y, "═", color);
	  }
	}
  }
}

void drawBracketBorder(Rect4 rect, Color color) {
  setBufferText(rect.x, rect.y, "█", color);
  setBufferText(rect.x + 1, rect.y, "▀", color);

  setBufferText(rect.x + rect.w - 1, rect.y, "█", color);
  setBufferText(rect.x + rect.w - 2, rect.y, "▀", color);

  setBufferText(rect.x, rect.y + rect.h - 1, "█", color);
  setBufferText(rect.x + 1, rect.y + rect.h - 1, "▄", color);

  setBufferText(rect.x + rect.w - 1, rect.y + rect.h - 1, "█", color);
  setBufferText(rect.x + rect.w - 2, rect.y + rect.h - 1, "▄", color);
}

void showGetCoinNum(int num) {
  const int startX = 65;
  const int startY = 23;
  const char* word[5] = { "C", "O", "I", "N", ":" };
  const char* digits[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
  const char* digit = "0";
  const char* tenDigit = "0";
  for (int i = 0; i < 5; i++) {
	setBufferText(startX + i, startY, word[i], yellow);
  }
  if (num < 10) {
	setBufferText(startX + 6, startY, digits[num], yellow);
  } else {
	tenDigit = digits[num / 10];
	digit = digits[num % 10];
	setBufferText(startX + 5, startY, tenDigit, yellow);
	setBufferText(startX + 6, startY, digit, yellow);
  }
}

void showLifeBar(int countdown, int max) {
  const Color srcColor = { 90, 171, 245 };
  const Color destColor = { 246, 51, 51 };
  const float dr = (destColor.r - srcColor.r) / max;
  const float dg = (destColor.g - srcColor.g) / max;
  const float db = (destColor.b - srcColor.b) / max;
  for (int i = 0; i < countdown; i++) {
	Color gradient;
	gradient.r = srcColor.r + dr * i;
	gradient.g = srcColor.g + dg * i;
	gradient.b = srcColor.b + db * i;
	setBufferText(2 + i, 2, "█", gradient);
  }
}