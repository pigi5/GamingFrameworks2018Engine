#pragma once

#include <SFML\Audio.hpp>

using namespace std;

class Audio
{
protected:
	string fileName;

public:
	Audio();
	Audio(string fileName);
	~Audio();
};