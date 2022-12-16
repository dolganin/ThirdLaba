#define _CRT_SECURE_NO_WANINGS

//#include <Windows.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <limits.h>
#include "BaseConvert.h"
#include "Mute.hpp"
#include "Mix.hpp"
#include "Turn up.hpp"
#include <string>

using namespace std;

class Message {
public:
	//references
	std::string startWork = "\nSound proccesor v.1.0.0\n";
	std::string analYsis = "\nAnalysing files...\n";
	std::string help = "\n	This is my implemention of simple sound proccessor.\n	There you can edit ur sound files in WAV-format.\n\	Working ONLY for wav-files\n\n Errors codes:\n\ 0 - Unknown Command in Arguments\n\ 1 - Config file isn't existing or can't be opened\n\ 2 - Not enough arguments in command line\n\ 3 - Program can't find name of config file in arguments\n\ 5 - Unkown plugin you are used";
	//errors
	std::string error0 = "\nUnknown argument\n";
	std::string error1 = "\nThere is no config file - program can't be executed\n";
	std::string error2 = "\nNot enough arguments.\n";
	std::string error3 = "\nName of config file isn't marked - use \"-c\" for marking\n";
	std::string error5 = "\nUnsupported format wav-file\n";
	//plugins
	std::string unknown = "\nUnknown plugin are used\n";
};

class InputParser: public Message{
public:
	InputParser() = default;
	~InputParser() = default;
	std::vector<std::vector<std::string>> Parse(std::string& name) {
		cout << "\nParsing config file...\n";
		ifstream in (name);
		if (!in) {
			cerr << Message::error1;
			exit(1);
		}
		std::string temp;
		std::vector<std::vector<std::string>> parsLine;
		std::vector<std::string> tempvect;
		while(!in.eof()) {
			std::getline(in, temp);
			if (temp[0] == '#'|| temp.empty()==true) continue;
			int start= 0, end=0;
			while ((start = temp.find_first_not_of(' ',end)) != std::string::npos){
				end = temp.find(' ', start);
				tempvect.push_back(temp.substr(start, end - start));
			}
			parsLine.push_back(tempvect);
			tempvect.clear();
		}
		return parsLine;

	}
};

class wavRead{
public:
	wavRead() = default;
	~wavRead() = default;
	bool noFile = false;
	ifstream& readWav(std::string& name) {
		ifstream in;
		in.open(name.c_str(),std::ios_base::binary);
		if (!in) {
			noFile = true;
		}
		return in;
	}

};

class wavParse: public wavRead, public Message{
public:
	wavParse() = default;
	~wavParse() = default;
	WAVFile parseWav(std::string s) {
		WAVFile file;
		ifstream in(s, std::ios_base::binary);
		if (!in.is_open())throw std::invalid_argument("\nThere is no file with such name\n");
		else {
			cout << "\nFile "+s+" was opened\n";

			int size = 0;
			in.seekg(0, std::ios::end);
			size = in.tellg();
			in.seekg(0);
			
			in.read(reinterpret_cast<char*>(&file.head), sizeof(WAVHeader));
			int trash = 0;
			int chunkid, chunksize;
			if (file.head.numchannels != 1 || file.head.sampleRate != 44100 || file.head.bitsPerSample != 16) {
				cerr << Message::error5;
				exit(5);
			}
			while (!in.eof()) {

				in.read(reinterpret_cast<char*>(&chunkid), sizeof(int));
				in.read(reinterpret_cast<char*>(&chunksize), sizeof(int));

				if (chunkid == 1635017060) {
					file.head.subchank2Id = chunkid;
					file.head.subchank2Size = chunksize;
					break;
				}
				if (chunksize == 1635017060) {
					in.read(reinterpret_cast<char*>(&file.head.subchank2Size), sizeof(int32_t));
					file.head.subchank2Id = chunksize;
					break;
				}
				trash += sizeof(int) * 2;
			}

			file.head.subchank2Size = size;
			unsigned long long int  dataSize = long long int (size) - sizeof(WAVHeader) - trash;
			
			file.data = new int16_t[dataSize];
			in.read(reinterpret_cast<char*>(file.data), dataSize);
			in.close();

			
		}
		return file;
	}
};

class WAV:public wavParse {
public:
	//ifstream& flow;
	WAVFile file;

	WAV(std::string name) 
	{
		this->file = parseWav(name);
	}
	~WAV() = default;
};

int main(int argc, char** argv) {

	Message console;
	cout << console.startWork;
	cout << console.analYsis;
	std::string config;
	std::string output;
	std::vector<std::string> input;
	InputParser file;

	size_t pos = 0;

	switch (argc) {
	case 1:
		cerr << console.error0;
		exit(0);
		break;
	case 2:
		if (strcmp(argv[1],"-h")==0) {
			cout << console.help;
			exit(-1);
			break;
		}
		else  cerr << console.error0;
		break;
	case 3:
		cerr << console.error2;
		exit(2);
		break;
	default:
		try {
			for (int i = 0; i < argc; i++) {
				if (strcmp(argv[i], "-c") == 0) {
					config = std::string(argv[i + 1]);
					pos = i + 2;
					break;
				}
				else if (argv[i] != "-c" && i == argc - 1) throw std::runtime_error(console.error3);
			}
		}
		catch (exception& e) {
			cout << e.what();
			exit(3);
		}
		break;
	};
	output = std::string(argv[pos++]);

	for (int i = pos; i < argc; i++)
		input.push_back(std::string(argv[i]));

	ofstream out(output, std::ios_base::binary);
	WAV main(input[0]);


	std::vector<WAV> files;
	for (int i = 0; i < input.size(); i++) {
		files.push_back(WAV(input[i]));

	}

	args arguments = args();
	arguments.add = new _stream*;

	for (int i = 0; i < files.size(); i++) {
		arguments.add[i] = new _stream;
		arguments.add[i]->stream = new int16_t[files[i].file.head.subchank2Size];
		arguments.add[i]->size = files[i].file.head.subchank2Size;
		memcpy(arguments.add[i]->stream, files[i].file.data, files[i].file.head.subchank2Size);
	}
	arguments.num = files.size();

	_stream outstream;
	std::vector<std::vector<std::string>> confill = file.Parse(config);
	Mute mut;
	Mix mi;
	TurnUp turn;
	for (int k = 0; k < confill.size(); k++) {
		arguments.main.stream = main.file.data;
		arguments.main.size = main.file.head.subchank2Size;
		arguments.argue = confill[k];
		if (confill[k][0] == mut.name) {
			main.file.data = mut.ProcessWav(arguments);
		}
		else if (confill[k][0] == mi.name) {
			main.file.data = mi.ProcessWav(arguments);
		}
		else if (confill[k][0] == turn.name) {
			main.file.data = turn.ProcessWav(arguments);
		}
		else {
			cerr << console.unknown;
			exit(5);
		}
		cout << "\nPlugin " + confill[k][0] + " started work\n";
		cout<< "\nPlugin " + confill[k][0] + " ended work\n";
	}
	out.write(reinterpret_cast<char*>(&main.file.head), sizeof(WAVHeader));
	out.write(reinterpret_cast<char*>(main.file.data), main.file.head.subchank2Size);
	out.close();

	cout << "\nSound processor ended his work\n";
	return 0;

}