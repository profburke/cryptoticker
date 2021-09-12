#pragma once

#include <Arduino.h>

class Currency {
  private:
    String name;
    String symbol;
    String value;
    
  public:
    // Wow! My C++ is rusty. Doesn't compile without a default
    // constructor being defined. Need to look into that.
    Currency();
    Currency(String name, String Symbol, String value);
    String getName();
    String getSymbol();
    String getValue();
};
