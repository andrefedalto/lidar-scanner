#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC


#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include "WindowManager.h"
#include "InputManager.h"
#include "Menu.h"

enum runStates { RUNNING, MENU, EXIT, SCAN };

int main()
{
	
	//Create a new window. Can be used both with OpenGL and SFML
	sf::RenderWindow window(sf::VideoMode(1000, 800, 32), "Lidar OpenGL");

	//Enable VerticalSync
	window.setVerticalSyncEnabled(true);

	InputManager input(&window);

	//Create a vector of cubes to be drawn on the main panel of the window
	std::vector<Cube> cubes;

	//Create an objec manager
	WindowManager manager(&window, &input, &cubes);

	

	//Call the setup, mostly for OpenGL and drawing the initial view
	manager.setup();

	//Create a menu
	Menu menu(&input, &window, &cubes);

	//Initial "runningState" set to Menu
	runStates state = MENU;
	manager.setOnMenu(true);
	menu.setRunning(true);


	// run the main loop
	bool running = true;
	
	//Code used to check Menu Actions
	int code = 200;

	//Set scanning mode to false
	bool scanning = false;

	while (running)
	{
		//Update objets
		manager.update();
		input.update();
		//Clear sceen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		switch (state) {
			case RUNNING: 
				//Returning to Menu
				if (!manager.running())
				{
					manager.setOnMenu(true);
					state = MENU;
					menu.setRunning(true);
					break;
				}
				//Check if scanning
				if (scanning)
					input.readSerial(&cubes);


				//Draw all 3D objects
				manager.draw3D();
				break;
			case MENU: 
				//Check if user close the Menu
				if (!menu.getRunning())
				{
					manager.setOnMenu(false);
					state = RUNNING;
					break;
				}
				if (scanning)
					input.readSerial(&cubes);

				//Get the code from Menu to check user action
				code = menu.update();

				//Code 200: OK
				if (code == 200){
					manager.draw3D();
					manager.draw2D();

					menu.draw();			
				}
				//Code 1: Scan
				else if (code == 1)
				{
					cubes.clear();
					scanning = true;
				}
				break;
			case EXIT: 
				break;
		}
		if (input.getExit())
		{
			running = false;
		}


		//Swap buffers and display the backbuffer on the screen
		window.display();


	}



	return 0;
}