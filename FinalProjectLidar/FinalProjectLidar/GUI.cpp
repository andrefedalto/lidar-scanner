#include "GUI.h"


GUI::GUI()
{
	
}


GUI::~GUI()
{
    //Delete all GUI elements
    for (int i = 0; i < _UI.size(); i++)
	{
		delete _UI.at(i);
		_UI.at(i) = NULL;
	}
}

void GUI::update()
{
    //As the SFML wrapper for the mouse is pretty decent, we decided not to use the InputManager for this
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
        //Get the mouse position
		sf::Vector2f position = (sf::Vector2f) sf::Mouse::getPosition(*window);
		
        //Changing the coordinate to match with OpenGL
		position.x -= (window->getSize().x / 2);
		position.y -= (window->getSize().y / 2);

        //Check if mouse is inside one of the GUIElements. If so, change it's state to active
		for (int i = 0; i < _UI.size(); i++)
		{
			if (_UI.at(i)->getRect().contains(position) && !_UI.at(i)->getHidden()) 
			{
				_UI.at(i)->setAction(true);
			}
		}
	}

}

void GUI::draw(sf::Vector2i windowSize)
{

    //Draws each GUIElement that is not hidden. It also draws the text (if exists) using the SFML library
	for (int i = 0; i < _UI.size(); i++)
	{

		if (!_UI.at(i)->getHidden())
		{
			
			_UI.at(i)->draw();
			
			if (_UI.at(i)->getText().length() > 0)
			{
                //As we are using the openGL and SFML to draw on the screen, we must save the OpenGL states before using the SFML.
				window->pushGLStates();
				sf::Text text(_UI.at(i)->getText(), font);
				text.setPosition(sf::Vector2f(_UI.at(i)->getRect().left + 30 + window->getSize().x / 2.0f, _UI.at(i)->getRect().top + 22 + window->getSize().y / 2.0f));
				text.setCharacterSize(24);
			
				text.setColor(sf::Color::White);
				window->draw(text);
				window->popGLStates();
			}
			
		}
	}
}

//Adds a new element to the GUI. It also set an unique identifier for each GUI element for helping remove, hide and show elements
void GUI::add(GUIElement* elem)
{
	
	elem->setId(curId++);
	elem->setHidden(false);
	_UI.push_back(elem);
}


//Remove the GUI Element.
//This function is not useful at the moment but could be useful if dynamic elemnts would be added and removed from the GUI
void GUI::remove(GUIElement* elem)
{
	for (int i = 0; i < _UI.size(); i++)
	{
		if (_UI.at(i)->getId() == elem->getId())
		{
			_UI.erase(_UI.begin() + i);
		}
	}
}

void GUI::setWindow(sf::RenderWindow* win)
{
	window = win;
	font.loadFromFile("OpenSans-Light.ttf");
}

//Hide element from GUI
void GUI::hide(GUIElement* elem)
{
	for (int i = 0; i < _UI.size(); i++)
	{
		if (_UI.at(i)->getId() == elem->getId())
		{
			_UI.at(i)->setHidden(true);
		}
	}
}

//Set element as not hidden, so it can be displayed and interected in the GUI
void GUI::show(GUIElement* elem)
{
	for (int i = 0; i < _UI.size(); i++)
	{
		if (_UI.at(i)->getId() == elem->getId())
		{
			_UI.at(i)->setHidden(false);
		}
	}
}

//Set the position of whole GUI relative to its initial position
void GUI::setPosition(sf::Vector2f newPos)
{
	for (int i = 0; i < _UI.size(); i++)
	{
		_UI.at(i)->setPosition(sf::Vector2f(_UI.at(i)->getInitialPos().x + newPos.x, _UI.at(i)->getInitialPos().y + newPos.y));
	}

}