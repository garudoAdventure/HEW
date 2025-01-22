#pragma once

enum SCENE {
  Title,
  Game,
  Result,
  Over,

  NUM,
};

void sceneInit();
void sceneUpdate();
void sceneRender();
void sceneDestroy();

void setScene(SCENE scene);
void checkScene();