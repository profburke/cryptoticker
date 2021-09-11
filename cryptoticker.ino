#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>
#include <Wire.h>
#include "WorldcoinClient.h"
#include "frames.h"

// Defines WIFI_SSID, WIFI_PWD, APIKEY as const char *
// secrets.h is in .gitignore so we don't commit these values.
#include "secrets.h"

const int UPDATE_INTERVAL_SECS = 10 * 60; // Update every 10 minutes
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = D3;
const int SDC_PIN = D5;

SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi   ui( &display );
WorldcoinClient worldcoin;
Ticker ticker;

bool readyForUpdate = false;
String currentCurrency = "USD";

FrameCallback frames[] = { drawDateTime, drawCurrency0, drawCurrency1, drawCurrency2, drawCurrency3 };
int numberOfFrames = 5;

OverlayCallback overlays[] = { drawHeaderOverlay };
int numberOfOverlays = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  display.init();
  display.clear();
  display.display();

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);

  WiFi.begin(WIFI_SSID, WIFI_PWD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print(worldcoin.getCurrencyName(0));
  Serial.print(": ");
  Serial.println(worldcoin.getCurrencyValue(0));

  ui.setTargetFPS(30);

//  ui.setActiveSymbol(activeSymbole);
//  ui.setInactiveSymbol(inactiveSymbole);

  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, numberOfFrames);

  ui.setOverlays(overlays, numberOfOverlays);
  ui.init();

  updateData(&display);

  ticker.attach(UPDATE_INTERVAL_SECS, setReadyForUpdate);
}

void loop() {
  if (readyForUpdate && ui.getUiState()->frameState == FIXED) {
    updateData(&display);
  }

  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your time budget.
    delay(remainingTimeBudget);
  }
}
