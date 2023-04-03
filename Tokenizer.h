#pragma once
#include "types.hpp"
#include "exceptions.hpp"
#include <string>


class Tokenizer
{
private:
	enum class State
	{
		NONE, WORD, STRING, NUM, OPERATOR
	};

public:
	static TokenSet* parse(const char* filename, FileData data);

	static bool handle_separator_char(const char& c, TokenSet* set, TokenType& type);
	static bool is_separator_char(const char& c);

	static void push_integer(TokenSet* set, std::string& buffer);
	static void push_float(TokenSet* set, std::string& buffer);
	static void push_literal(TokenSet* set, std::string& buffer, TokenType type);

	static long long convert_to_long(const std::string& str);
	static double convert_to_double(const std::string& str);

	static std::string generateTokenErrorString(const char&);

	static inline bool is_number(char c) {
		return (c >= '0' && c <= '9');
	}

	static inline bool is_word(char c) {
		return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c == '$');
	}

	static inline void push(TokenSet* set, TokenType type, char* data = nullptr) {
		set->push_back(std::pair<TokenType, char*>(type, data));
	}
};

