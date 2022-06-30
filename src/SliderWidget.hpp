#pragma once

#define DEFAULT_SLIDER_VALUE 2

struct SliderWidget
{
    int value;
    int maxValue;
    SliderWidget(int value, int maxValue){
        this->value = value;
        this->maxValue = maxValue;
    }
};
