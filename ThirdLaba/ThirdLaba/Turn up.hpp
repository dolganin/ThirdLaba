#pragma once

#include "BaseConvert.h"

class TurnUp :public Base {
public:
	std::string name = "turnup";
	int16_t* ProcessWav(args& a);
};