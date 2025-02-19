#define CONIOEX
#include "conioex.h"
#include "inputKey.h"

bool isOldPress = false;
bool isPress = false;
bool isRelease = false;

KeyMapping inputKeys[KeyType::KEY_NUM] = {
  { KeyType::UP, PK_UP },
  { KeyType::DOWN, PK_DOWN },
  { KeyType::SPACE, PK_SP }
};

KeyState inputKeyState[KeyType::KEY_NUM];

void inputKeyUpdate() {
  for (int i = 0; i < KeyType::KEY_NUM; i++) {
	inputKeyState[i].isPrePress = inputKeyState[i].isPress;
	inputKeyState[i].isPress = inport(inputKeys[i].KeyCode);
  }
}

bool getKeydown(KeyType type) {
  return !inputKeyState[type].isPrePress && inputKeyState[type].isPress;
}