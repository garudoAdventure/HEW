#define CONIOEX
#include "conioex.h"
#include "scene.h"
#include "gameTitle.h"
#include "buffer.h"
#include "coin.h"

float lightCenterX = 24.0f;
bool changeScene = false;
int sound;

void titleInit() {
  sound = opensound((char*)"./Sound/begin.mid");
  playsound(sound, 0);
}

void titleUpdate() {
  if (!changeScene && inport(PK_ENTER)) {
	changeScene = true;
  }
  if (changeScene) {
	if (!titleSceneFadeOut()) {
	  changeScene = false;
	  setScene(Scene::GAME);
	}
	return;
  }
  drawTitleBackground();
  showStartHint();
  drawTitle();
  drawSmallBoat();
  for (int i = 0; i < 4; i++) {
	drawTitleCoin(10 + 18 * i, 21);
  }
}

void titleRender() {

}

void titleDestroy() {
  closesound(sound);
}

bool titleSceneFadeOut() {
  static int frame = 0;
  for (int x = 0; x < 80; x++) {
	for (int y = 0; y < 25; y++) {
	  Color rawFgColor = getBufferFgColor(x, y);
	  Color rawBgColor = getBufferBgColor(x, y);
	  Color fadeOutFgColor;
	  fadeOutFgColor.r = rawFgColor.r - 50 < 0 ? 0 : rawFgColor.r - 50;
	  fadeOutFgColor.g = rawFgColor.g - 50 < 0 ? 0 : rawFgColor.g - 50;
	  fadeOutFgColor.b = rawFgColor.b - 50 < 0 ? 0 : rawFgColor.b - 50;
	  Color fadeOutBgColor;
	  fadeOutBgColor.r = rawBgColor.r - 50 < 0 ? 0 : rawBgColor.r - 50;
	  fadeOutBgColor.g = rawBgColor.g - 50 < 0 ? 0 : rawBgColor.g - 50;
	  fadeOutBgColor.b = rawBgColor.b - 50 < 0 ? 0 : rawBgColor.b - 50;
	  setBufferFgColor(x, y, fadeOutFgColor);
	  setBufferBgColor(x, y, fadeOutBgColor);
	}
  }
  frame++;
  if (frame > 200) {
	frame = 0;
	return false;
  }
  return true;
}

void drawTitleBackground() {
  for (int x = 0; x < 80; x++) {
	for (int y = 0; y < 25; y++) {
	  setBufferText(x, y, " ");
	  if (y <= 12) {
		setBufferBgColor(x, y, skyBlue);
	  }
	  if (13 <= y && y <= 14) {
		setBufferBgColor(x, y, seaBlueLight);
	  }
	  if (y == 15) {
		setBufferBgColor(x, y, seaBlue58);
	  }
	  if (16 <= y && y <= 18) {
		setBufferBgColor(x, y, seaBlue50);
	  }
	  if (y == 19) {
		setBufferBgColor(x, y, seaBlue41);
	  }
	  if (20 <= y && y <= 23) {
		setBufferBgColor(x, y, seaBlueDark);
	  }
	}
  }
}

void drawTitle() {
  static int frame = 0;
  const int time = 5;
  int move[4] = { 0, 0, 0, 0 };
  if (frame < 10 * time) {
	move[0] = -1;
  } else if (frame < 20 * time) {
	move[1] = -1;
  } else if (frame < 30 * time) {
	move[0] = 1;
	move[2] = -1;
  } else if (frame < 40 * time) {
	move[1] = 1;
	move[3] = -1;
  } else if (frame < 50 * time) {
	move[2] = 1;
  } else if (frame < 60 * time) {
	move[3] = 1;
  }

  const int firstLineStartX = 18;
  const int firstLineStartY = 1;
  drawHo(firstLineStartX, firstLineStartY + move[0], iceBlueLight);
  drawTsu(firstLineStartX + 11, firstLineStartY + move[1], iceBlueLight);
  drawKa(firstLineStartX + 17, firstLineStartY + move[2], iceBlueLight);
  drawI(firstLineStartX + 29, firstLineStartY + move[3], iceBlueLight);


  if (frame > 70 * time) {
	lightCenterX += 0.1f;
  }
  const int secondLineStartX = 26;
  const int secondLineStartY = 7;
  drawTo(secondLineStartX, secondLineStartY);
  drawRe(secondLineStartX + 7, secondLineStartY);
  drawJi(secondLineStartX + 14, secondLineStartY);
  drawYa(secondLineStartX + 24, secondLineStartY);
  drawNobashibo(secondLineStartX + 31, secondLineStartY);

  frame++;
  if (frame == 300 * time) {
	frame = 0;
	lightCenterX = 24.0f;
  }
}

