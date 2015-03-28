#include "InputManager.h"


InputManager::InputManager(sf::Window* renderWindow)
{
	_escape = false;
	_keys = { false, false, false, false };

	_window = renderWindow;

    //Set the serial Port for the communication with the Arduino
	SP = new Serial("\\\\.\\COM4");
    
    //Data lenght to be read from serial
	dataLength = 512;
	readResult = 0;

    //Delay between each reading.
	scanTime = 880;
	readCount = -1;
	lastKeyPress = clock.getElapsedTime();
	updateTime = clock.getElapsedTime();
}


InputManager::~InputManager()
{


}


void InputManager::update(){
	delta = clock.getElapsedTime() - updateTime;
	updateTime = clock.getElapsedTime();
	
	
    //Set the curent window as active
	_window->setActive();
	//Handle Events:
	//Handle if user closes or resize window.
	//Handle if Escape key is pressed;
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			// end the program
			_exit = true;
		}
		else if (event.type == sf::Event::Resized)
		{
			// adjust the viewport when the window is resized
			glViewport(0, 0, event.size.width, event.size.height);
		}
		//Handle Keyboard presses
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				_escape = true;
			}
		}
        //Check if the mouse wheel changed
		else if (event.type == sf::Event::MouseWheelMoved)
		{
			int deltaW = event.mouseWheel.delta;
			if (wheelDelta > 0 && deltaW == -1)
			{
				wheelDelta = 0;
			}
			else if (wheelDelta < 0 && deltaW == 1)
			{
				wheelDelta = 0;
			}
			wheelDelta += event.mouseWheel.delta;
		}

	}


    //Increment the mouse wheel based on it's direction
	if (wheelDelta > 0)
	{
		wheelDelta -= 0.12;
	}
	else if (wheelDelta < 0)
	{
		wheelDelta += 0.12;
	}

	//We check the controlling inputs outside the event poll as we want to check if the key is not pressed too.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_keys.W = true;
		
		//_position.z += 0.4f;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		_keys.W = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_keys.S = true;
		//_position.z += 0.4f;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_keys.S = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_keys.A = true;
		//_position.z += 0.4f;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_keys.A = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_keys.D = true;
		//_position.z += 0.4f;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_keys.D = false;
	}
}

bool InputManager::getEscape()
{
	if (_escape)
	{
		_escape = false;
		return true;
	}
	return false;
}

keyTyped InputManager::getKeyTyped()
{
	return _keys;
}

keyTyped InputManager::getKeyPress()
{
	if (clock.getElapsedTime().asMilliseconds() - lastKeyPress.asMilliseconds() > 200){
		if (_keys.A == true || _keys.W == true || _keys.S == true || _keys.D == true)
			lastKeyPress = clock.getElapsedTime();
		return _keys;
	}
	else
	{
		keyTyped keys = { false, false, false, false };
		return keys;
	}
}


void InputManager::saveCubes(std::vector<Cube> &cubes, std::string fileName)
{
	std::ofstream myfile;
	myfile.open("rooms/" + fileName + ".txt");
	for (int i = 0; i < cubes.size(); i++)
	{
		myfile << cubes.at(i).getPosition().x << " " <<
			cubes.at(i).getPosition().y << " " <<
			cubes.at(i).getPosition().z << " " <<
			cubes.at(i).getSize() << " " <<
			int(cubes.at(i).getColor().r) << " " <<
			int(cubes.at(i).getColor().g) << " " <<
			int(cubes.at(i).getColor().b) << std::endl;

	}
	myfile.close();
}

void InputManager::loadCubes(std::vector<Cube> *cubes, std::string fileName)
{
	cubes->clear();
	std::string line;
	std::ifstream myfile;
	myfile.open("rooms/" + fileName);
	std::string x, y, z;
	if (myfile.is_open())
	{
		int i = 0;
		Cube* temp;

		while (myfile >> x) {
			myfile >> y;
			myfile >> z;
			sf::Vector3f position(std::stof(x), std::stof(y), std::stof(z));
			

			myfile >> x;
			float size = std::stof(x);

			myfile >> x;
			myfile >> y;
			myfile >> z;
			sf::Color color(std::stoi(x), std::stoi(y), std::stoi(z));
			
			Cube temp(position, 1.8f, color);

			cubes->push_back(temp);
		}
		myfile.close();
	}
}

bool InputManager::getExit()
{
	return _exit;
}

int InputManager::getWheelData()
{
	return wheelDelta;
}

