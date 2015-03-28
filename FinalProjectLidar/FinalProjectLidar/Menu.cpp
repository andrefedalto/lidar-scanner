#include "Menu.h"


Menu::Menu(InputManager* in, sf::RenderWindow* win, std::vector<Cube> *cub)
{
	int sizeX = win->getSize().x;
	int sizeY = win->getSize().y;
	gui.setWindow(win);
	window = win;

	//Create all the GUI elements
	GUIRect* topbar = new GUIRect(sf::Vector2f( -sizeX / 2.0 - 15, - sizeY / 2), sf::Vector2f(sizeX/2.0f - 85, 100));
	topbar->setBackgroundColor(sf::Color(107, 185, 240));
	gui.add(topbar);
	gui.show(topbar);
	

	GUIRect* leftbar = new GUIRect(sf::Vector2f(-sizeX / 2.0 - 15, 100 - sizeY / 2), sf::Vector2f(sizeX / 2.0f - 85, sizeY));
	leftbar->setBackgroundColor(sf::Color(44, 62, 80));
	gui.add(leftbar);
	gui.show(leftbar);

	int border = 75;
	int borderTop = 25;


	newCapture = new GUIButton(sf::Vector2f(border - sizeX / 2.0, borderTop + 200 - sizeY / 2), sf::Vector2f(250, 75));
	newCapture->setText("New Scan");
	gui.add(newCapture);

	load = new GUIButton(sf::Vector2f(border - sizeX / 2.0, borderTop + 300 - sizeY / 2), sf::Vector2f(250, 75));
	load->setText("Load from File");
	gui.add(load);

	save = new GUIButton(sf::Vector2f(border - sizeX / 2.0, borderTop + 400 - sizeY / 2), sf::Vector2f(250, 75));
	save->setText("Save file");
	gui.add(save);

	exit = new GUIButton(sf::Vector2f(border - sizeX / 2.0, borderTop + 500 - sizeY / 2), sf::Vector2f(250, 75));
	exit->setText("Exit Menu");
	gui.add(exit);

	back = new GUIButton(sf::Vector2f(border - sizeX / 2.0, borderTop + 100 - sizeY / 2), sf::Vector2f(250, 75));
	back->setText("Return");
	gui.add(back);
	gui.hide(back);

	down = new GUIButton(sf::Vector2f((border + 275) - sizeX / 2.0, borderTop + 675 - sizeY / 2), sf::Vector2f(50, 50));
	down->setText("V");
	gui.add(down);
	gui.hide(down);

	hide = new GUIButton(sf::Vector2f((border) - sizeX / 2.0, borderTop + 675 - sizeY / 2), sf::Vector2f(50, 50));
	hide->setText("<");
	gui.add(hide);

	input = in;
	running = true;

	//Create the GUI frames to displayed saved files
	GUIFrame* frame = new GUIFrame(sf::Vector2f(15 - sizeX / 2.0, borderTop + 200 - sizeY / 2), sf::Vector2f(sizeX / 2.0f - 140, 250));
	frame->setWindowSize(sizeX, sizeY);
	frames.push_back(frame);

	GUIFrame* frame2 = new GUIFrame(sf::Vector2f(border - sizeX / 2.0, borderTop + 475 - sizeY / 2), sf::Vector2f(250, 250));
	frame2->setWindowSize(sizeX, sizeY);
	frames.push_back(frame2);
	
	gui.add(frames.at(0));
	gui.add(frames.at(1));
	gui.hide(frames.at(0));
	gui.hide(frames.at(1));

	windowSize = sf::Vector2i(sizeX, sizeY);

	//Get current saved files
	files = getFileNames();
	curFile = 0;


	cubes = cub;


	//As the title has a different and unique style, it was easier to create it outside the GUI.
	font.loadFromFile("OpenSans-Light.ttf");
	// Create a text
	sf::Text text("Lidar Scanner", font);
	title = text;
	title.setPosition(sf::Vector2f(15, 15));
	title.setCharacterSize(46);
	title.setColor(sf::Color(34, 49, 63));

	//Direction of the Animation moviment
	dir = 1;
	//Offset of the menu (used for hiding and showing)
	animationOffset = 0;
	
}


Menu::~Menu()
{
}

