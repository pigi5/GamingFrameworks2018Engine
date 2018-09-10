#include "../header/Audio.h"

Audio::Audio()
{
}

int Audio::loadSound(string fileName) {
	if (!this->buffer.loadFromFile(fileName))
	{
		return -1;
	}
	return 0;
}

void Audio::playSound() {
	this->sound.setBuffer(buffer);

	this->sound.play();

}