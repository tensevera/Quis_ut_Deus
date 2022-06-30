#pragma once

#include <string>
#include <functional>

enum WidgetType {normalWidget, sliderWidget, inputWidget, textWidget};

struct Widget {
	std::string name;
	int x;
	int y;
	Widget *prev;
	Widget *next;
    WidgetType type;
    void (*data);
    Widget(const char* name, int x, int y){
        this->name = name;
        this->x = x;
        this->y = y;
        this->next = nullptr;
        this->data = nullptr;
        this->type = normalWidget;
    }
};