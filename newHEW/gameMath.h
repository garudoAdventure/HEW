#pragma once

#define FIELD_W	  80
#define FIELD_H	  25
#define FIELD_X	  0
#define FIELD_Y	  0

#define PI	3.1415926

//	  0123456789 
//	0 ╔════════╗
//	1 ║        ║
//	2 ║        ║
//	3 ║        ║
//	4 ║        ║
//	5 ║        ║
//	6 ╚════════╝

struct Vector2 {
  int x;
  int y;
};

struct Vector2f {
  float x;
  float y;
};

struct Vector3 {
  float x;
  float y;
  float z;
};

struct Rect4 {
  int x;
  int y;
  int w;
  int h;
};

struct Matrix4x4 {
  float m[4][4] = { 0 };
};

float dotProduct(Vector3 vec1, Vector3 vec2);
Vector3 crossProduct(Vector3 vec1, Vector3 vec2);
float vectorLength(Vector3 vec);
Vector3 vectorUnitlize(Vector3 vec);

Vector3 matrixMultiple(Vector3 vec, Matrix4x4 matrix);
Matrix4x4 rotateXArray(float theta);
Matrix4x4 rotateYArray(float theta);
Matrix4x4 rotateZArray(float theta);
Matrix4x4 projectArray();