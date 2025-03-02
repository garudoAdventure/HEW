#include "color.h"
#include "game.h"

Color fieldSkyBlue;
Color fieldSeaBlueLight;
Color fieldSeaBlue58;
Color fieldSeaBlue50;
Color fieldSeaBlue41;
Color fieldSeaBlueDark;
Color fieldCoinGold;
Color fieldCoinGoldLight;
Color fieldIceWhite;
Color fieldIceBlueLight;
Color fieldIceBlueDark;
Color fieldBrown;
Color fieldBrownLight;
Color fieldBrownDark;

const int colorNum = 14;
Color* allColor[colorNum] = {
  &fieldSkyBlue,
  &fieldSeaBlueLight,
  &fieldSeaBlue58,
  &fieldSeaBlue50,
  &fieldSeaBlue41,
  &fieldSeaBlueDark,
  &fieldCoinGold,
  &fieldCoinGoldLight,
  &fieldIceWhite,
  &fieldIceBlueLight,
  &fieldIceBlueDark,
  &fieldBrown,
  &fieldBrownLight,
  &fieldBrownDark,
};

void colorInit() {
  fieldSkyBlue = skyBlue;
  fieldSeaBlueLight = seaBlueLight;
  fieldSeaBlue58 = seaBlue58;
  fieldSeaBlue50 = seaBlue50;
  fieldSeaBlue41 = seaBlue41;
  fieldSeaBlueDark = seaBlueDark;
  fieldCoinGold = coinGold;
  fieldCoinGoldLight = coinGoldLight;
  fieldIceWhite = white;
  fieldIceBlueLight = iceBlueLight;
  fieldIceBlueDark = iceBlueDark;
  fieldBrown = brown;
  fieldBrownLight = lightBrown;
  fieldBrownDark = darkBrown;
}

void colorUpdate() {
  int dr = 0;
  int dg = 0;
  int db = 0;
  switch (getCountdown()) {
	case 10:
	  dr = -52;
	  dg = -40;
	  db = -109;
	  break;
	case 20:
	  dr = -186;
	  dg = -70;
	  db = 55;
	  break;
	case 30:
	  dr = 0;
	  dg = -51;
	  db = 0;
	  break;
	case 40:
	  dr = 155;
	  dg = 4;
	  db = -132;
	  break;
	case 50:
	  dr = -52;
	  dg = -19;
	  db = -6;
	  break;
  }
  for (int i = 0; i < colorNum; i++) {
	allColor[i]->r = allColor[i]->r + dr < 0 ? 0 : (allColor[i]->r + dr > 255 ? 255 : allColor[i]->r + dr);
	allColor[i]->g = allColor[i]->g + dg < 0 ? 0 : (allColor[i]->g + dg > 255 ? 255 : allColor[i]->g + dg);
	allColor[i]->b = allColor[i]->b + db < 0 ? 0 : (allColor[i]->b + db > 255 ? 255 : allColor[i]->b + db);
  }
}

Color getSkyBlue() {
  return fieldSkyBlue;
}

Color getSeaBlueLight() {
  return fieldSeaBlueLight;
}

Color getSeaBlue58() {
  return fieldSeaBlue58;
}

Color getSeaBlue50() {
  return fieldSeaBlue50;
}

Color getSeaBlue41() {
  return fieldSeaBlue41;
}

Color getSeaBlueDark() {
  return fieldSeaBlueDark;
}

Color getCoinGold() {
  return fieldCoinGold;
}

Color getCoinGoldLight() {
  return fieldCoinGoldLight;
}

Color getIceWhite() {
  return fieldIceWhite;
}

Color getIceBlueLight() {
  return fieldIceBlueLight;
}

Color getIceBlueDark() {
  return fieldIceBlueDark;
}

Color getBrown() {
  return fieldBrown;
}

Color getBrownLight() {
  return fieldBrownLight;
}

Color getBrownDark() {
  return fieldBrownDark;
}