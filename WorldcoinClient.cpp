#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#include "WorldcoinClient.h"

const char *host = "www.worldcoinindex.com";

WorldcoinClient::WorldcoinClient() {
  currencies[0] = Currency("Bitcoin", "BTC", "n/a");
  currencies[1] = Currency("Ethereum", "ETH", "n/a");
  currencies[2] = Currency("Litecoin", "LTC", "n/a");
  currencies[3] = Currency("Monero", "XMR", "n/a");
}

Currency WorldcoinClient::getCurrency(int index) {
  return currencies[index];
}

void WorldcoinClient::update(String apiKey) {
  JsonStreamingParser parser;
  parser.setListener(this);

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  String url = "https://" + String(host) + "/apiservice/ticker?key=" + apiKey + "&label=ethbtc-ltcbtc-xmrbtc-btcbtc&fiat=usd";
  Serial.println("Requesting URL: " + url);

  if (https.begin(*client, url)) {
    int httpCode = https.GET();

    Serial.printf("[HTTPS] GET ... code: %d\n", httpCode);

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        for (int i = 0; i < payload.length(); i++) {
          parser.parse(payload.charAt(i));
        }
      }
    }
  }

  // dealloc client?
}

int WorldcoinClient::findName(String candidate) {
  int result = -1;
  for (int i = 0; i < numCurrencies; i ++) {
    if (candidate == currencies[i].getName()) {
      result = i;
      break;
    }
  }
  return result;
}

// SAX functions

void WorldcoinClient::startDocument() {
  Serial.println("start document");
}

void WorldcoinClient::endDocument() {
  Serial.println("end document");    
}

void WorldcoinClient::startObject() {
  currentName = "";
  currentSymbol = "";
  currentValue = "";
}

void WorldcoinClient::endObject() {
  int index = findName(currentName);
  if (index > -1) {
    currencies[index] = Currency(currentName, currentSymbol, currentValue);
  }
}

void WorldcoinClient::startArray() {
  Serial.println("start array");
}

void WorldcoinClient::endArray() {
  Serial.println("end array");
}

void WorldcoinClient::whitespace(char c) {
  Serial.println("whitespace");
}

void WorldcoinClient::key(String key) {
  Serial.println("key: " + key);
  currentKey = String(key);
}

void WorldcoinClient::value(String value) {
  Serial.println("value: " + value);
  if (currentKey == "Name") {
    currentName = value;
  }

  if (currentKey == "Label") {
    currentSymbol = value;
  }

  if (currentKey == "Price") {
    currentValue = String(value.toDouble(), 3);
  }
}
