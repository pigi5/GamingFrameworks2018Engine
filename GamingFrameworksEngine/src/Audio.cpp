#include "../header/Audio.h"

//basic constructor
Audio::Audio()
{
}

//constructor that takes the name of an audio file
Audio::Audio(string fileName) {
	this->fileName = fileName;
}

//destructor
Audio::~Audio() 
{
}