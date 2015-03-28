#pragma once

#include "GUIElement.h"
#include <SFML\Graphics.hpp>

//This class is simply to draw backgrounds. It is used behing the Title and behind all buttons
class GUIRect : public GUIElement
{
private:
	bool border;
	sf::Vector3f bgColor;
	sf::Vector3f borderColor;
public:
	GUIRect(sf::Vector2f pos, sf::Vector2f siz) : GUIElement(pos, siz){};
	virtual ~GUIRect();

	void draw();
	void setBackgroundColor(sf::Color);
	void setBorderColor(sf::Color);
};

