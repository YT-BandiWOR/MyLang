#pragma once
#include <string>
#include <fstream>
#include "types.hpp"
#include "exceptions.hpp"

FileData read_file_text(const char* filename) {
	std::ifstream file(filename);

	if (!file.is_open()) {
		throw FileReadError(filename);
	}

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = new char[size];

	file.read(buffer, size);
	file.close();

	return {size, buffer};
}