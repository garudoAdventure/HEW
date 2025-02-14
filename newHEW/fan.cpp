#define CONIOEX
#include "conioex.h"
#include "fan.h"
#include "UI.h"
#include "buffer.h"
#include "gameMath.h"
#include "player.h"

const int fanCenterX = 65 + 6;
const int fanCenterY = 1 + 2;

bool isFanActive = false;

const Vector2 fanBladeRotate[3][4] = {
 /* { {-2, 0}, {-2, 1}, { 0, -1 }, {-2, -1} },
  { {2, 0}, {2, -1}, {0, 1}, {2, 1} },*/
  { {0, -2}, {2, -2}, {4, -1}, {4, 0} },
  { {4, 1}, {2, 2}, {0, 2}, {-2, 2} },
  { {-4, 1}, {-4, 0}, {-4, -1}, {-2, -2} }
};

void fanInit() {
  drawBorder({ 64, 0, 16, 8 });
}

void fanUpdate() {
  static int frame = 0;
  if (frame < 40) {
	frame++;
  } else {
	frame = 0;
	clearFanScene();
	drawFan();
  }

  if (isFanActive) {
	drawBracketBorder({ 65, 1, 14, 6 }, yellow);
	drawFlag();

	if (inport(PK_W)) {
	  // velocity = velocity > maxV ? maxV : velocity + acceleration;
	  const float velocity = 0.01f;
	  setPlayerVelocity(velocity);
	}
	if (inport(PK_S)) {
	  // velocity = velocity > maxV ? maxV : velocity + acceleration;
	  const float velocity = -0.01f;
	  setPlayerVelocity(velocity);
	}
  }
}

void fanRender() {

}

void fanDestroy() {

}

void drawFan() {
  static int key = 0;
  setBufferText(fanCenterX, fanCenterY, "◎");

  Color color = isHitwall() ? red : blue;
  for (int i = 0; i < 3; i++) {
	setBufferText(fanCenterX + fanBladeRotate[i][key].x, fanCenterY + fanBladeRotate[i][key].y, "■", color);
  }
  key = (key + 1) % 4;
}

void clearFanScene() {
  for (int i = 0; i < 14; i++) {
	for (int j = 0; j < 6; j++) {
	  if (i == 13) {
		setBufferText(65 + i, 1 + j, " ");
	  } else {
		setBufferText(65 + i, 1 + j, "  ");
	  }
	}
  }
}

void setFanActive(bool active) {
  isFanActive = active;
}

void drawFlag() {
  const int centerY = 19;
  for (int i = 0; i < 6; i++) {
	setBufferText(28 + i, centerY - 3, "█", white);
	setBufferText(28 + i, centerY - 2, "█", white);
  }
  for (int i = 0; i < 10; i++) {
	setBufferText(26 + i, centerY, "█", white);
	setBufferText(26 + i, centerY + 1, "█", white);
	setBufferText(26 + i, centerY + 2, "█", white);
  }
  for (int i = 0; i < 9; i++) {
	setBufferText(30, centerY - 4 + i, "█", darkBrown);
	setBufferText(31, centerY - 4 + i, "█", darkBrown);
  }
}