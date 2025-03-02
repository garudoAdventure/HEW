#pragma once

void fanInit();
void fanUpdate();
void fanRender();
void fanDestroy();

void drawFan();
void drawFlag();
void clearFanScene();
void setFanActive(bool active);
bool getFanActive();
void showCurrentVelocity(float velocity);
void stopSlidingSound();