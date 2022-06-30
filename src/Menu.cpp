#include "Menu.hpp"
#include "TextureManager.hpp"
#include <iostream>

Menu::Menu(SDL_Renderer* ren, TTF_Font* font) {
    this->renderer = ren;
    this->font = font;
    freeSliderPoints = 3;
    widgetHead = widgetTail = nullptr;
}
Menu::~Menu() {
    clearWidgets();
}

void Menu::clearWidgets() {
    Widget* current = widgetHead;
    Widget* next = nullptr;

    while (current != nullptr)
    {
        if(current->data != nullptr)
            delete(current->data);
        next = current->next;
        delete(current);
        current = next;
    }
 
    widgetHead = widgetTail = nullptr;
}

void Menu::addWidget(const char* name){
    Widget *w;

    if(widgetTail != nullptr)
        w = new Widget(name,WIDGET_OFFSET_X,widgetTail->y+WIDGET_SPACING);
    else
        w = new Widget(name,WIDGET_OFFSET_X, 100);

    if(!widgetHead)
    {
        widgetTail = w;
        activeWidget = w;
        widgetHead = w;

        w->prev = nullptr;
    }
    else
    {
    widgetTail->next = w; 
    w->prev = widgetTail;
    widgetTail = w;
    }
}

void Menu::addWidget(const char* name, int value, int maxValue){
    addWidget(name);
    Widget *w = widgetTail;

    w->data = new SliderWidget(value,maxValue);
    w->type = sliderWidget;
}

void Menu::addWidget(const char* name, const char* text, bool fixed){
    addWidget(name);
    Widget *w = widgetTail;
    if(fixed)
    {
        w->data = new TextWidget(text);
        w->type = textWidget;
    }
    else
    {
        w->data = new InputWidget(text);
        w->type = inputWidget;
    }
}

const char* Menu::getActiveWidgetName() const {
    if(activeWidget)
        return activeWidget->name.c_str();
    return "";
}

void Menu::drawWidget(Widget* w) {
    SDL_Color c;

    c.r = c.g = c.b = 0;

    if (w == activeWidget)
    {
        TextureManager::drawText(">", w->x - WIDGET_SPACING, w->y, c.r, c.g, c.b,font,renderer);
    }

    TextureManager::drawText((w->name).c_str(), w->x, w->y, c.r, c.g, c.b,font,renderer);

    switch (w->type)
    {
    case sliderWidget:
    {
        SliderWidget* s = (SliderWidget*)w->data;
        const char* slideValue = (std::to_string(s->value) + "/" + std::to_string(s->maxValue)).c_str();
        TextureManager::drawText(slideValue,w->x + 2 * RECTANGLE_OFFSET_X + RECTANGLE_SIZE, w->y - RECTANGLE_OFFSET_Y,c.r,c.g,c.b,font,renderer); //actual value in numbers
        break;
    }
    case inputWidget:
    {
        InputWidget* i = (InputWidget*) w->data;
        TextureManager::drawText(i->text.c_str(), w->x + RECTANGLE_OFFSET_X,w->y,c.r,c.g,c.b,font,renderer);
        break;
    }
    case textWidget:
    {
        TextWidget* i = (TextWidget*) w->data;
        TextureManager::drawText(i->text.c_str(), w->x + RECTANGLE_OFFSET_X,w->y,c.r,c.g,c.b,font,renderer);
        break;
    }
    default: 
        break;
    }
}

bool Menu::changeValue(char input) {
    if(activeWidget->type!=inputWidget)
        return false;

    InputWidget* i = ((InputWidget*)activeWidget->data);
    if(i->text.size() > 10)
        return false;

    if(input == BACKSPACE_ASCII)
    {
        if((i->text).size() > 0) 
        {
            (i->text).erase((i->text).size() - 1);
            if(i->text.size() == 0)
                i->text = " ";
        }
        return true;
    }
    if(i->text[0] == ' ')
        i->text = input-UPPER_CASE_OFFEST;
    else
        i->text += input;
    return true;
}

bool Menu::changeValue(int input) {
    if(activeWidget->type != sliderWidget)
        return false;

    if(input > 0) 
    {
        if(freeSliderPoints > 0 && ((SliderWidget*)activeWidget->data)->value < ((SliderWidget*)activeWidget->data)->maxValue)
        {
            ++((SliderWidget*)activeWidget->data)->value;
            --freeSliderPoints;
        }
    }
    else
    {
        if(((SliderWidget*)activeWidget->data)->value > 1)
        {
            --((SliderWidget*)activeWidget->data)->value;
            ++freeSliderPoints;
        }
    }
    return true;
}

const char* Menu::getTextValue(Widget* w) const {
    if(w->type == inputWidget)
        return ((InputWidget*)w->data)->text.c_str();
    if(w->type == textWidget)
        return ((TextWidget*)w->data)->text.c_str();
    return "";
}
int Menu::getSliderValue(Widget* w) const {
    if(w->type != sliderWidget)
        return -1;
    return ((SliderWidget*)w->data)->value;
}

void Menu::drawMenu() {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    Widget *w = widgetHead;

    while(w != nullptr)
    {
        drawWidget(w);
        w = w->next;
    }
}

void Menu::skipToNextWidget() {
    activeWidget = activeWidget->next;
    if (!activeWidget)
        activeWidget = widgetHead;
    return;
}

void Menu::skipToPreviousWidget() {
    activeWidget = activeWidget->prev;
    if(!activeWidget)
        activeWidget=widgetTail;
    return;
}