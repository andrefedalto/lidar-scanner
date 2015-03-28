#pragma once

#include <vector>
#include <iostream>

#include "GUIElement.h"
#include "Cube.h"

//Class displayed the preview of the saved files.
//It stores an array of cubes that is loaded file and stored to be displayed if the user is currently viewing the saved files.
class GUIFrame : public GUIElement
{
private:
	std::vector<Cube> cubes;            //Array of cubes to be displayed from file
	sf::Vector2i winSize;               //Size of the window
	float rotation;
	float maxMag;                       //Max magnitude from all cubes' position
	float offset;
public:
	GUIFrame(sf::Vector2f pos, sf::Vector2f siz) : GUIElement(pos, siz){};
	virtual ~GUIFrame();

	void draw();
	void drawCubes();

	void setCubes(std::vector<Cube>&);

	void setWindowSize(int, int);

	bool getAction();
};

