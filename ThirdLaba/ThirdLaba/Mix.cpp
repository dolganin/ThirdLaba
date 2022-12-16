#include "Mix.hpp"


int16_t* Mix::ProcessWav(args& a) {
	int beg = std::stoi(a.argue[2])*freq;
	_stream add;
	add.stream = a.add[a.argue[1].back() - 49]->stream;
	add.size = a.add[a.argue[1].back() - 49]->size;

	if (beg < 0 || beg>a.main.size) throw("Argument begin is out of range");
	for (size_t i = beg; i < a.main.size; i++) {
		if (i == add.size) break;
		a.main.stream[i] = (a.main.stream[i] + add.stream[i]) / 2;
	}
	return a.main.stream;
}