int Menu::update(){
	gui.update();
	//Check mouse events for the Menu.
	//The Inputs class is not needed as the mouse wrapper is pretty decent.
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//Check if each button was clicked
		if (exit->getAction())
		{
			running = false;
		}
		else if (newCapture->getAction())
		{
			input->writeSerial("start", 5);
			return 1;
		}
		else if (save->getAction())
		{
			//Save current cube view to a file
			std::cout << std::to_string(files.size()) << std::endl;
			input->saveCubes(*cubes, std::to_string(files.size() + 1));
			//Reload file names
			files = getFileNames();
		}
		else if (load->getAction())
		{
			curFile = 0;
			//Hide all menu options except the ones used to show the files to be laoded
			hideOption();
			
			//If there is any file to be loaded
			if (files.size() > 0)
			{
				//Set the frame's cubes array to the array read from file
				std::vector<Cube> cube;
				input->loadCubes(&cube, files.at(curFile));
				frames.at(0)->setCubes(cube);
				gui.show(frames.at(0));
				

				if (files.size() > 1)
				{
					input->loadCubes(&cube, files.at(curFile+1));
					frames.at(1)->setCubes(cube);
					gui.show(frames.at(1));
				}
			}
			
		}
		else if (back->getAction())
		{
			//If the back button is pressed (from the load screen)
			showOptions();
		}
		else if (down->getAction())
		{
			/*Check if there are more files to be loaded, if so, change the frame's cubes to the new file.
			This is pretty stupid way of coding as we are reading the files all over again, even if they
			were already loaded in the memory. But as our software does not have issues with small delay when
			loading files, this was acceptable.*/
			if (files.size() > 2){
				curFile++;
				curFile = curFile % files.size();
				std::vector<Cube> cube;
				input->loadCubes(&cube, files.at(curFile));
				frames.at(0)->setCubes(cube);
				gui.show(frames.at(0));


				if (files.size() > 1)
				{
					input->loadCubes(&cube, files.at((curFile + 1) % files.size()));
					frames.at(1)->setCubes(cube);
					gui.show(frames.at(1));
				}
			}
		} 
		//If user asks to show/hide menu and menu is not being currently animated
		else if ((hide->getAction() && animation == false))
		{
			animation = true;
			//Change the direction of moviment
			dir = dir*(-1);
			clock.restart();
			hideTime = clock.getElapsedTime();
		}
	}
	if (input->getEscape())
	{
		running = false;
	}
	//If menu is being currently animated and is in the desired range
	if (animation && animationOffset > -windowSize.x/2.0f + 175 && animationOffset <= 0)
	{
		hideTime = clock.getElapsedTime();
		//Change the offset depending on the time to create a smooth animation
		animationOffset += cbrt(hideTime.asMicroseconds())* dir * input->getDeltaTime()*4;
		//Set the whole GUI's position relative to the offset
		gui.setPosition(sf::Vector2f(animationOffset, 0));

		//Change the hide button to still be showing
		hide->setPosition(sf::Vector2f(hide->getRect().left - animationOffset*0.8f, 700 - windowSize.y / 2.0f));
		//As the title is not in the GUI, change it's position
		title.setPosition(sf::Vector2f(15 + animationOffset, title.getPosition().y));
	}
	else if (animation)
	{
		//If it is outside the desired range and still animating. Stop animating;
		if (animationOffset < 0)
		{
			animationOffset = -windowSize.x / 2.0f + 176;
			hide->setText(">");
		}
		else{
			animationOffset = 0;
			hide->setText("<");
		}
		animation = false;
	}


	for (int i = 0; i < frames.size(); i++)
	{
		if (frames.at(i)->getAction())
		{
			input->loadCubes(cubes, files.at((curFile + i)%files.size()));

		}
	}

	//OK
	return 200;
}

void Menu::draw(){

	
	//This is done just to make the same coordinate system the same as OpenGL
	glTranslatef(windowSize.x / 2.0f, windowSize.y / 2.0f, 0);
	gui.draw(windowSize);
    
    //To avoid problems between 2D and 3D, the cubes inside the frames are drawn outside the normal GUI Loop
	for (int i = 0; i < frames.size(); i++)
	{
		if (!frames.at(i)->getHidden())
		{
			frames.at(i)->drawCubes();
		}
	}
    
    //Saves OpenGL States
	window->pushGLStates();
    //Draw TitleText
	window->draw(title);
    //Restore OpenGL States
	window->popGLStates();
}

void Menu::setRunning(bool var)
{
	running = var;
}

bool Menu::getRunning()
{
	return running;
}

//Function from the web, check .h for reference
//This function get file names from a specified folder.
//We adapted the function to ignore dirs and extra data
std::vector<std::string> Menu::getFileNames()
{
	std::vector<std::string> files;
	tinydir_dir dir;
	tinydir_open(&dir, "rooms/");

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		std::string name = file.name;
		if (name.at(name.size() - 1) == 't')
			files.push_back(file.name);
		if (file.is_dir)
		{
			//is dir
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
	return files;
}

//Hide the options from Menu
void Menu::hideOption()
{
	gui.hide(newCapture);
	gui.hide(load);
	gui.hide(save);
	gui.hide(exit);
	gui.show(back);
	gui.show(down);
	gui.hide(hide);
}

//Show Options from menu
void Menu::showOptions()
{
	gui.hide(frames.at(0));
	gui.hide(frames.at(1));
	gui.hide(down);
	gui.hide(back);
	gui.show(newCapture);
	gui.show(load);
	gui.show(save);
	gui.show(exit);
	gui.show(hide);

	
}