#pragma once

#include "Audio.h"

class Music : public Audio
{
private:
	sf::Music music;

public:
	Music();
	Music(Audio* audio);
	Music(std::string fileName);
	~Music();

	int playMusic();
	void stopMusic();

	std::string getFileName();

	Music& operator=(Music other);


};