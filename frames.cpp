#include "frames.h"
#include "WorldcoinClient.h"

extern WorldcoinClient worldcoin;
extern const char *APIKEY;
extern bool readyForUpdate;
extern String currentCurrency;

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);

  // According to specs for HTTP date header, the various fields are all
  // fixed width; e.g.
  // Tue, 14 Sep 2021 13:39:24 GMT
  // So we can hard code the indices for substring...
  String date = worldcoin.getMRU().substring(0, 16);
  String time = worldcoin.getMRU().substring(17);
  
  display->drawString(64 + x, 5 + y, date);
  display->drawString(64 + x, 20 + y, time);
}

/*
 * Between the next several functions and the names, symbols, values arrays in WorldcoinClient
 * you'd think this was written in BASIC, not C++. Now that I've gotten past some getting up
 * to speed with Arduino/ESP8266 development, it's time to work on improving the code.
 *
 * However...
 *
 * Made an initial refactoring here where I only had two frame functions, one to draw the date/time
 * and one for each of the four currency frames. The idea being to pull the frame out of the 
 * OLEDDisplayUIState object and from there decide which currency to display.  However, there
 * is some weirdness with when the frame functions are being called during updates that results
 * in the wrong currency being displayed while the text is "scrolling" into place... so I've
 * undone those changes and will work on some other things while I take a little time to trace
 * through the display library.
 */
void drawCurrency0(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrency(0).getSymbol(), worldcoin.getCurrency(0).getValue());
}

void drawCurrency1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrency(1).getSymbol(), worldcoin.getCurrency(1).getValue());
}

void drawCurrency2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrency(2).getSymbol(), worldcoin.getCurrency(2).getValue());
}

void drawCurrency3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawCurrency(display, state, x, y, worldcoin.getCurrency(3).getSymbol(), worldcoin.getCurrency(3).getValue());
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
