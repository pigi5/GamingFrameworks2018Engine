#pragma once

#include "Audio.h"

class Sound : Audio
{
private:
	sf::Sound sound;
	sf::SoundBuffer buffer;

public:

	Sound();
	Sound(string fileName);
	~Sound();

	bool loadSound();
	void playSound();

};
