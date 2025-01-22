#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "player.h"
#include "field.h"
#include "buffer.h"
#include "UI.h"

bool isPause;

void gameInit() {
  renderBorder(0, 0, 64, 25);
  renderBorder(64, 0, 16, 8);
  renderBorder(64, 8, 16, 8);
  renderBorder(64, 16, 16, 8);

  isPause = false;
  fieldInit();
  playerInit();
}

void gameUpdate() {
  static int frame = 0;

  if (isPause) {
	if (_kbhit()) {
	  isPause = false;
	}
	return;
  }
  if (inport(PK_BS)) {
	isPause = true;
  }
  fieldUpdate();
  playerUpdate();
}

void gameRender() {
  bufferRender();
}

void gameDestroy() {
  playerDestroy();
}