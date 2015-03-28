#pragma once
#include <iostream>
#include "GUIElement.h"
#include <SFML/Graphics.hpp>
class GUIButton : public GUIElement
{
private:
	
	

public:
	GUIButton(sf::Vector2f pos, sf::Vector2f siz) : GUIElement(pos, siz){};
	virtual ~GUIButton();
	void draw();
	bool getAction();
	void setAction(bool);
};

