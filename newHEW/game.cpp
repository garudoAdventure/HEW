#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "gameField.h"
#include "scene.h"
#include "player.h"
#include "buffer.h"
#include "fan.h"
#include "compass.h"
#include "gun.h"
#include "inputKey.h"
#include "UI.h"

int gameElapsedTime;
int countdown;

bool gameStartFinish;
bool gameEndFinish;

int gameSound;
int startSound;
int endSound;

void gameInit() {
  fieldInit();
  playerInit();
  fanInit();
  compassInit();
  gunInit();
  gameElapsedTime = 0;
  countdown = 60;
  gameStartFinish = false;
  gameEndFinish = false;
  gameSound = opensound((char*)"./Sound/galaxy.mp3");
  startSound = opensound((char*)"./Sound/gameStart.mp3");
  endSound = opensound((char*)"./Sound/gameEnd.mp3");
  playsound(startSound, 0);
}

void gameUpdate() {
  inputKeyUpdate();
  fieldUpdate();
  showGetCoinNum(getCollectCoinNum());
  
  gameStartFinish = showGameStart();
  if (!gameStartFinish) {
	drawMyBoat();
	drawFlag();
	return;
  }

  if (!gameCountdown()) {
	gameEndFinish = showGameEnd();
	drawMyBoat();
	drawFlag();
	if (gameEndFinish) {
	  setScene(Scene::RESULT);
	}
	return;
  }

  playerUpdate();
  showLifeBar(countdown, 60);
  
  compassUpdate();
  gunUpdate();

  if (getGunActive()) {
	drawCannon();
  } else {
	fanUpdate();
	if (getCompassActive()) {
	  drawRudder();
	} else {  
	  drawFlag();
	}
  }
  setGunActive(false);
  setCompassActive(false);
  setFanActive(false);

  gameElapsedTime++;
}

void gameRender() {
  bufferRender();
}

void gameDestroy() {
  fieldDestroy();
  playerDestroy();
  fanDestroy();
  compassDestroy();
  gunDestroy();
  closesound(gameSound);
  closesound(startSound);
  closesound(endSound);
}

int getGameElapsedTime() {
  return gameElapsedTime;
}

int getCountdown() {
  return countdown;
}

bool gameCountdown() {
  static int frameCount = 0;
  if (countdown < 0) {
	frameCount = 0;
	return false;
  }
  if (frameCount < 150) {
	frameCount++;
  } else {
	frameCount = 0;
	countdown--;
  }
  return true;
}

