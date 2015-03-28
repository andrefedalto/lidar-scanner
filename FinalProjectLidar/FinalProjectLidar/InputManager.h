#pragma once
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <queue>

#include "Cube.h"

//Credits to http://playground.arduino.cc/Interfacing/CPPWindows for this class
#include "Serial.h"

#define PI 3.14159265

typedef struct keyTyped{
	bool W;
	bool S;
	bool D;
	bool A;
}keyTyped;

class InputManager
{
private:
	bool _escape;
	bool _exit;
	sf::Clock clock;
	sf::Time lastKeyPress;
	sf::Time delta;
	sf::Time updateTime;
	
	sf::Window* _window;
	keyTyped _keys;                     //WSDA values
	float wheelDelta;                   //Wheel delta

	bool scanning;
	int scanTime;
	int readCount;
	sf::Time readDelay;
	sf::Time displayDelay;
	std::queue<Cube> buffer;

	Serial* SP;							//Serial IO Class
	int dataLength;
	int readResult;




public:
	InputManager(sf::Window*);
	~InputManager();

	void update();
	bool getEscape();                   //Return if Escape key is pressed
	bool getExit();                     //Return if exit window button is clicked
	keyTyped getKeyTyped();             //Get if a key is typed
	keyTyped getKeyPress();             //Return a single press from a key

	void saveCubes(std::vector<Cube>&, std::string);        //Save an array of cubes into file
	void loadCubes(std::vector<Cube>*, std::string);        //Loads a file into an array of cubes

	void setScanning(bool);              //Set the scanning mode true or false
	bool parseSerial();                  //Parse data from Serial
	void readSerial(std::vector<Cube>*); //Load data from serial into cubes array
	void sendSerial(char);

	int getWheelData();                 //Return Wheel ticks
    bool is_number(const std::string&); //Credits to http://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c for this function:

    
	void writeSerial(char*, int);       //Write into serial port

	float getDeltaTime();
	
};

