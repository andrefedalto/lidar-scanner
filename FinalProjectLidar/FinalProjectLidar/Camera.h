#pragma once

#include <SFML/Window.hpp>

class Camera
{
private:
	float _speed;
public:
	Camera();
	~Camera();

	sf::Vector3f position;

};

