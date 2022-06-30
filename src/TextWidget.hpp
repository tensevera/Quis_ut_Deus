#pragma once

#include <string>

struct TextWidget
{
    std::string text;
    TextWidget(const char* text){
        this->text = text;
    } 
};
