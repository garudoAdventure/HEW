#define CONIOEX
#include "conioex.h"
#include "fan.h"
#include "UI.h"
#include "buffer.h"
#include "gameMath.h"
#include "player.h"
#include "mic.h"
#include "game.h"

const int fanCenterX = 65 + 6;
const int fanCenterY = 1 + 2;

const float maxV = 0.008f;
const float acceleration = 0.0001f;

const Vector2 fanBladeRotate[3][4] = {
  { {0, -2}, {2, -2}, {4, -1}, {4, 0} },
  { {4, 1}, {2, 2}, {0, 2}, {-2, 2} },
  { {-4, 1}, {-4, 0}, {-4, -1}, {-2, -2} }
};

bool isFanActive;
int slidingSound;

void fanInit() {
  drawBorder({ 64, 0, 16, 8 });

  isFanActive = false;

  slidingSound = opensound((char*)"./Sound/sliding.mp3");
  setvolume(slidingSound, 0);
  playsound(slidingSound, 1);
}

void fanUpdate() {
  clearFanScene();
  drawFan();
  showCurrentVelocity(getPlayerVelocity());

  bool triggerFan = false;
  switch (getControlMode()) {
	case ControlMode::MIC:
	  triggerFan = getMicPeak() > 95.0f;
	  break;
	case ControlMode::KEYBOARD:
	  triggerFan = inport(PK_UP);
	  break;
  }
  if (triggerFan) {
	const float velocity = getPlayerVelocity();
	float newV = velocity + acceleration > maxV ? maxV : velocity + acceleration;
	setPlayerVelocity(newV);
  }
}

void fanRender() {

}

void fanDestroy() {
  closesound(slidingSound);
}

void drawFan() {
  static int frame = 0;
  static int key = 0;
  int v = round(getPlayerVelocity() * 400000);
  if (frame < 202 - v) {
	frame++;
  } else {
	key = (key + 1) % 4;
	frame = 0;
  }
  setBufferText(fanCenterX, fanCenterY, "◎");
  Color color = isHitwall() ? red : blue;
  for (int i = 0; i < 3; i++) {
	setBufferText(fanCenterX + fanBladeRotate[i][key].x, fanCenterY + fanBladeRotate[i][key].y, "■", color);
  }
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

bool getFanActive() {
  return isFanActive;
}

void stopSlidingSound() {
  stopsound(slidingSound);
}

void drawFlag() {
  static int frame = 0;
  int centerY = 19 + getBoatWave();
  for (int i = 0; i < 6; i++) {
	setFieldBufferText(28 + i, centerY - 3, "█", gray169);
	setFieldBufferText(28 + i, centerY - 2, "█", gray169);
  }
  for (int i = 0; i < 10; i++) {
	setFieldBufferText(26 + i, centerY, "█", gray169);
	setFieldBufferText(26 + i, centerY + 1, "█", gray169);
	setFieldBufferText(26 + i, centerY + 2, "█", gray169);
  }
  for (int i = 0; i < 9; i++) {
	setFieldBufferText(30, centerY - 4 + i, "█", darkBrown);
	setFieldBufferText(31, centerY - 4 + i, "█", darkBrown);
  }
}

void showCurrentVelocity(float velocity) {
  const char* digits[10] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
  const char* digit = "0";
  const char* tenDigit = "0";
  const char* hunDigit = "0";
  
  int num = round(velocity * 20000);
  hunDigit = digits[num / 100];
  tenDigit = digits[num % 100 / 10];
  digit = digits[num % 10];
  setBufferText(fanCenterX - 3, fanCenterY + 3, hunDigit);
  setBufferText(fanCenterX - 2, fanCenterY + 3, tenDigit);
  setBufferText(fanCenterX - 1, fanCenterY + 3, digit);

  setBufferText(fanCenterX, fanCenterY + 3, "k");
  setBufferText(fanCenterX + 1, fanCenterY + 3, "n");
  setBufferText(fanCenterX + 2, fanCenterY + 3, "o");
  setBufferText(fanCenterX + 3, fanCenterY + 3, "t");
  // setBufferText(fanCenterX + 4, fanCenterY + 3, "t");

  float soundVol = 100.0f / 160.0f * num;
  setvolume(slidingSound, soundVol);
}