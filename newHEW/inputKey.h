#pragma once

enum KeyType {
  UP,
  DOWN,
  SPACE,

  KEY_NUM
};

struct KeyMapping {
  KeyType type;
  int KeyCode;
};

struct KeyState {
  bool isPrePress;
  bool isPress;
  bool isRelease;
};

void inputKeyUpdate();
bool getKeydown(KeyType type);