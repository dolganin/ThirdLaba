#pragma once

#include<stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
using namespace std;



typedef struct _stream {
	int16_t* stream;
	unsigned long long size;
};

#pragma pack (push,1)
typedef struct WAVHeader_ {
	int32_t chunkId, chunkSize, format, subchunk1Id, subchunk1Size;
	int16_t audioformat, numchannels;
	int32_t sampleRate, byteRate;
	int16_t blockAlign, bitsPerSample;
	int32_t subchank2Id, subchank2Size;
}WAVHeader;
#pragma pack (pop)
#pragma pack (push,1)
typedef struct WAVFile {

	WAVHeader head;
	int16_t* data;
};
#pragma pack (pop)
class args {
public:
	args() {
		this->main = { 0 };
		this->add = NULL;
		this->num = { 0 };
	}
	~args() {
		if (add != NULL) {
			for (int i = 0; i < num; i++) {
				delete[] add[i]->stream;
				delete add[i];
			}
			add = nullptr;
		}
	}
	_stream main;
	_stream** add;
	int num;
	std::vector<std::string> argue;
};



class Base {
public:
	virtual int16_t* ProcessWav(args& a) = 0; 
	int freq = 44100;
};


