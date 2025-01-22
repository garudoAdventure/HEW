#include "UI.h"
#include "buffer.h"

void renderBorder(int x, int y, int w, int h) {
  for (int i = 0; i < w; i++) {
	for (int j = 0; j < h; j++) {
	  if (i != 0 && i != w - 1 && j != 0 && j != h - 1) {
		continue;
	  }
	  if (i == 0) {
		if (j == 0) {
		  setBufferText(i + x, j + y, "╔");
		}
		else if (j == h - 1) {
		  setBufferText(i + x, j + y, "╚");
		}
		else {
		  setBufferText(i + x, j + y, "║");
		}
	  }
	  else if (i == w - 1) {
		if (j == 0) {
		  setBufferText(i + x, j + y, "╗");
		}
		else if (j == h - 1) {
		  setBufferText(i + x, j + y, "╝");
		}
		else {
		  setBufferText(i + x, j + y, "║");
		}
	  }
	  else {
		setBufferText(i + x, j + y, "═");
	  }
	}
  }
}