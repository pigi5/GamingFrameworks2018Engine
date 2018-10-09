#pragma once

#include "Audio.h"

class Music : Audio
{
private:
	sf::Music music;

public:
	Music();
	Music(std::string fileName);
	~Music();

	int playMusic();
	void stopMusic();

	std::string getFileName();

	Music& operator=(Music other);


};