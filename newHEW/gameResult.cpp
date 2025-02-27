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
#include "fstream"

GameResultData resultData;
int score;
bool showResultFinish;
int resultSound;

const int scoreIncrease = 17;

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
  resultData.coin = getCollectCoinNum();
  resultData.ice = getDestroyIceNum();
  resultData.hot = (float)getPeakTime() / (float)getGameElapsedTime() * 100.0f;
  resultData.score = resultData.coin * 1000 + resultData.ice * 500 + resultData.hot * 10;

  resultSound = opensound((char*)"./Sound/result.mp3");
  playsound(resultSound, 0);

  std::ofstream File;
  File.open("result.txt", std::ios::app);
  File << resultData.coin << "       ";
  File << resultData.ice << "      ";
  File << resultData.hot << "      ";
  File << resultData.score << "\n";
  File.close();
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
	score = score + scoreIncrease > resultData.score ? resultData.score : score + scoreIncrease;
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
  std::cout << resultData.coin;
  gotoxy(startX + i, 14);
  std::cout << "破壊した氷山";
  gotoxy(startX + 20 + i, 14);
  std::cout << resultData.ice;
  gotoxy(startX + i, 16);
  std::cout << "ほっと力";
  gotoxy(startX + 20 + i, 16);
  std::cout << resultData.hot << "%";
  gotoxy(startX + i, 19);
  std::cout << "スコア";
  gotoxy(startX + 20 + i, 19);
  std::cout << score;
  score = score + scoreIncrease > resultData.score ? resultData.score : score + scoreIncrease;

  resetTextColor();
  return false;
}