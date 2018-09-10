#pragma once

#include <SFML/Audio.hpp>

using namespace std;

class Audio
{
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;

public:
	Audio();

	int loadSound(string fileName);
	void playSound();
};