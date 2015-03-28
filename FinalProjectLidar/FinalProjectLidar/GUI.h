#pragma once
#include <iostream>
#include <vector>

#include <SFML/OpenGL.hpp>
#include "GUIElement.h"
#include "GUIButton.h"
#include "GUIFrame.h"
#include "GUIRect.h"


class GUI
{
private:
	std::vector<GUIElement*> _UI;       //Elements that compose the GUI
	int curId;                          //Current identifier value, used for assigning an unique ID for each element
	sf::RenderWindow* window;           //Window used for aquiring the mouse position

	sf::Font font;
public:
	GUI();
	~GUI();
	void update();                      //Updates _UI state
	void add(GUIElement*);              //Add element to the _UI
	void remove(GUIElement*);           //Remove element from _UI
	void show(GUIElement*);             //Set hidden state as false on the selected Element
	void hide(GUIElement*);             //Set hidden state as true
	void draw(sf::Vector2i);            //Draw all elements that have the hidden state as false
	void setWindow(sf::RenderWindow*); 
	void setPosition(sf::Vector2f);
};

