#pragma once

struct GameResultData {
  int coin;
  int ice;
  float hot;
  int score;
};

void resultInit();
void resultUpdate();
void resultRender();
void resultDestroy();

bool showResultInfo();