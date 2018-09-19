#include "../header/Music.h"

Music::Music()
{

}

Music::Music(string fileName)
{
	this->fileName = fileName;
}

Music::~Music()
{

}

int Music::playMusic()
{
	if (!this->music.openFromFile(this->fileName))
	{
		return -1;
	}

	this->music.play();
	return 0;
}

void Music::stopMusic()
{
	this->music.stop();
}