#define CONIOEX
#include "conioex.h"
#include "gameResult.h"
#include "scene.h"
#include "iostream"

int hSound;

void resultInit() {
  // hSound = opensound((char*)"gameover.mp3");
  // playsound(hSound, 0);
}

void resultUpdate() {
  static int frame = 0;
  if (frame < 100) {
	frame++;
	return;
  }
  if (_kbhit()) {
	setScene(SCENE::Title);
	frame = 0;
  }
}

void resultRender() {
  gotoxy(1, 1);
  std::cout << " ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗██╗     ███████╗ █████╗ ██████╗" << std::endl;
  std::cout << "██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔════╝██║     ██╔════╝██╔══██╗██╔══██╗" << std::endl;
  std::cout << "██║  ███╗███████║██╔████╔██║█████╗      ██║     ██║     █████╗  ███████║██████╔╝" << std::endl;
  std::cout << "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║     ██║     ██╔══╝  ██╔══██║██╔══██╗" << std::endl;
  std::cout << "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╗███████╗███████╗██║  ██║██║  ██║" << std::endl;
  std::cout << " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝" << std::endl;
}

void resultDestroy() {
  // stopsound(hSound);
  // closesound(hSound);
}