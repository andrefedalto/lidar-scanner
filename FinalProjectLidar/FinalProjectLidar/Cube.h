#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

/*
 Class used to store each measurement from the Lidar-LITE. Each object from this class stores one measurement
 */

class Cube
{
private:
	sf::Vector3f _position;
	sf::Color _colour;
	float _size;
public:
	Cube(sf::Vector3f position, float size, sf::Color colour);
	~Cube();
	void draw();
	void update();

	void setPosition(float, float, float);

	sf::Vector3f getPosition();
	float getSize();
	sf::Color getColor();
	void setSize(float);
};