void InputManager::readSerial(std::vector<Cube>* cubes)
{
    //Check if Serial Port is connected.
	if (SP->IsConnected() && clock.getElapsedTime().asMilliseconds() - readDelay.asMilliseconds() > scanTime){
        //If is connected and the delay time has been passed, data will be read
		std::cout << "Connected and reading" << std::endl;
		readDelay = clock.getElapsedTime();
		parseSerial();
	}
    
    //As the software read the data from the serial in blocks, it wouldn't display the readings in a smooth way
    //We use a queue to store the cubes that were read but yet not displayed in the screen. After a small delay we take one element out of
    //the queue and add to the main cubes array so it can be displayed on screen. This will give an impression of a continous reading
	if (buffer.size() > 0 && clock.getElapsedTime().asMilliseconds() - displayDelay.asMilliseconds() > scanTime / readCount && readCount > 0 && SP->IsConnected())
	{
		displayDelay = clock.getElapsedTime();
		cubes->push_back(buffer.front());
		buffer.pop();
	}
	else if (readCount <= 0 && buffer.size() > 0)
	{
		readCount = buffer.size();
		displayDelay = clock.getElapsedTime();
		cubes->push_back(buffer.front());
		buffer.pop();
	}
    
    //In case there is any issue and there is too many cubes in the queue, add all to the main cubes array
	if (buffer.size()> 100)
	{
		while (!buffer.empty())
		{
			cubes->push_back(buffer.front());
			buffer.pop();
		}
	}

}

bool InputManager::parseSerial()
{
	char incomingData[512] = "";
	//Reading from Serial
	readResult = SP->ReadData(incomingData, dataLength);
	printf("Bytes read: (-1 means no data available) %i\n", readResult);
	if (readResult == -1)
		return false;


    
	std::cout << readCount << std::endl;
	std::string azimuthalAngleString = "";
	std::string polarAngleString = "";
	std::string valueString = "";
    
    //Iterate throught all the string read from the Serial. As we are reading in chunks, it is not guarateed that the lines will not be broken and that the start of the string is actually
    //the first data to be read
	for (int i = 0; i < dataLength; i++)
	{
        //If char is the header of the theta (azimuthal) angle
		if (incomingData[i] == 'T')
		{
            //As there is no guarantee that the string will be complete, we must check if there will be at least 3 more characters to be parsed into the correct value
			if (i < dataLength - 4)
			{
				azimuthalAngleString.clear();
				azimuthalAngleString.push_back(incomingData[(i + 1) % dataLength]);
				azimuthalAngleString.push_back(incomingData[(i + 2) % dataLength]);
				azimuthalAngleString.push_back(incomingData[(i + 3) % dataLength]);
			}
		}
        //If char is the header of the Polar angle
		if (incomingData[i] == 'P')
		{
			if (i < dataLength - 4)
			{
				polarAngleString.clear();
				polarAngleString.push_back(incomingData[(i + 1) % dataLength]);
				polarAngleString.push_back(incomingData[(i + 2) % dataLength]);
				polarAngleString.push_back(incomingData[(i + 3) % dataLength]);
			}
		}
        //If the char is the header of the distance Value..
		if (incomingData[i] == 'V')
		{
			if (i < dataLength - 5)
			{

				valueString.clear();
				valueString.push_back(incomingData[(i + 1) % dataLength]);
				valueString.push_back(incomingData[(i + 2) % dataLength]);
				valueString.push_back(incomingData[(i + 3) % dataLength]);
				valueString.push_back(incomingData[(i + 4) % dataLength]);

                //After reading the distance value and the other two values are complete. We can assume that we have a correct reading
				if (azimuthalAngleString.length() == 3 && polarAngleString.length() == 3 && valueString.length() == 4)
				{
                    //As we are reading from the serial, there might be so trash in the reading, so we check if every value is actually a number
					if (is_number(azimuthalAngleString) && is_number(polarAngleString) && is_number(valueString))
					{
                        
						float distanceValue = std::stoi(valueString)*2.0f;
                        //As the coordinate system from our servos is inverted compared to the Spherical coordinate system, we change it before displayed it in the screen.
						int azimuthalAngleValue = std::stoi(polarAngleString) + 90;
						int polarAngleValue = std::stoi(azimuthalAngleString);
						

						std::cout << azimuthalAngleValue << " " << polarAngleValue << " " << distanceValue << std::endl;
                        //Convert from Spherical Coordinate system to XYZ
						sf::Vector3f posVec = sf::Vector3f(distanceValue* sin((azimuthalAngleValue)* PI / 180.0) * cos((polarAngleValue)* PI / 180.0), distanceValue* sin((azimuthalAngleValue)* PI / 180.0) * sin((polarAngleValue)* PI / 180.0), distanceValue* cos((azimuthalAngleValue)* PI / 180.0));
						//sf::Color colour = sf::Color(rand() % 205 + 50, rand() % 205 + 50, rand() % 205 + 50);
                        
                        //Set the cube color based on the angles and distance value
						sf::Color colour = sf::Color(azimuthalAngleValue - 20, polarAngleValue + 70, (distanceValue / 400.0f) * 255.0f);
						
						Cube cube(posVec, 1.8f, colour);
						buffer.push(cube);
					}


				}

			}
		}
	}

	readCount = buffer.size();

}

//Function from StackOverflow, check .h for reference.
bool InputManager::is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

void InputManager::writeSerial(char* buf, int siz)
{
	SP->WriteData(buf, siz);

}

float InputManager::getDeltaTime(){
	return delta.asSeconds();
}