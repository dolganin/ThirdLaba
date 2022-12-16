#include "Mute.hpp"

int16_t* Mute::ProcessWav(args& a)
{
	if (a.argue.size() != 3) throw ("Too many arguments");
	int16_t* main = a.main.stream;
	int beg = std::stol(a.argue[1])*freq;
	int end = std::stol(a.argue[2])*freq;
	if (beg > end) throw("\nArgument begin in config-file greater than end\n");
	for (; beg != end; beg++) {
		main[beg] = 0;
	}
	return main;
}
