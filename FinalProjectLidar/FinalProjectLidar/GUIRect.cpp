#include "GUIRect.h"


GUIRect::~GUIRect()
{
}

void GUIRect::draw()
{
	if (!border)
	{
		borderColor = bgColor;
	}
	glColor3f(borderColor.x, borderColor.y, borderColor.z);
	glRectf(position.x - 1, position.y - 1, position.x + size.x + 1, position.y + size.y + 1);
	glColor3f(bgColor.x, bgColor.y, bgColor.z);

	glRectf(position.x, position.y, position.x + size.x, position.y + size.y);

}


void GUIRect::setBackgroundColor(sf::Color var)
{
	border = false;
	bgColor = sf::Vector3f(var.r / 255.0f, var.g / 255.0f, var.b / 255.0f);
	
}

void GUIRect::setBorderColor(sf::Color var){
	border = true;
	borderColor = sf::Vector3f(var.r / 255.0f, var.g / 255.0f, var.b / 255.0f);
}