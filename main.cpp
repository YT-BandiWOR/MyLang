#include <iostream>
#include "Tokenizer.h"
#include "exceptions.hpp"
#include "filereader.hpp"


int main(int argc, const char* argv[]) {
	try
	{
		if (argc < 1) {
			std::cerr << "Using: " << argv[0] << " <filepath>" << std::endl;
			return EXIT_FAILURE;
		}

		auto file_name = "main.mln";//argv[1];
		auto file_content = read_file_text(file_name);

		auto tokens = Tokenizer::parse(file_name, file_content);

	}
	catch (FileReadError& exc) {
		std::cerr << exc.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (SyntaxError& exc)
	{
		std::cerr << exc.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}