void drawHo(int x, int y, Color color) {
  const char* moji[5][9] = {
	{"█", " ", "▀", "▀", "▀", "█", "▀", "▀", "▀"},
	{"█", " ", "▄", "▄", "▄", "█", "▄", "▄", "▄"},
	{"█", " ", " ", " ", " ", "█", " ", " ", " "},
	{"█", " ", "█", "▀", "▀", "█", "▀", "▀", "▀"},
	{"█", " ", "█", "▄", "▄", "█", " ", " ", " "}
  };
  for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 5; j++) {
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawTsu(int x, int y, Color color) {
  const char* moji[5][4] = {
	{" ", " ", " ", " "},
	{" ", " ", " ", " "},
	{" ", " ", " ", " "},
	{"▀", "▀", "▀", "█"},
	{"▄", "▄", "▄", "█"}
  };
  for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 5; j++) {
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawKa(int x, int y, Color color) {
  const char* moji[5][10] = {
	{" ", " ", " ", "█", " ", " ", " ", " ", "█", " "},
	{"▄", "▄", "▄", "█", "▄", "▄", "▄", "▄", " ", "█"},
	{" ", " ", " ", "█", " ", " ", " ", "█", " ", " "},
	{" ", " ", "█", " ", " ", " ", " ", "█", " ", " "},
	{" ", "█", " ", " ", " ", " ", "▄", "█", " ", " "}
  };
  for (int i = 0; i < 10; i++) {
	for (int j = 0; j < 5; j++) {
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawI(int x, int y, Color color) {
  const char* moji[5][9] = {
	{"█", " ", " ", " ", " ", " ", "█", " ", " "},
	{"█", " ", " ", " ", " ", " ", " ", "█", " "},
	{"█", " ", " ", " ", " ", " ", " ", " ", "█"},
	{" ", "█", " ", " ", "█", " ", " ", " ", "█"},
	{" ", " ", "█", "█", " ", " ", " ", " ", " "}
  };
  for (int i = 0; i < 9; i++) {
	for (int j = 0; j < 5; j++) {
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawTo(int x, int y) {
  const char* moji[5][5] = {
	{"█", " ", " ", " ", " "},
	{"█", " ", " ", " ", " "},
	{"█", "█", "▄", " ", " "},
	{"█", " ", "▀", "█", "▄"},
	{"█", " ", " ", " ", " "}
  };
  for (int i = 0; i < 5; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = gold;
	  int lightX = (int)lightCenterX;
	  if (lightX - 1 <= x + i && x + i <= lightX + 1) {
		color = lightGold;
	  }
	  if (lightX == x + i) {
		color = white;
	  }
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawRe(int x, int y) {
  const char* moji[5][5] = {
	{"█", " ", " ", " ", " "},
	{"█", " ", " ", " ", " "},
	{"█", " ", " ", " ", "█"},
	{"█", " ", "█", "█", " "},
	{"█", "█", " ", " ", " "}
  };
  for (int i = 0; i < 5; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = gold;
	  int lightX = (int)lightCenterX;
	  if (lightX - 1 <= x + i && x + i <= lightX + 1) {
		color = lightGold;
	  }
	  if (lightX == x + i) {
		color = white;
	  }
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawJi(int x, int y) {
  const char* moji[5][10] = {
	{" ", " ", " ", " ", " ", " ", "▄", " ", "▄", " "},
	{" ", "▄", " ", " ", " ", " ", "▄", "▀", " ", "▀"},
	{"▄", " ", "▀", " ", " ", " ", "█", " ", " ", " "},
	{" ", "▀", " ", " ", " ", "█", " ", " ", " ", " "},
	{"▄", "▄", "▄", "▄", "█", " ", " ", " ", " ", " "}
  };
  for (int i = 0; i < 10; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = gold;
	  int lightX = (int)lightCenterX;
	  if (lightX - 1 <= x + i && x + i <= lightX + 1) {
		color = lightGold;
	  }
	  if (lightX == x + i) {
		color = white;
	  }
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawYa(int x, int y) {
  const char* moji[5][5] = {
	{" ", " ", " ", " ", " "},
	{" ", " ", " ", " ", " "},
	{" ", " ", "▄", " ", " "},
	{" ", "▄", "█", "▀", "█"},
	{"▀", " ", "█", " ", " "}
  };
  for (int i = 0; i < 5; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = gold;
	  int lightX = (int)lightCenterX;
	  if (lightX - 1 <= x + i && x + i <= lightX + 1) {
		color = lightGold;
	  }
	  if (lightX == x + i) {
		color = white;
	  }
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawNobashibo(int x, int y) {
  const char* moji[5][7] = {
	{" ", " ", " ", " ", " ", " ", " "},
	{" ", " ", " ", " ", " ", " ", " "},
	{"▄", "▄", "▄", "▄", "▄", "▄", "▄"},
	{" ", " ", " ", " ", " ", " ", " "},
	{" ", " ", " ", " ", " ", " ", " "}
  };
  for (int i = 0; i < 7; i++) {
	for (int j = 0; j < 5; j++) {
	  Color color = gold;
	  int lightX = (int)lightCenterX;
	  if (lightX - 1 <= x + i && x + i <= lightX + 1) {
		color = lightGold;
	  }
	  if (lightX == x + i) {
		color = white;
	  }
	  setBuffer(x + i, y + j, moji[j][i], color, skyBlue);
	}
  }
}

void drawSmallBoat() {
  static int frame = 0;
  static int startX = 5;
  int startY = 18;
  if (frame < 200) {
	frame++;
  } else {
	if (startX < 33) {
	  startX++;
	}
	frame = 0;
  }
  setBufferText(startX + 4, startY - 3, "▄", darkBrown);
  setBufferText(startX + 2, startY - 2, "█", white);
  setBufferText(startX + 3, startY - 2, "█", white);
  setBufferText(startX + 4, startY - 2, "█", darkBrown);
  for (int i = 0; i < 8; i++) {
	setBufferText(startX + 1 + i, startY - 1, "▄", brown);
	if (i == 3) {
	  setBufferText(startX + 1 + i, startY - 1, "█", darkBrown);
	}
  }
  for (int i = 0; i < 10; i++) {
	setBufferText(startX + i, startY, "█", darkBrown);
  }
  for (int i = 0; i < 8; i++) {
	setBufferText(startX + 1 + i, startY + 1, "█", darkBrown);
	if (i < 1 || i > 5) {
	  setBufferText(startX + 1 + i, startY + 1, "▀", darkBrown);
	}
  }
}

void drawTitleCoin(int x, int y) {
  static int frame = 0;
  const int turnSpeed = 250;
  const int centerX = x;
  const int centerY = y;
  const int width = 5;
  const int height = 4;
  const char* coinFront[height][width] = {
	{" ", "Y", "Y", "Y", " "},
	{"Y", "Y", "M", "Y", "Y"},
	{"Y", "Y", "M", "Y", "Y"},
	{" ", "Y", "Y", "Y", " "},
  };
  const char* coinSide[height][width] = {
	{" ", " ", "Y", " ", " "},
	{" ", " ", "Y", " ", " "},
	{" ", " ", "Y", " ", " "},
	{" ", " ", "Y", " ", " "},
  };
  char* coinTemp[height][width];
  if (frame < turnSpeed) {
	memcpy(coinTemp, coinFront, sizeof(coinTemp));
  }
  else {
	memcpy(coinTemp, coinSide, sizeof(coinTemp));
  }
  for (int i = 0; i < width; i++) {
	for (int j = 0; j < height; j++) {
	  Color color = white;
	  if (coinTemp[j][i] != " ") {
		if (coinTemp[j][i] == "Y") {
		  color = { 239, 191, 4 };
		}
		if (coinTemp[j][i] == "M") {
		  color = { 244, 221, 131 };
		}
		setBufferText(centerX - width / 2 + i, centerY - height / 2 + j, "█", color);
	  }
	}
  }
  frame++;
  if (frame == turnSpeed * 2) {
	frame = 0;
  }
}

void showStartHint() {
  static int frame = 0;
  const int maxFrame = 300;
  const char* hint[9] = {
	"ほ", "っ", "と", "し", "て", "ス", "タ", "ー", "ト"
  };
  if (frame < maxFrame) {
	for (int i = 0; i < 9; i++) {
	  setBufferText(31 + i * 2, 14, hint[i]);
	}
  } else {
	for (int i = 0; i < 9; i++) {
	  setBufferText(31 + i * 2, 14, "  ");
	}
  }
  if (frame == maxFrame + 50) {
	frame = 0;
  }
  
  frame++;
}