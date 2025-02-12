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

float dotProduct(Vector3 vec1, Vector3 vec2);
Vector3 crossProduct(Vector3 vec1, Vector3 vec2);
float vectorLength(Vector3 vec);
Vector3 vectorUnitlize(Vector3 vec);