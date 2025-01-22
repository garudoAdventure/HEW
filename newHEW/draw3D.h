#pragma once
#include "mathStruct.h"

#define PI	3.1415926

struct Matrix4x4 {
  float m[4][4] = { 0 };
};

Vector3 matrixMultiple(Vector3 vec, Matrix4x4 matrix);
Matrix4x4 rotateXArray(float theta);
Matrix4x4 rotateYArray(float theta);
Matrix4x4 rotateZArray(float theta);
Matrix4x4 projectArray();
void drawCube(Vector3* vec, const char* shape);
void cubeDemo();