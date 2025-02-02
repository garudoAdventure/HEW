#pragma once

enum Scene {
  TITLE,
  GAME,
  RESULT,

  NUM,
};

void sceneInit();
void sceneUpdate();
void sceneRender();
void sceneDestroy();

void setScene(Scene scene);
void checkScene();