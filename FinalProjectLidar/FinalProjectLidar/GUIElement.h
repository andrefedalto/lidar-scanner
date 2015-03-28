#pragma once

#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\Window.hpp>


//This Super Class has all the attributes and methods common to all the GUIElements
//The methods and attributes from this class just store basic information of the Elements.
class GUIElement
{
protected:
	int id;
	bool hide;
	bool action;
	sf::Vector2f position;
	sf::Vector2f initialPosition;
	sf::Vector2f size;

	std::string text;

	sf::Clock clock;
	sf::Time lastAction;
public:
	GUIElement(sf::Vector2f, sf::Vector2f);
	~GUIElement();

	virtual void draw();
	virtual bool getAction();
	virtual void setAction(bool);

	sf::Rect<float> getRect();

	void setId(int);
	int getId();
	void setHidden(bool);
	bool getHidden();
	void setText(std::string);
	std::string getText();
	void setPosition(sf::Vector2f);
	sf::Vector2f getInitialPos();
};

