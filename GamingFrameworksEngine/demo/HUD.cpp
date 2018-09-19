#include "HUD.h"

HUD::HUD(State startState) : Overlay(startState)
{
	this->xSize = 50;
	this->ySize = 50;
	this->sprite = new Sprite();
}