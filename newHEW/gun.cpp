#define CONIOEX
#include "conioex.h"
#include "UI.h"
#include "buffer.h"
#include "gun.h"
#include "gameMath.h"
#include "player.h"
#include "gameField.h"
#include "iceberg.h"

const int rawGunCenterX = 65 + 7;
const int rawGunCenterY = 17 + 2;

bool isGunActive = false;

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
  drawBorder({ 64, 16, 16, 8 });
  clearGunScreen();
  gunCenterX = rawGunCenterX;
  gunCenterY = rawGunCenterY;
  drawCrosshair();
}

void gunUpdate() {
  static int frame = 0;
  static int centerKey = 0;
  frame++;

  if (isShooting) {
	drawCannon();
	if (!gunFireEffect()) {
	  const Player* player = getPlayer();
	  Vector2f bulletPos = { player->pos.x, player->pos.y };
	  for (int i = 1; i < 3; i++) {
		int mapX = player->pos.x + player->dir.x * i;
		int mapY = player->pos.y + player->dir.y * i;
		IceList* iceList = getIceList();
		IceNode* iceNode = iceList->next;
		while (iceNode != NULL) {
		  if (iceNode->pos.x == mapX && iceNode->pos.y == mapY) {
			setIcebergExplode(iceNode);
			break;
		  }
		  iceNode = iceNode->next;
		}
	  }
	}
	return;
  }

  if (frame > 150) {
	frame = 0;
	centerKey = (centerKey + 1) % 4;
  }
  gunCenterX = rawGunCenterX + centerCoord[centerKey].x;
  gunCenterY = rawGunCenterY + centerCoord[centerKey].y;

  clearGunScreen();
  drawCrosshair();

  if (isGunActive) {
	drawBracketBorder({ 65, 17, 14, 6 }, yellow);
	drawCannon();

	if (getKeydown(KeyType::SPACE)) {
	  isShooting = true;

	  /* Test */
	  const Player* player = getPlayer();
	  Vector2f bulletPos = { player->pos.x, player->pos.y };
	  for (int i = 1; i < 3; i++) {
		int mapX = player->pos.x + player->dir.x * i;
		int mapY = player->pos.y + player->dir.y * i;
		IceList* iceList = getIceList();
		IceNode* iceNode = iceList->next;
		while (iceNode != NULL) {
		  if (iceNode->pos.x == mapX && iceNode->pos.y == mapY) {
			setIcebergExplode(iceNode);
			break;
		  }
		  iceNode = iceNode->next;
		}
	  }
	}
  }
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

bool gunFireEffect() {
  static int frame = 0;
  const int frameTime = 10;
  // Draw Fire
  if (frame < 1 * frameTime) {
	drawFire5x3();
  }
  else if (frame < 2 * frameTime) {
	drawFire7x4();
  }
  else if (frame < 3 * frameTime) {
	drawFire5x4();
  }
  frame++;
  if (frame == 3 * frameTime) {
	frame = 0;
	isShooting = false;
	return false;
  }
  return true;
}

void clearGunScreen() {
  for (int i = 0; i < 14; i++) {
	for (int j = 0; j < 6; j++) {
	  setBufferText(65 + i, 17 + j, " ");
	}
  }
}

void setGunActive(bool active) {
  isGunActive = active;
}

void drawCannon() {
  const int startY = 18;
  for (int i = 0; i < 6; i++) {
	setBufferText(30, startY + i, "█", gray80);
	setBufferText(31, startY + i, "█", gray92);
	setBufferText(32, startY + i, "█", gray80);
	if (i > 0) {
	  setBufferText(29, startY + i, "█", gray69);
	  setBufferText(33, startY + i, "█", gray69);
	}
	if (i > 3) {
	  setBufferText(28, startY + i, "█", gray54);
	  setBufferText(34, startY + i, "█", gray54);
	}
	if (i > 4) {
	  setBufferText(27, startY + i, "█", darkBrown);
	  setBufferText(35, startY + i, "█", darkBrown);
	}
  }
}

void drawFire5x3() {
  const int startX = 29;
  const int startY = 16;

  setBufferText(startX + 1, startY, "▄", lightRed);
  setBufferText(startX + 2, startY, "▄", lightRed);
  setBufferText(startX + 3, startY, "▄", lightRed);

  setBufferText(startX, startY + 1, "█", lightRed);
  setBufferText(startX + 1, startY + 1, "█", lightOrange);
  setBufferText(startX + 2, startY + 1, "█", lightYellow);
  setBufferText(startX + 3, startY + 1, "█", lightOrange);
  setBufferText(startX + 4, startY + 1, "█", lightRed);

  setBufferText(startX, startY + 2, "▀", lightRed);
  setBufferText(startX + 4, startY + 2, "▀", lightRed);
}

void drawFire7x4() {
  const int startX = 28;
  const int startY = 15;

  for (int i = 0; i < 5; i++) {
	setBufferText(startX + 1 + i, startY, "▄", lightRed);
  }
  for (int i = 0; i < 2; i++) {
	setBufferText(startX, startY + 1 + i, "█", lightRed);
	setBufferText(startX + 1, startY + 1 + i, "█", lightOrange);
	setBufferText(startX + 2, startY + 1 + i, "█", lightYellow);
	setBufferText(startX + 3, startY + 1 + i, "█", whiteOrange);
	setBufferText(startX + 4, startY + 1 + i, "█", lightYellow);
	setBufferText(startX + 5, startY + 1 + i, "█", lightOrange);
	setBufferText(startX + 6, startY + 1 + i, "█", lightRed);
  }
  setBufferText(startX + 1, startY + 3, "▀", lightRed);
  setBufferText(startX + 5, startY + 3, "▀", lightRed);
}

void drawFire5x4() {
  const int startX = 29;
  const int startY = 14;

  setBufferText(startX + 1, startY, "▄", lightRed);
  setBufferText(startX + 2, startY, "█", lightRed);
  setBufferText(startX + 3, startY, "▄", lightRed);
  for (int i = 0; i < 3; i++) {
	setBufferText(startX, startY + 1 + i, "█", lightRed);
	setBufferText(startX + 1, startY + 1 + i, "█", lightOrange);
	if (i == 1) {
	  setBufferText(startX + 2, startY + 1 + i, "█", whiteOrange);
	}
	else {
	  setBufferText(startX + 2, startY + 1 + i, "█", lightYellow);
	}
	setBufferText(startX + 3, startY + 1 + i, "█", lightOrange);
	setBufferText(startX + 4, startY + 1 + i, "█", lightRed);
  }
}