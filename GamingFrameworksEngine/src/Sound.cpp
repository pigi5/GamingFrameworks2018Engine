#include "../header/Sound.h"

Sound::Sound()
{

}

Sound::Sound(std::string fileName)
{
	this->fileName = fileName;
}

Sound::Sound(Audio* audio)
{
	this->fileName = audio->fileName;
	this->name = audio->name;
}

Sound::~Sound()
{

}

//loads sound from given filename
bool Sound::loadSound()
{
	if (!this->buffer.loadFromFile(this->fileName))
	{
		return false;
	}
	return true;
}

//plays sound
void Sound::playSound()
{
	this->sound.setBuffer(buffer);

	this->sound.play();

}