bool showGameStart() {
  static int frame = 0;
  static int shift = 0;
  const int maxShift = 110;
  if (gameStartFinish) {
	return true;
  }
  if (shift == maxShift) {
	frame = 0;
	shift = 0;
	return true;
  }
  const char* word[6][42] = {
	{ "█", "█", "█", "█", "█", "█", "█", "╗", "█", "█", "█", "█", "█", "█", "█", "█", "╗", " ", "█", "█", "█", "█", "█", "╗", " ", "█", "█", "█", "█", "█", "█", "╗", " ", "█", "█", "█", "█", "█", "█", "█", "█", "╗" },
	{ "█", "█", "╔", "═", "═", "═", "═", "╝", "╚", "═", "═", "█", "█", "╔", "═", "═", "╝", "█", "█", "╔", "═", "═", "█", "█", "╗", "█", "█", "╔", "═", "═", "█", "█", "╗", "╚", "═", "═", "█", "█", "╔", "═", "═", "╝" },
	{ "█", "█", "█", "█", "█", "█", "█", "╗", " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "█", "█", "█", "█", "█", "║", "█", "█", "█", "█", "█", "█", "╔", "╝", " ", " ", " ", "█", "█", "║", " ", " ", " " },
	{ "╚", "═", "═", "═", "═", "█", "█", "║", " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "╔", "═", "═", "█", "█", "║", "█", "█", "╔", "═", "═", "█", "█", "╗", " ", " ", " ", "█", "█", "║", " ", " ", " " },
	{ "█", "█", "█", "█", "█", "█", "█", "║", " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", " ", " ", "█", "█", "║", "█", "█", "║", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", " ", " ", " " },
	{ "╚", "═", "═", "═", "═", "═", "═", "╝", " ", " ", " ", "╚", "═", "╝", " ", " ", " ", "╚", "═", "╝", " ", " ", "╚", "═", "╝", "╚", "═", "╝", " ", " ", "╚", "═", "╝", " ", " ", " ", "╚", "═", "╝", " ", " ", " " }
  };
  if (frame < 2) {
	frame++;
  } else {
	shift++;
	frame = 0;
  }
  if (shift == maxShift) {
	playsound(gameSound, 1);
  }
  for (int i = 0; i < 50; i++) {
	for (int j = 0; j < 8; j++) {
	  setFieldBufferText(-49 + shift + i, 2 + j, "█", gold);
	}
  }
  for (int i = 0; i < 42; i++) {
	for (int j = 0; j < 6; j++) {
	  setFieldBufferText(-45 + shift + i, 3 + j, word[j][i], white, gold);
	}
  }
  return shift > maxShift;
}

bool showGameEnd() {
  static int frame = 0;
  static int shift = 0;
  const int maxShift = 110;
  if (frame == 0 && shift == 0) {
	stopsound(gameSound);
	playsound(endSound, 0);
  }
  if (shift == maxShift) {
	frame = 0;
	shift = 0;
	return true;
  }
  const char* word[6][56] = {
	{ "█", "█", "█", "█", "█", "█", "█", "█", "╗", "█", "█", "╗", "█", "█", "█", "╗", " ", " ", " ", "█", "█", "█", "╗", "█", "█", "█", "█", "█", "█", "█", "╗", " ", " ", " ", "█", "█", "╗", " ", " ", " ", "█", "█", "╗", "█", "█", "█", "█", "█", "█", "╗", " ", " ", " ", "█", "█", "╗" },
	{ "╚", "═", "═", "█", "█", "╔", "═", "═", "╝", "█", "█", "║", "█", "█", "█", "█", "╗", " ", "█", "█", "█", "█", "║", "█", "█", "╔", "═", "═", "═", "═", "╝", " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", "█", "█", "╔", "═", "═", "█", "█", "╗", " ", " ", "█", "█", "║" },
	{ " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", "█", "█", "╔", "█", "█", "█", "█", "╔", "█", "█", "║", "█", "█", "█", "█", "█", "╗", " ", " ", " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", "█", "█", "█", "█", "█", "█", "╔", "╝", " ", " ", "█", "█", "║" },
	{ " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", "█", "█", "║", "╚", "█", "█", "╔", "╝", "█", "█", "║", "█", "█", "╔", "═", "═", "╝", " ", " ", " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", "█", "█", "╔", "═", "═", "═", "╝", " ", " ", " ", "╚", "═", "╝" },
	{ " ", " ", " ", "█", "█", "║", " ", " ", " ", "█", "█", "║", "█", "█", "║", " ", "╚", "═", "╝", " ", "█", "█", "║", "█", "█", "█", "█", "█", "█", "█", "╗", " ", " ", " ", "╚", "█", "█", "█", "█", "█", "█", "╔", "╝", "█", "█", "║", " ", " ", " ", " ", " ", " ", " ", "█", "█", "╗" },
	{ " ", " ", " ", "╚", "═", "╝", " ", " ", " ", "╚", "═", "╝", "╚", "═", "╝", " ", " ", " ", " ", " ", "╚", "═", "╝", "╚", "═", "═", "═", "═", "═", "═", "╝", " ", " ", " ", " ", "╚", "═", "═", "═", "═", "═", "╝", " ", "╚", "═", "╝", " ", " ", " ", " ", " ", " ", " ", "╚", "═", "╝" },
  };
  if (frame < 2) {
	frame++;
  }
  else {
	shift++;
	frame = 0;
  }
  for (int i = 0; i < 60; i++) {
	for (int j = 0; j < 8; j++) {
	  setFieldBufferText(-49 + shift + i, 2 + j, "█", gold);
	}
  }
  for (int i = 0; i < 56; i++) {
	for (int j = 0; j < 6; j++) {
	  setFieldBufferText(-45 + shift + i, 3 + j, word[j][i], white, gold);
	}
  }
  return shift > maxShift;
}