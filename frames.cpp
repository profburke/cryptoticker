#include "frames.h"
#include "WorldcoinClient.h"

extern WorldcoinClient worldcoin;
extern const char *APIKEY;
extern bool readyForUpdate;
extern String currentCurrency;

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String date = "99/99/9999";
  int textWidth = display->getStringWidth(date);
  display->drawString(64 + x, 5 + y, date);
  display->setFont(ArialMT_Plain_24);
}

void drawCurrency0(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrencySymbol(0), worldcoin.getCurrencyValue(0));
}

void drawCurrency1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrencySymbol(1), worldcoin.getCurrencyValue(1));
}

void drawCurrency2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrencySymbol(2), worldcoin.getCurrencyValue(2));
}

void drawCurrency3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrencySymbol(3), worldcoin.getCurrencyValue(3));
}

void drawCurrency(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y, String symbol, String price) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(2 + x, 5 + y, symbol);
  display->setFont(ArialMT_Plain_16);
  display->drawString(2 + x, 30 + y, price);
}

void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setColor(WHITE);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(102, 54, currentCurrency);
  display->drawHorizontalLine(0, 52, 128);
}

void drawProgress(OLEDDisplay *display, int percentage, String label) {
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display) {
  Serial.println("executing updateData");
  drawProgress(display, 30, "Updating time...");
  drawProgress(display, 50, "Updating price indices...");
  worldcoin.update(APIKEY);
  drawProgress(display, 80, "Updating SOMETHING ELSE...");
  readyForUpdate = false;
  drawProgress(display, 100, "Done...");
  delay(250);
}

void setReadyForUpdate() {
  Serial.println("Setting readyForUpdate to true");
  readyForUpdate = true;
}
