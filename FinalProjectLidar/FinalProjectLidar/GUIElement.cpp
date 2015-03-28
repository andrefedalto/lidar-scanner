#include "GUIElement.h"


GUIElement::GUIElement(sf::Vector2f pos, sf::Vector2f siz)
{
	position = pos;
	initialPosition = pos;
	size = siz;
	hide = false;
	action = false;
	lastAction = clock.getElapsedTime();
	
}


GUIElement::~GUIElement()
{
}

void GUIElement::draw()
{

}

void GUIElement::setId(int value)
{
	id = value;
}

int GUIElement::getId()
{
	return id;
}

sf::Rect<float> GUIElement::getRect()
{
	sf::FloatRect rect(position, size);
	return rect;
}

void GUIElement::setAction(bool var){
	action = var;
}

bool GUIElement::getAction()
{
	return false;
}

void GUIElement::setHidden(bool var)
{
	hide = var;
}

bool GUIElement::getHidden()
{
	return hide;
}

void GUIElement::setText(std::string var)
{
	text = var;
}

std::string GUIElement::getText()
{
	return text;
}

void GUIElement::setPosition(sf::Vector2f pos)
{
	position = pos;
}

sf::Vector2f GUIElement::getInitialPos(){
	return initialPosition;
}