#define CONIOEX
#include "conioex.h"
#include "UI.h"
#include "buffer.h"
#include "compass.h"
#include "mathStruct.h"
#include "player.h"

Vector2 NESWCoord[16] = {
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

void compassInit() {
  renderBorder(64, 8, 16, 8);
  drawCompass();
}

void compassUpdate() {
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
	if (angle <= 11.25f + 22.5f * i ) {
	  northKey = i;
	  break;
	}
  }
  const int centerX = 6;
  const int centerY = 2;
  const char* NESWChar[] = { "Ⓝ", "Ⓔ", "ⓢ", "Ⓦ" };
  int preKeyOpr = -1;
  if (inport(PK_RIGHT)) {
	preKeyOpr = -1;
  }
  if (inport(PK_LEFT)) {
	preKeyOpr = 1;
  }
  for (int i = 0; i < 4; i++) {
	int key = (northKey + 4 * i) % 16;
	int preKey = key + preKeyOpr;
	if (preKey < 0) preKey = 15;
	if (preKey > 15) preKey = 0;
	setBufferText(65 + centerX + NESWCoord[preKey].x, 9 + centerY + NESWCoord[preKey].y, "  ");
	setBufferText(65 + centerX + NESWCoord[key].x, 9 + centerY + NESWCoord[key].y, NESWChar[i]);
  }
}

void compassRender() {

}

void compassDestroy() {

}

void drawCompass() {
  const int centerX = 6;
  const int centerY = 2;
  setBufferText(65 + centerX, 9 + centerY, "◎");
  setBufferText(65 + centerX, 9 + centerY - 1, "↑", { 255, 0, 0 });
  setBufferText(65 + centerX - 2, 9 + centerY, "←");
  setBufferText(65 + centerX, 9 + centerY + 1, "↓", {0, 0, 255});
  setBufferText(65 + centerX + 2, 9 + centerY, "→");
  setBufferText(65 + centerX - 1, 9 + centerY - 1, "↖");
  setBufferText(65 + centerX + 2, 9 + centerY - 1, "↗");
  setBufferText(65 + centerX + 2, 9 + centerY + 1, "↘");
  setBufferText(65 + centerX - 1, 9 + centerY + 1, "↙");
}