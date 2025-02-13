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