#pragma once

#include "Audio.h"

class Music : Audio
{
private:
	sf::Music music;

public:
	Music();
	Music(string fileName);
	~Music();

	int playMusic();
	void stopMusic();


};