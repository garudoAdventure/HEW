#include "gameMath.h"
#include "math.h"

float dotProduct(Vector3 vec1, Vector3 vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 crossProduct(Vector3 vec1, Vector3 vec2) {
  float x = vec1.y * vec2.z - vec2.y * vec1.z;
  float y = vec1.z * vec2.x - vec2.z * vec1.x;
  float z = vec1.x * vec2.y - vec2.x * vec1.y;
  return { x, y, z };
}

float vectorLength(Vector3 vec) {
  float len = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
  len = sqrtf(len);
  return len;
}

Vector3 vectorUnitlize(Vector3 vec) {
  float x = vec.x / vectorLength(vec);
  float y = vec.y / vectorLength(vec);
  float z = vec.z / vectorLength(vec);
  return { x, y, z };
}

Vector3 matrixMultiple(Vector3 vec, Matrix4x4 matrix) {
  Vector3 returnVec;
  returnVec.x = matrix.m[0][0] * vec.x + matrix.m[0][1] * vec.y + matrix.m[0][2] * vec.z + matrix.m[0][3];
  returnVec.y = matrix.m[1][0] * vec.x + matrix.m[1][1] * vec.y + matrix.m[1][2] * vec.z + matrix.m[1][3];
  returnVec.z = matrix.m[2][0] * vec.x + matrix.m[2][1] * vec.y + matrix.m[2][2] * vec.z + matrix.m[2][3];
  float w = matrix.m[3][0] * vec.x + matrix.m[3][1] * vec.y + matrix.m[3][2] * vec.z + matrix.m[3][3];
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
  rxArray.m[1][1] = cosf(theta);
  rxArray.m[1][2] = -sinf(theta);
  rxArray.m[2][1] = sinf(theta);
  rxArray.m[2][2] = cosf(theta);
  rxArray.m[3][3] = 1.0f;

  return rxArray;
}

Matrix4x4 rotateYArray(float theta) {
  Matrix4x4 ryArray;
  ryArray.m[0][0] = cosf(theta);
  ryArray.m[0][2] = sinf(theta);
  ryArray.m[1][1] = 1.0f;
  ryArray.m[2][0] = -sinf(theta);
  ryArray.m[2][2] = cosf(theta);
  ryArray.m[3][3] = 1.0f;

  return ryArray;
}

Matrix4x4 rotateZArray(float theta) {
  Matrix4x4 rzArray;
  rzArray.m[0][0] = cosf(theta);
  rzArray.m[0][1] = -sinf(theta);
  rzArray.m[1][0] = sinf(theta);
  rzArray.m[1][1] = cosf(theta);
  rzArray.m[2][2] = 1.0f;
  rzArray.m[3][3] = 1.0f;

  return rzArray;
}

Matrix4x4 projectArray() {
  Matrix4x4 pArray;
  const float fov = 45.0f;
  const float zNear = 0.1f;
  const float zFar = 10.0f;
  // const float aspect = width / height;

  const float f = 1.0f / tanf(fov * 0.5f * PI / 180.0f);
  const float nF = 1.0f / (zFar - zNear);

  pArray.m[0][0] = f;
  pArray.m[1][1] = f;
  pArray.m[2][2] = -(zNear + zFar) * nF;
  pArray.m[2][3] = -2.0f * zNear * zFar * nF;
  pArray.m[3][2] = -1.0f;
  pArray.m[3][3] = 0.0f;

  return pArray;
}