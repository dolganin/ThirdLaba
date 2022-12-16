#pragma once
#include "BaseConvert.h"

class Mix :public Base {
public:
	std::string name = "mix";
	int16_t* ProcessWav(args& a);
};