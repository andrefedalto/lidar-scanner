#include "GUIFrame.h"


GUIFrame::~GUIFrame()
{
}

void GUIFrame::draw()
{

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glRectf(position.x - 1, position.y - 1, position.x + size.x + 1, position.y + size.y + 1);
	glColor3f(0.1137, 0.1137, 0.1137);
	glRectf(position.x, position.y, position.x + size.x, position.y + size.y);

	glPopMatrix();


	sf::Time elapsed = clock.getElapsedTime();
	if (elapsed.asMilliseconds() - lastAction.asMilliseconds() > 500){
		action = false;
	}
	
}

void GUIFrame::drawCubes()
{
    //Set the the OpenGL to handle 3D. This has to be done as this method is called after drawing the GUI, so the OpenGL was configured to draw 2D elements
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 5000.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //This is an attempt to make the 3D element be drawn inside the GUIFrame.
	//glTranslatef((position.x / (winSize.x / 2.0) * 5) + 1.3f, -(position.y / (winSize.y / 2.0) * 5) - 1.5f, -5);
	if (position.y > 0)
		glTranslatef((position.x / (winSize.x / 2.0) * 475) + 230 - maxMag, -(position.y  / (winSize.y / 2.0) * 475) - 40 - maxMag, -450 / (offset) );
	else
		glTranslatef((position.x / (winSize.x / 2.0) * size.x) + size.x/2 - maxMag, -(position.y / (winSize.y / 2.0) * 475) - 140, -450 / offset);
    
    //Rotate the Element every frame
	glRotatef(rotation / 3.f, 0.0f, 1.0f, 0.0f);
	rotation += 0.1;
    
    //Draw all cubes of the GUIFrame
	for (int i = 0; i < cubes.size(); i+=(1 + (cubes.size()/8000)))
	{
		if (i < cubes.size())
			cubes.at(i).draw();

	}

}

void GUIFrame::setCubes(std::vector<Cube> &cub)
{
	action = false;
	sf::Time elapsed = clock.getElapsedTime();
	if (elapsed.asMilliseconds() - lastAction.asMilliseconds() > 500){
		action = false;
	}
    
    //Used to set the position inside the GUIFrame.
	maxMag = 0;

	rotation = 0;
    
    //Check the maximum magnute of all position vectors inside the cub array. This is used to draw inside the GUIFrame
	for (int i = 0; i < cub.size(); i++)
	{
			sf::Vector3f cPos = cub.at(i).getPosition();
			float temp = sqrt((cPos.x * cPos.x) + (cPos.y * cPos.y) + (cPos.z * cPos.z));
			if (temp > maxMag)
				maxMag = temp;
			float size = pow(cub.size(), 1.0/20.0f);
			cub.at(i).setSize(size);
			cub.at(i).setPosition(cPos.x, cPos.y, cPos.z);
	}

	

	offset = pow(100 / maxMag, 1.0f / 1.7f);
	cubes = cub;

}



void GUIFrame::setWindowSize(int varX, int varY)
{
	winSize = sf::Vector2i(varX, varY);
}

//Return action. If is set to true, it will be set to false before returning true so it will only be triggered once.
bool GUIFrame::getAction()
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