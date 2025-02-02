#define CONIOEX
#include "conioex.h"
#include "gameMap.h"
#include "buffer.h"
#include "gameField.h"
#include "player.h"
#include "game.h"

void mapInit() {
}

void mapUpdate() {
  static int frameCount = 0;
  frameCount = (frameCount + 1) % 200;
  if (inport(PK_BS) && frameCount > 50) {
	setGameScene(GameScene::FIELD);
	frameCount = 0;
	return;
  }
  
  clearMap();
  renderMap();
}

void mapRender() {

}

void mapDestroy() {
  clearMap();
}

void clearMap() {
  for (int i = 0; i < GameFieldWidth - 1; i++) {
	for (int j = 0; j < GameFieldHeight; j++) {
	  setBufferBgColor(i, j, { 0, 0, 0 });
	  setBufferText(i, j, " ");
	}
  }
}

void renderMap() {
  for (int i = 0; i < GameFieldHeight; i++) {
	for (int j = 0; j < GameFieldWidth - 1; j++) {
	  switch (map[i][j]) {
		case '*':
		  setBufferText(j, i, "*");
		  break;
		case 'O':
		  setBufferText(j, i, "O");
		  break;
	  }
	}
  }
}