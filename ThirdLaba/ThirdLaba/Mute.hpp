#pragma once
#include "BaseConvert.h"

class Mute : public Base {
public:
	int16_t* ProcessWav(args& a);
	std::string name = "mute";
};