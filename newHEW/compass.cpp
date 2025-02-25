#define CONIOEX
#include "conioex.h"
#include "UI.h"
#include "buffer.h"
#include "compass.h"
#include "gameMath.h"
#include "player.h"
#include "mic.h"

const int compassCenterX = 65 + 6;
const int compassCenterY = 9 + 2;

const Vector2 NESWCoord[16] = {
  {0, -2}, // N
  {2, -2}, // NNE
  {3, -2}, // NE
  {4, -1}, // ENE
  {4, 0},  // E
  {4, 1},  // ESE
  {3, 2},  // SE
  {2, 2},  // SSE
  {0, 2},  // S
  {-2, 2}, // SSW
  {-3, 2}, // SW
  {-4, 1}, // WSW
  {-4, 0}, // W
  {-4, -1},// WNW
  {-3, -2},// NW
  {-2, -2} // NNW
};

bool isCompassActive = false;

void compassInit() {
  drawBorder({ 64, 8, 16, 7 });
}

void compassUpdate() {
  clearCompassScene();
  drawCompass();
  drawDirection();
  
  if (isCompassActive) {
	drawBracketBorder({ 65, 9, 14, 5 }, yellow);
	drawRudder();
	
	if (getMicPeak() > 50.0f) {
	  if (inport(PK_LEFT)) {
		const float angle = -0.1 * PI / 180.0f;
		setPlayerRotate(angle);
	  }
	  if (inport(PK_RIGHT)) {
		const float angle = 0.1 * PI / 180.0f;
		setPlayerRotate(angle);
	  }
	}
  }
}

void compassRender() {

}

void compassDestroy() {

}

void drawCompass() {
  setBufferText(compassCenterX, compassCenterY, "◎");
  setBufferText(compassCenterX, compassCenterY - 1, "↑", red);
  setBufferText(compassCenterX - 2, compassCenterY, "←");
  setBufferText(compassCenterX, compassCenterY + 1, "↓", blue);
  setBufferText(compassCenterX + 2, compassCenterY, "→");
  setBufferText(compassCenterX - 1, compassCenterY - 1, "↖");
  setBufferText(compassCenterX + 2, compassCenterY - 1, "↗");
  setBufferText(compassCenterX + 2, compassCenterY + 1, "↘");
  setBufferText(compassCenterX - 1, compassCenterY + 1, "↙");
}

void drawDirection() {
  const Player* player = getPlayer();
  float angle = player->viewAngle * 180.0f / PI;
  while (angle > 360.0f) {
	angle -= 360.f;
  }
  while (angle < 0.0f) {
	angle += 360.f;
  }
  int northKey = 0;
  for (int i = 0; i < 16; i++) {
	if (angle <= 11.25f + 22.5f * i) {
	  northKey = i;
	  break;
	}
  }
  const char* NESWChar[] = { "Ⓝ", "Ⓔ", "ⓢ", "Ⓦ" };
  for (int i = 0; i < 4; i++) {
	int key = (northKey + 4 * i) % 16;
	setBufferText(compassCenterX + NESWCoord[key].x, compassCenterY + NESWCoord[key].y, NESWChar[i]);
  }
}

void clearCompassScene() {
  for (int i = 0; i < 14; i++) {
	for (int j = 0; j < 5; j++) {
	  if (i == 13) {
		setBufferText(65 + i, 9 + j, " ");
	  } else {
		setBufferText(65 + i, 9 + j, "  ");
	  }
	}
  }
}

void setCompassActive(bool active) {
  isCompassActive = active;
}

void drawRudder() {
  const int centerY = 19 + getBoatWave();
  setBufferText(31, centerY - 3, "█", gold);

  setBufferText(26, centerY - 2, "█", gold);
  setBufferText(27, centerY - 2, "█", gold);
  setBufferText(30, centerY - 2, "█", yellowBrown);
  setBufferText(31, centerY - 2, "█", yellowBrown);
  setBufferText(32, centerY - 2, "█", yellowBrown);
  setBufferText(35, centerY - 2, "█", gold);
  setBufferText(36, centerY - 2, "█", gold);

  setBufferText(28, centerY - 1, "█", yellowBrown);
  setBufferText(29, centerY - 1, "█", yellowBrown);
  setBufferText(30, centerY - 1, "█", gold);
  setBufferText(31, centerY - 1, "▀", gold);
  setBufferText(32, centerY - 1, "█", gold);
  setBufferText(33, centerY - 1, "█", yellowBrown);
  setBufferText(34, centerY - 1, "█", yellowBrown);

  setBufferText(26, centerY, "█", gold);
  setBufferText(27, centerY, "█", gold);
  setBufferText(28, centerY, "█", yellowBrown);
  setBufferText(29, centerY, "█", gold);
  setBufferText(31, centerY, "█", yellowBrown);
  setBufferText(33, centerY, "█", gold);
  setBufferText(34, centerY, "█", yellowBrown);
  setBufferText(35, centerY, "█", gold);
  setBufferText(36, centerY, "█", gold);

  setBufferText(28, centerY + 1, "█", yellowBrown);
  setBufferText(29, centerY + 1, "█", yellowBrown);
  setBufferText(30, centerY + 1, "█", gold);
  setBufferText(31, centerY + 1, "▄", gold);
  setBufferText(32, centerY + 1, "█", gold);
  setBufferText(33, centerY + 1, "█", yellowBrown);
  setBufferText(34, centerY + 1, "█", yellowBrown);

  setBufferText(26, centerY + 2, "█", gold);
  setBufferText(27, centerY + 2, "█", gold);
  setBufferText(30, centerY + 2, "█", yellowBrown);
  setBufferText(31, centerY + 2, "█", yellowBrown);
  setBufferText(32, centerY + 2, "█", yellowBrown);
  setBufferText(35, centerY + 2, "█", gold);
  setBufferText(36, centerY + 2, "█", gold);

  setBufferText(31, centerY + 3, "█", gold);
}