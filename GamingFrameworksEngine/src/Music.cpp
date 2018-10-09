#include "../header/Music.h"


Music::Music()
{

}

Music::Music(std::string fileName)
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

std::string Music::getFileName() {
	return this->fileName;
}

Music& Music::operator=(Music other) {
	return other;
}