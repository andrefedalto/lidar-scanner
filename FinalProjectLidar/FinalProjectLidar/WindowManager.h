#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "Cube.h"
#include "InputManager.h"
#include "GUI.h"
#include "GUIButton.h"

#define PI 3.14159265

class WindowManager
{
private:
	sf::Window* _window;

	sf::Vector2i mousePos;
	sf::Vector2f mouseDelta;        //Moviment of the mouse between the last and current frame
	sf::Vector3f _position;         //Position of the Scene
	sf::Vector3f savedPostion;
	sf::Vector2f rotation;
	sf::Vector2f movement;

	bool _running;

	std::vector<Cube> *cubes;

	InputManager* inputs;

	bool onMenu;
	bool autoRotate;

	sf::Clock clock;
	sf::Time keyTimer;

public:
	WindowManager(sf::Window*, InputManager*, std::vector<Cube>*);
	~WindowManager();

	void setup();
	void update();
	void draw3D();              //Setup the Scene for 3D drawing
	void draw2D();              //Setup the Scene for 2D Drawing
	bool running();
	void handleInput();         //Use the INputManager to move the view
	void setOnMenu(bool);       //Change Menu State
	void newCapture();          //Temporary function for testing
	sf::Vector2f getMouseDelta(); //Return the mouse delta
};

