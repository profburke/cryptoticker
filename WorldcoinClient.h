
#pragma once

#include <JsonListener.h>
#include <JsonStreamingParser.h>

class WorldcoinClient: public JsonListener {
  private:
    String currentKey;
    String currentSymbol;
    String currentName;
    String currentValue;

    static const int numCurrencies = 4;
    String names[numCurrencies];
    String symbols[numCurrencies];
    String values[numCurrencies];

    int findName(String candidate);

  public:

    WorldcoinClient();

    void update(String apiKey);

    String getCurrencyName(int index);
    String getCurrencySymbol(int index);
    String getCurrencyValue(int index);

    // do these need to be public?
    virtual void startDocument();                                                                                 
    virtual void endDocument();                                                                                   

    virtual void startObject();      
    virtual void endObject();                                                                                     

    virtual void startArray();                                                                                    
    virtual void endArray();                                                                                      

    virtual void whitespace(char c);                                                                              
    virtual void key(String key);                                                                                 
    virtual void value(String value);                                                                             
};

