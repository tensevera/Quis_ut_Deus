#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Widget.hpp"
#include "SliderWidget.hpp"
#include "InputWidget.hpp"
#include "TextWidget.hpp"

#define WIDGET_OFFSET_X 100
#define WIDGET_SPACING 50
#define RECTANGLE_SIZE 200
#define RECTANGLE_OFFSET_X 100
#define RECTANGLE_OFFSET_Y 5
#define UPPER_CASE_OFFEST 32
#define BACKSPACE_ASCII 8

class Menu
{
private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    Widget *activeWidget;
    Widget* widgetHead;
    Widget* widgetTail;

    int freeSliderPoints;

    void drawWidget(Widget* w);
public:
    Menu(SDL_Renderer* ren,TTF_Font* font);
    ~Menu();

    void addWidget(const char* name);
    void addWidget(const char* name, int value, int maxValue);
    void addWidget(const char* name, const char* text, bool fixed);
    void clearWidgets();

    void drawMenu();
    bool changeValue(char input);
    bool changeValue(int input);

    void skipToNextWidget();
    void skipToPreviousWidget();

    const char* getActiveWidgetName() const;
    const char* getTextValue(Widget* w) const;
    int getSliderValue(Widget* w) const;
};
