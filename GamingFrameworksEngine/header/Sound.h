#pragma once

#include "Audio.h"

class Sound : public Audio
{
private:

	sf::Sound sound;
	sf::SoundBuffer buffer;

public:

	Sound();
	Sound(std::string fileName);
	Sound(Audio* audio);
	~Sound();

	bool loadSound();
	void playSound();

};
