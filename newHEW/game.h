#pragma once

#include "mathStruct.h"

enum GameScene {
  FIELD,
  MAP,
};

struct ViewData {
  Vector2 visionData[62] = { 0 };
  float distanceToObj[62] = { 0 };
};

void gameInit();
void gameUpdate();
void gameRender();
void gameDestroy();

void gameSceneInit();
void gameSceneDestroy();
void setGameScene(GameScene scene);