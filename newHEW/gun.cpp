#define CONIOEX
#include "conioex.h"
#include "UI.h"
#include "buffer.h"
#include "gun.h"
#include "gameMath.h"
#include "player.h"
#include "gameField.h"
#include "iceberg.h"
#include "inputKey.h"
#include "mic.h"
#include "game.h"

const int rawGunCenterX = 65 + 7;
const int rawGunCenterY = 15 + 2;

const Vector2 centerCoord[4] = {
  {0, 0}, {-1, 0}, {-1, 1}, {0, 1}
};

int gunCenterX;
int gunCenterY;
Color crosshairCol;

bool isGunActive;
bool inShootingRange;
bool isShooting;
bool isStartShooting;
int gunShootTimer;
Vector2 lockIceCoord;
float micPeak;

int explodeSound;

void gunInit() {
  drawBorder({ 64, 15, 16, 7 });
  
  clearGunScreen();
  gunCenterX = rawGunCenterX;
  gunCenterY = rawGunCenterY;
  crosshairCol = white;
  drawCrosshair();

  isGunActive = false;
  inShootingRange = false;
  isShooting = false;
  isStartShooting = false;
  lockIceCoord = { 0, 0 };
  micPeak = false;

  explodeSound = opensound((char*)"./Sound/explosion.mp3");
}

void gunUpdate() {
  gunScreenUpdate();
  crosshairCol = white;

  if (isShooting) {
	isGunActive = true;
	if (gunFireEffect() || !inShootingRange) {
	  return;
	}
	IceList* iceList = getIceList();
	IceNode* iceNode = iceList->next;
	while (iceNode != NULL) {
	  if (iceNode->pos.x == lockIceCoord.x && iceNode->pos.y == lockIceCoord.y) {
		playsound(explodeSound, 0);
		setIcebergExplode(iceNode);
		break;
	  }
	  iceNode = iceNode->next;
	}
	inShootingRange = false;
	return;
  }

  inShootingRange = false;
  checkShootingRange();

  if (getControlMode() == ControlMode::KEYBOARD) {
	if (getKeydown(KeyType::SPACE)) {
	  isShooting = true;
	}
	return;
  }
  
  if (isStartShooting) {
	float mic = getMicPeak();
	if (gunShootTimer < 0) {
	  isStartShooting = false;
	} else {
	  if (mic < 50.0f) {
		isShooting = true;
		isStartShooting = false;
	  }
	  gunShootTimer--;
	}
  }
  if (!isStartShooting && getMicPeak() - micPeak > 50.0f) {
	isStartShooting = true;
	gunShootTimer = 50;
  }

  micPeak = getMicPeak();
}

void gunRender() {

}

void gunDestroy() {
  closesound(explodeSound);
}

void checkShootingRange() {
  const Player* player = getPlayer();
  float dirX = sinf(player->viewAngle);
  float dirY = -cosf(player->viewAngle);
  float testX;
  float testY;
  float centerX;
  float centerY;
  int mapX;
  int mapY;

  testX = player->pos.x + dirX * 2;
  testY = player->pos.y + dirY * 2;
  mapX = (int)testX;
  mapY = (int)testY;
  if (getMapCoordEle(mapX, mapY) == 'O') {
	centerX = (float)mapX + 0.5f;
	centerY = (float)mapY + 0.5f;
	if ((centerX - testX) * (centerX - testX) + (centerY - testY) * (centerY - testY) <= 0.5f) {
	  crosshairCol = red;
	}
  }

  testX = player->pos.x + dirX;
  testY = player->pos.y + dirY;
  mapX = (int)testX;
  mapY = (int)testY;
  if (getMapCoordEle(mapX, mapY) == 'O') {
	centerX = (float)mapX + 0.5f;
	centerY = (float)mapY + 0.5f;
	if ((centerX - testX) * (centerX - testX) + (centerY - testY) * (centerY - testY) <= 0.5f) {
	  crosshairCol = green;
	  inShootingRange = true;
	  lockIceCoord = { mapX, mapY };
	}
  }
}

void drawCrosshair() {
  setBufferText(gunCenterX, gunCenterY, "⊕");
  setBufferText(gunCenterX, gunCenterY - 1, "|", crosshairCol);
  setBufferText(gunCenterX, gunCenterY + 1, "|", crosshairCol);
  setBufferText(gunCenterX + 2, gunCenterY, "━", crosshairCol);
  setBufferText(gunCenterX - 3, gunCenterY, "━", crosshairCol);
  setBufferText(gunCenterX + 2, gunCenterY - 1, "╗", crosshairCol);
  setBufferText(gunCenterX + 2, gunCenterY + 1, "╝", crosshairCol);
  setBufferText(gunCenterX - 2, gunCenterY - 1, "╔", crosshairCol);
  setBufferText(gunCenterX - 2, gunCenterY + 1, "╚", crosshairCol);
}

void gunScreenUpdate() {
  static int frame = 0;
  static int centerKey = 0;
  frame++;
  if (frame > 150) {
	frame = 0;
	centerKey = (centerKey + 1) % 4;
  }
  gunCenterX = rawGunCenterX + centerCoord[centerKey].x;
  gunCenterY = rawGunCenterY + centerCoord[centerKey].y;

  clearGunScreen();
  drawCrosshair();
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
	for (int j = 0; j < 5; j++) {
	  setBufferText(65 + i, 16 + j, " ");
	}
  }
}

void setGunActive(bool active) {
  isGunActive = active;
}

bool getGunActive() {
  return isGunActive;
}

void drawCannon() {
  const int startY = 17 + getBoatWave();
  for (int i = 0; i < 7; i++) {
	setFieldBufferText(30, startY + i, "█", gray80);
	setFieldBufferText(31, startY + i, "█", gray92);
	setFieldBufferText(32, startY + i, "█", gray80);
	if (i > 0) {
	  setFieldBufferText(29, startY + i, "█", gray69);
	  setFieldBufferText(33, startY + i, "█", gray69);
	}
	if (i > 3) {
	  setFieldBufferText(28, startY + i, "█", gray54);
	  setFieldBufferText(34, startY + i, "█", gray54);
	}
	if (i > 4) {
	  setFieldBufferText(27, startY + i, "█", darkBrown);
	  setFieldBufferText(35, startY + i, "█", darkBrown);
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