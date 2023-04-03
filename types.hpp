#pragma once
#include <vector>
#include <fstream>

enum class TokenType
{
	NONE,
	INTEGER, FLOAT, STRING, BOOL, WORD,

	COLON, SEMICOLON, DOT,

	PLUS, MINUS, STAR, SLASH,
	PLUSEQ, MINUSEQ, STAREQ, SLASHEQ,

	OPEN_BRACE, CLOSE_BRACE,
	OPEN_CURLY, CLOSE_CURLY,
	OPEN_SQUARE, CLOSE_SQUARE,

	EQ, EQEQ, LT, LTE, GT, GTE, NOTEQ, NOT
};

struct FileData
{
	std::streamsize size;
	char* data;
	bool allocated = true;
};


using TokenSet = std::vector<std::pair<TokenType, char*>>;