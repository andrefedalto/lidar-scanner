#include "Cube.h"


Cube::Cube(sf::Vector3f pos, float size, sf::Color colour)
{
	_position = pos;
	_size = size;
	_colour = colour;
}


Cube::~Cube()
{
}

void Cube::draw()
{
	glPushMatrix();
    
	glTranslatef(_position.x, _position.y, _position.z);
    
    //Change the color of the cube to OpenGL RGB (goes from [0,1])
	glColor3f((float) _colour.r / 255.f, (float) _colour.g / 255.f, (float) _colour.b / 255.f);

    //Draw a cube using the position of the cube as the center.
	glBegin(GL_QUADS);

		glVertex3f(-(_size/2.0f), -(_size/2.0f), -(_size/2.0f));
		glVertex3f(-(_size/2.0f), (_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), (_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), -(_size/2.0f), -(_size/2.0f));

		glVertex3f(-(_size/2.0f), -(_size/2.0f), (_size/2.0f));
		glVertex3f(-(_size/2.0f), (_size/2.0f), (_size/2.0f));
		glVertex3f((_size/2.0f), (_size/2.0f), (_size/2.0f));
		glVertex3f((_size/2.0f), -(_size/2.0f), (_size/2.0f));

		glVertex3f(-(_size/2.0f), -(_size/2.0f), -(_size/2.0f));
		glVertex3f(-(_size/2.0f), (_size/2.0f), -(_size/2.0f));
		glVertex3f(-(_size/2.0f), (_size/2.0f), (_size/2.0f));
		glVertex3f(-(_size/2.0f), -(_size/2.0f), (_size/2.0f));

		glVertex3f((_size/2.0f), -(_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), (_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), (_size/2.0f), (_size/2.0f));
		glVertex3f((_size/2.0f), -(_size/2.0f), (_size/2.0f));

		glVertex3f(-(_size/2.0f), -(_size/2.0f), (_size/2.0f));
		glVertex3f(-(_size/2.0f), -(_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), -(_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), -(_size/2.0f), (_size/2.0f));

		glVertex3f(-(_size/2.0f), (_size/2.0f), (_size/2.0f));
		glVertex3f(-(_size/2.0f), (_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), (_size/2.0f), -(_size/2.0f));
		glVertex3f((_size/2.0f), (_size/2.0f), (_size/2.0f));

	glEnd();

	glPopMatrix();
}

void Cube::setPosition(float x, float y, float z)
{
	_position = sf::Vector3f(x, y, z);
}

sf::Vector3f Cube::getPosition()
{
	return _position;
}
float Cube::getSize()
{
	return _size;
}
sf::Color Cube::getColor()
{
	return _colour;
}

void Cube::setSize(float var)
{
	_size = var;
}