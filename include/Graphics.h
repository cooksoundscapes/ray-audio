#pragma once
#include <raylib.h>
#include <vector>
#include "XmlParser.h"

class Graphics
{
public:
    Graphics(int w, int h);
    ~Graphics();
 
    Color fromHex(int);
    
    void loop(XmlParser*);    
private:
    
    int width, height;
};