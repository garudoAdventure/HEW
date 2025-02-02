#define CONIOEX
#include "conioex.h"
#include "audio.h"
#include "console.h"
#include "draw.h"
#include "draw3D.h"
#include "math.h"

float width = 80.0f;
float height = 25.0f;

float thetaIncrease = 0.1f;

Vector3 matrixMultiple(Vector3 vec, Matrix4x4 matrix) {
  Vector3 returnVec;
  returnVec.x = vec.x * matrix.m[0][0] + vec.y * matrix.m[1][0] + vec.z * matrix.m[2][0] + matrix.m[3][0];
  returnVec.y = vec.x * matrix.m[0][1] + vec.y * matrix.m[1][1] + vec.z * matrix.m[2][1] + matrix.m[3][1];
  returnVec.z = vec.x * matrix.m[0][2] + vec.y * matrix.m[1][2] + vec.z * matrix.m[2][2] + matrix.m[3][2];
  float w = vec.x * matrix.m[0][3] + vec.y * matrix.m[1][3] + vec.z * matrix.m[2][3] + matrix.m[3][3];
  if (w != 0.0f) {
	returnVec.x /= w;
	returnVec.y /= w;
	returnVec.z /= w;
  }
  return returnVec;
}

Matrix4x4 rotateXArray(float theta) {
  Matrix4x4 rxArray;
  rxArray.m[0][0] = 1.0f;
  rxArray.m[1][1] = cosf(theta * 0.5f);
  rxArray.m[1][2] = sinf(theta * 0.5f);
  rxArray.m[2][1] = -sinf(theta * 0.5f);
  rxArray.m[2][2] = cosf(theta * 0.5f);
  rxArray.m[3][3] = 1.0f;

  return rxArray;
}

Matrix4x4 rotateYArray(float theta) {
  Matrix4x4 ryArray;
  ryArray.m[0][0] = cosf(theta * 0.5f);
  ryArray.m[0][2] = sinf(theta * 0.5f);
  ryArray.m[1][1] = 1.0f;
  ryArray.m[2][0] = -sinf(theta * 0.5f);
  ryArray.m[2][2] = cosf(theta * 0.5f);
  ryArray.m[3][3] = 1.0f;

  return ryArray;
}

Matrix4x4 rotateZArray(float theta) {
  Matrix4x4 rzArray;
  rzArray.m[0][0] = cosf(theta);
  rzArray.m[0][1] = sinf(theta);
  rzArray.m[1][0] = -sinf(theta);
  rzArray.m[1][1] = cosf(theta);
  rzArray.m[2][2] = 1.0f;
  rzArray.m[3][3] = 1.0f;

  return rzArray;
}

Matrix4x4 projectArray() {
  static float fovy = 90.0f;
  static float fovyIncrease = 0.1f;

  Matrix4x4 pArray;
  float zNear = 0.1f;
  float zFar = 1000.0f;
  float aspect = height / width;

  const float f = 1.0f / tanf(fovy * 0.5 * PI / 180);
  const float nF = 1.0f / (zFar - zNear);

  pArray.m[0][0] = f * aspect;
  pArray.m[1][1] = f;
  pArray.m[2][2] = (zNear + zFar) * nF;
  pArray.m[3][2] = -2.0f * zNear * zFar * nF;
  pArray.m[2][3] = 1.0f;
  pArray.m[3][3] = 0.0f;

  return pArray;
}

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
	drawLine({ (int)vec[i].x, (int)vec[i].y }, { (int)vec[next].x, (int)vec[next].y }, "▄");
	drawLine({ (int)vec[i + 4].x, (int)vec[i + 4].y }, { (int)vec[next + 4].x, (int)vec[next + 4].y }, "▀");
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

  float peak = getPeak();
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
	temp[i].x += 1.0f;
	temp[i].y += 1.0f;
	temp[i].x *= width;
	temp[i].y *= 0.5 * height;

	temp[i].x -= 0.5 * width;
  }
  drawCube(temp);
}