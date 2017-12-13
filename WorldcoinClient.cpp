#include <ESP8266WiFi.h>                                                                                          
#include <WiFiClientSecure.h>                                                                                           
#include "WorldcoinClient.h"  

const char *host = "www.worldcoinindex.com";
const int httpsPort = 443;

WorldcoinClient::WorldcoinClient() {
  for (int i = 0; i < numCurrencies; i++) {
    values[i] = "n/a";
  }
  names[0] = "Bitcoin";
  names[1] = "Ethereum";
  names[2] = "Litecoin";
  names[3] = "Monero";
  symbols[0] = "BTC";
  symbols[1] = "ETH";
  symbols[2] = "LTC";
  symbols[3] = "XMR";
}

String WorldcoinClient::getCurrencyName(int i) {
  return names[i];
}

String WorldcoinClient::getCurrencySymbol(int i) {
  return symbols[i];
}

String WorldcoinClient::getCurrencyValue(int i) {
  return values[i];
}

void WorldcoinClient::update(String apiKey) {
  JsonStreamingParser parser;
  parser.setListener(this);
  WiFiClientSecure client;

  Serial.print("Connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {                                                        
    Serial.println("connection failed");                                                                          
    return;                                                                                                       
  }                                                                                                               

  Serial.print("API KEY: ");
  Serial.println(apiKey);
  String url = "/apiservice/json?key=" + apiKey;
  Serial.print("Requesting URL: ");                                                                               
  Serial.println(url);                                                                                            
                                                                                                                  
  // This will send the request to the server                                                                     
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +                                                           
               "Host: " + host + "\r\n" + 
               "User-Agent: ct 1.0\r\n" +                                                                
               "Connection: close\r\n\r\n");                                                                      
  
  int retryCounter = 0;                                                                                           
  while(!client.available()) {                                                                                    
    delay(1000);                                                                                                  
    retryCounter++;                                                                                               
    if (retryCounter > 10) {                                                                                      
      return;                                                                                                     
    }                                                                                                             
  }

  int pos = 0;                                                                                                    
  boolean isBody = false;                                                                                         
  char c;                                                                                                         
                                                                                                                  
  int size = 0;                                                                                                   
  client.setNoDelay(false);                                                                                       
  while(client.connected()) {                                                                                     
    while((size = client.available()) > 0) {                                                                      
      c = client.read();
      if (c == '{' || c == '[') {                                                                                 
        isBody = true;                                                                                            
      }                                                                                                           
      if (isBody) {                                                                                               
        parser.parse(c);                                                                                          
      }                                                                                                           
    }                                                                                                             
  }                  
}

int WorldcoinClient::findName(String candidate) {
  int result = -1;
  for (int i = 0; i < numCurrencies; i ++) {
    if (candidate == names[i]) {
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
    values[index] = currentValue;
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
  currentKey = String(key);
}

void WorldcoinClient::value(String value) {
  if (currentKey == "Name") {
    currentName = value;
  }

  if (currentKey == "Label") {
    currentSymbol = value;
  }

  if (currentKey == "Price_usd") {
    currentValue = value;
  }
}


