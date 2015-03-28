#pragma once


#include <vector>
#include <string>

#include <cmath>

#include "lib/tinydir.h"

#include "GUI.h"
#include "InputManager.h"
#include "Cube.h"

class Menu
{
private:
	GUI gui;
	GUIButton* exit;
	GUIButton* newCapture;
	GUIButton* load;
	GUIButton* save;
	GUIButton* back;
	GUIButton* down;
	GUIButton* hide;
	InputManager* input;

	sf::RenderWindow* window;

	sf::Text title;
	sf::Font font;

	std::vector<std::string> files;  //Store saved file names
	int curFile;

	std::vector <GUIFrame*> frames; //Store frames to draw the 3D preview of saved files
	bool running;
	sf::Vector2i windowSize;
    
	std::vector<Cube> * cubes;      //Reference to the main array of cubes
    
    //Used to the "hide and show Menu" animation
	sf::Clock clock;
	sf::Time hideTime;
	bool animation;
	float dir;
	float animationOffset;

public:
	Menu(InputManager*, sf::RenderWindow*, std::vector<Cube>*);
	~Menu();
	int update();
	void draw();
	void setRunning(bool);
	bool getRunning();

	void hideOption();
	void showOptions();
	std::vector<std::string> getFileNames(); //Reference: https://github.com/cxong/tinydir

};

