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
#include "coin.h"
#include "iceberg.h"
#include "UI.h"

bool isPause;
GameItem items[3] = { GameItem::FAN, GameItem::COMPASS, GameItem::GUN };
int nowItemKey = 0;
int gameElapsedTime;
int gameSound;
int startSound;
int endSound;

void gameInit() {
  gameElapsedTime = 0;
  isPause = false;
  fieldInit();
  playerInit();
  fanInit();
  compassInit();
  gunInit();
  icebergInit();
  coinInit();
  gameSound = opensound((char*)"./Sound/galaxy.mp3");
  startSound = opensound((char*)"./Sound/gameStart.mp3");
  endSound = opensound((char*)"./Sound/gameEnd.mp3");
  playsound(startSound, 0);
}

void gameUpdate() {
  static int frameCount = 0;
  static int countdown = 60;
  if (frameCount < 450) {
	frameCount++;
  }
  else {
	frameCount = 0;
	countdown--;
  }

  inputKeyUpdate();
  setGameItemActive();
  
  fieldUpdate();
  playerUpdate();
  if (showGameStart()) {
	return;
  }

  if (countdown < 0) {
	if (!showGameEnd()) {
	  setScene(Scene::RESULT);
	}
	return;
  }

  showLifeBar(countdown, 60);
  
  fanUpdate();
  compassUpdate();
  gunUpdate();

  gameElapsedTime++;
}

void gameRender() {
  bufferRender();
}

void gameDestroy() {
  playerDestroy();
  closesound(gameSound);
}

void setGameItemActive() {
  if (getKeydown(KeyType::UP)) {
	nowItemKey = nowItemKey - 1 < 0 ? 0 : nowItemKey - 1;
  }
  if (getKeydown(KeyType::DOWN)) {
	nowItemKey = nowItemKey + 1 > 2 ? 2 : nowItemKey + 1;
  }
  switch (items[nowItemKey]) {
	case GameItem::FAN:
	  setFanActive(true);
	  setCompassActive(false);
	  setGunActive(false);
	  break;
	case GameItem::COMPASS:
	  setFanActive(false);
	  setCompassActive(true);
	  setGunActive(false);
	  break;
	case GameItem::GUN:
	  setFanActive(false);
	  setCompassActive(false);
	  setGunActive(true);
	  break;
	}
}

int getGameElapsedTime() {
  return gameElapsedTime;
}

bool showGameStart() {
  static int frame = 0;
  static int shift = 0;
  const int maxShift = 110;
  if (shift == maxShift) {
	return false;
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
  return shift < maxShift;
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
	return false;
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
  return shift < maxShift;
}