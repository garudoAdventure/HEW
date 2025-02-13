#define CONIOEX
#include "conioex.h"
#include "mic.h"
#include "console.h"
#include "draw.h"
#include "draw3D.h"
#include "math.h"

float width = 63.0;
float height = 24.0f;

float thetaIncrease = 0.1f;

void clearCube(Vector3* vec) {
  for (int i = 0; i < 4; i++) {
	int next = i + 1 > 3 ? 0 : i + 1;
	drawLine({ (int)vec[i].x, (int)vec[i].y }, { (int)vec[next].x, (int)vec[next].y }, " ");
	drawLine({ (int)vec[i + 4].x, (int)vec[i + 4].y }, { (int)vec[next + 4].x, (int)vec[next + 4].y }, " ");
	drawLine({ (int)vec[i].x, (int)vec[i].y }, { (int)vec[i + 4].x, (int)vec[i + 4].y }, " ");
  }
}

void drawCube(Vector3* vec) {
  for (int i = 0; i < 4; i++) {
	int next = i + 1 > 3 ? 0 : i + 1;
	drawLine({ (int)vec[i].x, (int)vec[i].y }, { (int)vec[next].x, (int)vec[next].y }, "█");
	drawLine({ (int)vec[i + 4].x, (int)vec[i + 4].y }, { (int)vec[next + 4].x, (int)vec[next + 4].y }, "█");
	drawLine({ (int)vec[i].x, (int)vec[i].y }, { (int)vec[i + 4].x, (int)vec[i + 4].y }, "█");
  }
}

void cubeDemo() {
  static float fTheta = 0.01f;
  static Vector3 temp[8] = { 0 };
  float zMin = -1.0f;
  float zDepth = 2.0f;
  float minX = -1.0f;
  float maxX = 1.0f;
  float minY = -1.0f;
  float maxY = 1.0f;
  Vector3 edgeVertex[8] = {
	{minX, minY, zMin}, {maxX, minY, zMin}, {maxX, minY, zMin + zDepth}, {minX, minY, zMin + zDepth},
	{minX, maxY, zMin}, {maxX, maxY, zMin}, {maxX, maxY, zMin + zDepth}, {minX, maxY, zMin + zDepth},
  };

  clearCube(temp);

  memcpy(temp, edgeVertex, sizeof(temp));

  float peak = getMicPeak();
  if (peak > 80.0f) {
	thetaIncrease = 0.8f;
  }
  else {
	thetaIncrease = 0.01f;
  }

  fTheta += thetaIncrease;
  for (int i = 0; i < 8; i++) {
	temp[i] = matrixMultiple(temp[i], rotateYArray(fTheta));
	temp[i].z += 3.0f;
	temp[i] = matrixMultiple(temp[i], projectArray());
	temp[i].x += width / 2;
	temp[i].y += height / 2;
  }
  drawCube(temp);
}