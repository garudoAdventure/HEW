#define CONIOEX
#include "conioex.h"
#include "field.h"
#include "mathStruct.h"
#include "buffer.h"

void fieldInit() {
  renderMap();
}

void fieldUpdate() {

}

void fieldRender() {
}

void fieldDestroy() {

}

void renderMap() {
  for (int i = 1; i < 32; i++) {
	for (int j = 1; j < 13; j++) {
	  Color color = { 255, 255, 255 };
	  if (i > 8) {
		color = { 135, 135, 135 };
		if (j < 2) {
		  continue;
		}
	  }
	  if (i > 16) {
		color = { 75, 75, 75 };
		if (j < 3) {
		  continue;
		}
	  }
	  setBufferText(i, j, "█");
	  setBufferFgColor(i, j, color);

	  setBufferText(i, 24 - j, "█");
	  setBufferFgColor(i, 24 - j, color);

	  setBufferText(63 - i, j, "█");
	  setBufferFgColor(63 - i, j, color);

	  setBufferText(63 - i, 24 - j, "█");
	  setBufferFgColor(63 - i, 24 - j, color);
	}
  }
}