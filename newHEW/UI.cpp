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
  const int startX = 56;
  const char* word[5] = { "C", "O", "I", "N", ":" };
  const char* digits[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
  const char* digit = "0";
  const char* tenDigit = "0";
  for (int i = 0; i < 5; i++) {
	setBuffer(startX + i, 1, word[i], yellow, skyBlue);
  }
  if (num < 10) {
	setBuffer(startX + 6, 1, digits[num], yellow, skyBlue);
  } else {
	tenDigit = digits[num / 10];
	digit = digits[num % 10];
	setBuffer(startX + 5, 1, tenDigit, yellow, skyBlue);
	setBuffer(startX + 6, 1, digit, yellow, skyBlue);
  }
}