#include "Turn up.hpp"

int16_t* TurnUp::ProcessWav(args& a) {
	if (a.argue.size() != 3) throw ("Too many arguments");
	int16_t* main = a.main.stream;
	int beg = std::stol(a.argue[1]) * freq;
	int end = std::stol(a.argue[2]) * freq;
	if (beg > end) throw ("Argument begin in config-file smaller than end");
	for (; beg != end; beg++) {
		main[beg] +=13245;
	}
	return a.main.stream;
}