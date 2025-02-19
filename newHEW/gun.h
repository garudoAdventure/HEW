#pragma once

void gunInit();
void gunUpdate();
void gunRender();
void gunDestroy();

void drawCrosshair();
void drawCannon();
void shootEffect();
bool gunFireEffect();
void clearGunScreen();
void setGunActive(bool active);

void drawFire5x3();
void drawFire7x4();
void drawFire5x4();