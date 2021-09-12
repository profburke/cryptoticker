#include "Currency.h"

Currency::Currency() {
  this->name = "";
  this->symbol = "";
  this->value = "";  
}

Currency::Currency(String name, String symbol, String value) {
  this->name = name;
  this->symbol = symbol;
  this->value = value;
}

String Currency::getName() {
  return name;
}

String Currency::getSymbol() {
  return symbol;
}

String Currency::getValue() {
  return value;
}
