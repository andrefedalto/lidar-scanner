#include "GUIButton.h"



GUIButton::~GUIButton()
{
	action = false;
}

void GUIButton::draw()
{
    //Draws the inner color of the button
	sf::Color color(211, 84, 0);
	sf::Vector3f vec(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
    
    //Draws the border of the button
	glColor3f(0.9, 0.9, 0.9);
	glRectf(position.x-1, position.y - 1, position.x + size.x + 1, position.y + size.y+1);
	glColor3f(vec.x, vec.y, vec.z);

	glRectf(position.x, position.y, position.x+size.x, position.y+size.y);
    
    
    //This fix some bugs when the software starts. So a button cannot be active for more than 500ms
	sf::Time elapsed = clock.getElapsedTime();
	if (elapsed.asMilliseconds() - lastAction.asMilliseconds() > 500){
		action = false;
	}
}

void GUIButton::setAction(bool var){
	action = var;
}

//This function returns the current action and turns the button as inactive, so it won't be fired more than once.
//It also has a delay the button won't be triggered more than once every 500ms
bool GUIButton::getAction()
{
	sf::Time elapsed = clock.getElapsedTime();
	if (elapsed.asMilliseconds() - lastAction.asMilliseconds() > 500){
		lastAction = elapsed;
		bool temp = action;
		action = false;
		return temp;
	}
	else
	{
		return false;
	}
}
