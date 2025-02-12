#define CONIOEX
#include "conioex.h"
#include "UI.h"
#include "buffer.h"
#include "gun.h"
#include "mathStruct.h"
#include "player.h"

const int rawGunCenterX = 65 + 7;
const int rawGunCenterY = 17 + 2;

int gunCenterX;
int gunCenterY;

bool isShooting = false;

const Rect4 shootEffectCoord[7] = {
  {-1, -1, 3, 3},
  {-2, -1, 5, 3},
  {-3, -1, 7, 3},
  {-4, -1, 9, 4},
  {-5, -2, 11, 5},
  {-6, -2, 13, 5},
  {-7, -2, 14, 6},
};

const Vector2 centerCoord[4] = {
  {0, 0}, {-1, 0}, {-1, 1}, {0, 1}
};

void gunInit() {
  renderBorder(64, 16, 16, 8);
  clearGunScreen();
  gunCenterX = rawGunCenterX;
  gunCenterY = rawGunCenterY;
  drawCrosshair();
}

void gunUpdate() {
  static int frame = 0;
  static int centerKey = 0;
  frame++;
  if (inport(PK_SP)) {
	isShooting = true;
  }
  if (isShooting) {
	shootEffect();
	return;
  }
  if (frame > 200) {
	frame = 0;
	centerKey = (centerKey + 1) % 4;
  }
  gunCenterX = rawGunCenterX + centerCoord[centerKey].x;
  gunCenterY = rawGunCenterY + centerCoord[centerKey].y;

  clearGunScreen();
  drawCrosshair();
}

void gunRender() {

}

void gunDestroy() {

}

void drawCrosshair() {
  setBufferText(gunCenterX, gunCenterY, "⊕");
  setBufferText(gunCenterX, gunCenterY - 1, "|");
  setBufferText(gunCenterX, gunCenterY + 1, "|");
  setBufferText(gunCenterX + 2, gunCenterY, "━");
  setBufferText(gunCenterX - 3, gunCenterY, "━");
  setBufferText(gunCenterX + 2, gunCenterY - 1, "╗");
  setBufferText(gunCenterX + 2, gunCenterY + 1, "╝");
  setBufferText(gunCenterX - 2, gunCenterY - 1, "╔");
  setBufferText(gunCenterX - 2, gunCenterY + 1, "╚");
}

void shootEffect() {
  static int frame = 0;
  if (frame < 5) {
	frame++;
	return;
  }
  frame = 0;
  static int count = 0;
  const int key = count % 7;
  const int startX = gunCenterX + shootEffectCoord[key].x;
  const int startY = gunCenterY + shootEffectCoord[key].y;
  const int endX = startX + shootEffectCoord[key].w;
  const int endY = startY + shootEffectCoord[key].h;
  for (int x = startX; x < endX; x++) {
	for (int y = startY; y < endY; y++) {
	  if (x == gunCenterX && y == gunCenterY) {
		continue;
	  }
	  if (x < 65 || x > 79 || y < 17 || y > 22) {
		continue;
	  }
	  if (count < 7) {
		setBufferText(x, y, "▒", {50, 50, 50});
	  } else {
		setBufferText(x, y, " ");
	  }
	  
	}
  }
  count++;
  if (count == 14) {
	isShooting = false;
	count = 0;
	drawCrosshair();
  }
}

void clearGunScreen() {
  for (int i = 0; i < 14; i++) {
	for (int j = 0; j < 6; j++) {
	  setBufferText(65 + i, 17 + j, " ");
	}
  }
}