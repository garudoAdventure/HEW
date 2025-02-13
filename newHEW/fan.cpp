#define CONIOEX
#include "conioex.h"
#include "fan.h"
#include "UI.h"
#include "buffer.h"
#include "gameMath.h"

const int fanCenterX = 65 + 6;
const int fanCenterY = 1 + 2;

const Vector2 fanBladeRotate[5][4] = {
  { {-2, 0}, {-2, 1}, { 0, -1 }, {-2, -1} },
  { {2, 0}, {2, -1}, {0, 1}, {2, 1} },
  { {0, -2}, {2, -2}, {4, -1}, {4, 0} },
  { {4, 1}, {2, 2}, {0, 2}, {-2, 2} },
  { {-4, 1}, {-4, 0}, {-4, -1}, {-2, -2} }
};

void fanInit() {
  renderBorder(64, 0, 16, 8);
  setBufferText(fanCenterX, fanCenterY, "◎");
}

void fanUpdate() {
  static int count = 0;
  static int frame = 0;
  if (frame < 40) {
	frame++;
	return;
  }
  frame = 0;

  Color color = { 255, 255, 255 };
  for (int i = 0; i < 5; i++) {
	int key = count;
	int preKey = count - 1 < 0 ? 3 : count - 1;
	if (i < 2) {
	  color = { 255, 255, 0 };
	} else {
	  color = { 0, 255, 255 };
	}
	setBufferText(fanCenterX + fanBladeRotate[i][preKey].x, fanCenterY + fanBladeRotate[i][preKey].y, "  ");
	setBufferText(fanCenterX + fanBladeRotate[i][key].x, fanCenterY + fanBladeRotate[i][key].y, "■", color);
  }
  count = (count + 1) % 4;
}

void fanRender() {

}

void fanDestroy() {

}