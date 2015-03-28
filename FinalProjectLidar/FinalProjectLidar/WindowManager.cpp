#include "WindowManager.h"

#include "Cube.h"
#include "WindowManager.h"

WindowManager::WindowManager(sf::Window* window, InputManager* input, std::vector<Cube> *cub)
{
	_window = window;
	inputs = input;
	cubes = cub;
	autoRotate = true;
}


WindowManager::~WindowManager()
{

}

void WindowManager::setup()
{
	//Initializing OpenGL
	// Set color and depth clear value
	glClearDepth(1.f);
	glClearColor(0.1412f, 0.1412f, 0.1412f, 0.f);
	//Initializing the rotation of the Scene
	//This makes it centered to when the mouse is in the middle of the screen;
	rotation = sf::Vector2f((_window->getSize().x) / 2 + _window->getPosition().x, (_window->getSize().y) / 2 + _window->getPosition().y);

	//Camera Position
	_position = sf::Vector3f(-50, 0, -150);

	//Set state to running
	_running = true;

	//Creating cube
	float radius = 100;
	int angle = 30;
    
    //Add some cubes to the initial view. This will show when the software first start
	for (int i = 0; i < 360; i+=10)
	{

		for (int j = 0; j < 180; j+=10)
		{
            //Use the equation of the Spherical Coordinate system to display a sphere
			sf::Vector3f posVec = sf::Vector3f(radius* sin((i)* PI / 180.0) * cos((j)* PI / 180.0), radius* sin((i)* PI / 180.0) * sin((j)* PI / 180.0), radius* cos((i)* PI / 180.0));
			float cubeSize = 1.0f;

			sf::Color colour = sf::Color(rand() % 205 + 50, rand() % 205 + 50, rand() % 205 + 50);
			Cube cube(posVec, cubeSize, colour);
			cubes->push_back(cube);
		}
	}
	//inputs->loadCubes(cubes, "1");

}

void WindowManager::newCapture()
{
    //Function used for testing. This creates a random pattern on the screen.
	cubes->clear();

	for (int i = 0; i < 180; i++)
	{
		for (int j = 0; j < 180; j++)
		{
			sf::Vector3f posVec = sf::Vector3f(rand() % 100 * sin((i * 12)* PI / 180.0) * cos((j * 12)* PI / 180.0), rand() % 100 * sin((i * 12)* PI / 180.0) * sin((j * 12)* PI / 180.0), rand() % 100 * cos((i * 12)* PI / 180.0));
			float cubeSize = 5.f;

			sf::Color colour = sf::Color(rand() % 205 + 50, rand() % 205 + 50, rand() % 205 + 50);
			Cube cube(posVec, cubeSize, colour);
			cubes->push_back(cube);
		}
	}
}


/*
 Initially we didn't have a menu, instead we had a First person view of the scene where you could move around with WASD and the mouse.
 When we added the menu we kept both of the views, so we have a condition that checks if the user is viewing using the menu or not, and the inputs are handled differently
 */
void WindowManager::update()
{

	sf::Vector2i temp = sf::Mouse::getPosition();
    //Get the difference between the last Mouse position and the actual mouse position to move the view
	mouseDelta = (sf::Vector2f)temp - (sf::Vector2f)mousePos;
	mousePos = temp;
    
    //Check if the Menu is being displayed
	if (onMenu)
	{
		
        
		rotation.x += 0.5;
        
        //Zoom in and out with the mousewheel
		if (_position.z > -1000 && _position.z < 100)
		{ 
			_position.z += inputs->getWheelData();
		}
		else if (_position.z <= -1000)
		{
			_position.z = -999;
		}
		else if (_position.z >= 100)
		{
			_position.z = 99;
		}
        
        //Move left/right up/down when pressing the left mouse button and moving the mouse
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			_position.x += mouseDelta.x/1.5f;
			_position.y -= mouseDelta.y/1.5f;

		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			rotation.y += mouseDelta.x;
		}

        //Stop/Start the auto rotation whe pressing W
		if (inputs->getKeyPress().W)
		{
			autoRotate = !autoRotate;
		}

		if (autoRotate)
		{
			rotation.y += inputs->getDeltaTime()*5;
		}

		

	}
	else{
		handleInput();
		//Updating the deltaMouse
		

		//Setting Scene Rotation based on mouse movement:


		rotation += mouseDelta;
	}



}

void WindowManager::draw3D()
{
	
    //Enable the OpenGL configs to draw in 3D
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 2000.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// get global mouse position
	//Rotating the whole scene

	if (onMenu)
	{ 
		
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(rotation.y / 3.f, 0.0f, 1.0f, 0.0f);
		glRotatef(90, 1.0f, 0.0f, 0.0f);
		
		//glRotatef(rotation.x / 3.f, 0.0f, 1.0f, 0.0f);
		//glRotatef(rotation.x / 3.f, 0.0f, 1.0f, 0.0f);
	}
	else{
		
		glRotatef(rotation.x / 3.f, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.y / 3.f, 1.0f, 0.0f, 0.0f);
		
		



		glTranslatef(_position.x, _position.y-50, _position.z);
		glRotatef(90, 1.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < cubes->size(); i++)
	{
		cubes->at(i).draw();
	}

	//drawTriangles();
}

void WindowManager::draw2D(){
    //Enable the OpenGL for 2D Drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0f, _window->getSize().x, _window->getSize().y, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0);

	glDisable(GL_DEPTH_TEST);

	/*glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(100, 100, 0);
	glVertex3f(300, 100, 0);
	glVertex3f(100, 300, 0);
	glEnd();*/
}

sf::Vector2f WindowManager::getMouseDelta()
{
	return mouseDelta;
}

bool WindowManager::running()
{
	if (inputs->getEscape())
	{
		return false;
	}
	return true;
}

void WindowManager::setOnMenu(bool var)
{
	if (var == false)
	{
		_position = savedPostion;
		mousePos = sf::Mouse::getPosition();
	}
	else
	{
		rotation.y = 0;
		savedPostion = _position;
		_position = sf::Vector3f(85, 0, -250);
	}
	onMenu = var;
}

void WindowManager::handleInput()
{
	if (inputs->getKeyTyped().W)
	{
		_position.z += inputs->getDeltaTime() *8;
	}
	if (inputs->getKeyTyped().S)
	{
		_position.z -= inputs->getDeltaTime() * 8;
	}
	if (inputs->getKeyTyped().A)
	{
		_position.x += inputs->getDeltaTime() * 8;
	}
	if (inputs->getKeyTyped().D)
	{
		_position.x -= inputs->getDeltaTime() * 8;
	}
	if (inputs->getEscape())
	{
		_running = false;
	}

}
