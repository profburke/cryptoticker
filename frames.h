
#pragma once

#include <OLEDDisplayUi.h>

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency0(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency1(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency3(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrency(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y, String symbol, String price);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void setReadyForUpdate();
