#define CONIOEX
#include "conioex.h"
#include "gameResult.h"
#include "scene.h"
#include "UI.h"
#include "buffer.h"
#include "iostream"
#include "player.h"
#include "iceberg.h"
#include "console.h"
#include "game.h"
#include "mic.h"

int resultSound;
int score;
int maxScore;
int resultCoinNum;
int resultIceNum;
float hotPower;
bool showResultFinish;

const char resultWord[6][135] = {
  "██████╗ ███████╗███████╗██╗   ██╗██╗  ████████╗",
  "██╔══██╗██╔════╝██╔════╝██║   ██║██║  ╚══██╔══╝",
  "██████╔╝█████╗  ███████╗██║   ██║██║     ██║	  ",
  "██╔══██╗██╔══╝  ╚════██║██║   ██║██║     ██║	  ",
  "██║  ██║███████╗███████║╚██████╔╝███████╗██║	  ",
  "╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝ ╚══════╝╚═╝   "
};

void resultInit() {
  drawBorder({ 6, 3, 65, 19 }, white);
  for (int i = 0; i < 63; i++) {
	for (int j = 0; j < 17; j++) {
	  setBufferText(7 + i, 4 + j, " ");
	}
  }

  showResultFinish = false;
  score = 0;
  resultCoinNum = getCollectCoinNum();
  resultIceNum = getDestroyIceNum();
  hotPower = (float)getPeakTime() / (float)getGameElapsedTime() * 100.0f;
  maxScore = resultCoinNum * 1000 + resultIceNum * 500 + hotPower * 10;

  resultSound = opensound((char*)"./Sound/result.mp3");
  playsound(resultSound, 0);
}

void resultUpdate() {
  for (int i = 0; i < 6; i++) {
	gotoxy(17, 5 + i);
	std::cout << resultWord[i];
  }

  showResultFinish = showResultInfo();

  if (inport(PK_ENTER)) {
	setScene(Scene::TITLE);
	return;
  }
}

void resultRender() {
  
}

void resultDestroy() {
  closesound(resultSound);
}

bool showResultInfo() {
  static int frame = 0;
  static int i = 0;
  const int startX = 22;
  if (showResultFinish) {
	frame = 0;
	i = 0;
	gotoxy(startX + 26, 19);
	std::cout << score;
	score = score + 123 > maxScore ? maxScore : score + 123;
	return true;
  }
  if (i > 5) {
	return true;
  }
  if (frame < 20) {
	frame++;
  } else {
	frame = 0;
	i++;
  }
  setTextColor({ 51 * i, 51 * i, 51 * i });

  gotoxy(startX + i - 1, 12);
  std::cout << " ";
  gotoxy(startX + 20 + i - 1, 12);
  std::cout << " ";
  gotoxy(startX + i - 1, 14);
  std::cout << " ";
  gotoxy(startX + 20 + i - 1, 14);
  std::cout << " ";
  gotoxy(startX + i - 1, 16);
  std::cout << " ";
  gotoxy(startX + 20 + i - 1, 16);
  std::cout << " ";
  gotoxy(startX + i - 1, 19);
  std::cout << " ";
  gotoxy(startX + 20 + i - 1, 19);
  std::cout << " ";
  
  gotoxy(startX + i, 12);
  std::cout << "コイン";
  gotoxy(startX + 20 + i, 12);
  std::cout << resultCoinNum;
  gotoxy(startX + i, 14);
  std::cout << "破壊した氷山";
  gotoxy(startX + 20 + i, 14);
  std::cout << resultIceNum;
  gotoxy(startX + i, 16);
  std::cout << "ほっと力";
  gotoxy(startX + 20 + i, 16);
  std::cout << hotPower << "%";
  gotoxy(startX + i, 19);
  std::cout << "スコア";
  gotoxy(startX + 20 + i, 19);
  std::cout << score;
  score = score + 77 > maxScore ? maxScore : score + 77;

  resetTextColor();
  return false;
}