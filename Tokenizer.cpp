#include "Tokenizer.h"
#include <iostream>

TokenSet* Tokenizer::parse(const char* filename, FileData data)
{
	auto length = data.size - 1;

	State state = State::NONE;
	TokenType type = TokenType::NONE;
	auto set = new TokenSet;
	set->reserve(4096);

	bool isComment = false;
	bool isSlashString = false;

	std::string buffer(100, 0);

	size_t line = 1, col = 0;

	for (size_t i = 0; i < length; i++)
	{
		const auto c = data.data[i];
		col++;

		switch (type)
		{
		case TokenType::NONE:
		{
			if (c == '\n') {
				col = 0;
				line++;
			}
			if (c == '\n' && isComment) { isComment = false; continue; }
			if (c == ' ' || isComment) continue;
			if (c == '#') {
				isComment = !isComment;
			}

			if (is_number(c)) {
				type = TokenType::INTEGER;
				buffer += c;
				continue;
			}

			if (handle_separator_char(c, set, type)) {
				continue;
			}
			else if (is_word(c))
			{
				type = TokenType::WORD;
				buffer += c;
				continue;
			}
			else if (c == '"') {
				type = TokenType::STRING;
				continue;
			}
			else {
				throw SyntaxError(filename, generateTokenErrorString(c), line, col);
			}
		}	
		break;
		case TokenType::INTEGER:
		{
			if (is_number(c))
			{
				buffer += c;
				continue;
			}
			else if (c == '.') {
				buffer += c;
				type = TokenType::FLOAT;
				continue;
			}
			else if (is_separator_char(c)) {
				push_integer(set, buffer);
				buffer.clear();
				handle_separator_char(c, set, type);
			}
			else {
				throw SyntaxError(filename, generateTokenErrorString(c), line, col);
			}
		}
		break;
		case TokenType::FLOAT:
		{
			if (is_number(c)) {
				buffer += c;
			}
			else if (is_separator_char(c)) {
				push_float(set, buffer);
				buffer.clear();
				handle_separator_char(c, set, type);
			}
			else {
				throw SyntaxError(filename, generateTokenErrorString(c), line, col);
			}
		}
		break;
		case TokenType::WORD:
		{
			if (is_word(c) || is_number(c)) {
				buffer += c;
			}
			else if (is_separator_char(c)) {
				push_literal(set, buffer, TokenType::WORD);
				buffer.clear();
				handle_separator_char(c, set, type);
			}
			else {
				throw SyntaxError(filename, generateTokenErrorString(c), line, col);
			}
		}
		break;
		case TokenType::STRING:
		{
			if (c == '\\') {
				if (isSlashString) {
					isSlashString = false;
					switch (c)
					{
					case 'n':
					case 'N':
						buffer += '\n';
						break;
					case 't':
					case 'T':
						buffer += '\t';
						break;
					case 'b':
					case 'B':
						buffer += '\b';
						break;
					case 'r':
					case 'R':
						buffer += '\r';
						break;
					case '"':
						buffer += '"';
						break;
					default:
						throw SyntaxError(filename, generateTokenErrorString(c), line, col);
						break;
					}
				}
				else {
					isSlashString = true;
				}
			}
			else if (c == '"' && !isSlashString) {
				push_literal(set, buffer, TokenType::STRING);
				buffer.clear();
				continue;
			}
			else if (c == '\n') {
				throw SyntaxError(filename, "String not terminated.", line, col);
			}
			else {
				buffer += c;
			}
		}
		break;
		}
	}

	if (data.allocated)
	{
		delete[] data.data;
		data.allocated = false;
	}

	return set;
}


bool Tokenizer::handle_separator_char(const char& c, TokenSet* set, TokenType& type)
{
	switch (c)
	{
	case '<': type = TokenType::LT; return true;
	case '>': type = TokenType::GT; return true;
	case '!': type = TokenType::NOT; return true;
	case '=': type = TokenType::EQ; return true;
	case ':': type = TokenType::COLON; return true;
	case '+': type = TokenType::PLUS; return true;
	case '-': type = TokenType::MINUS; return true;
	case '*': type = TokenType::STAR; return true;
	case '/': type = TokenType::SLASH; return true;
	case ';': push(set, TokenType::SEMICOLON); type=TokenType::NONE; return true;
	case '(': push(set, TokenType::OPEN_BRACE); type = TokenType::NONE; return true;
	case ')': push(set, TokenType::CLOSE_BRACE); type = TokenType::NONE; return true;
	case '[': push(set, TokenType::OPEN_SQUARE); type = TokenType::NONE; return true;
	case ']': push(set, TokenType::CLOSE_SQUARE); type = TokenType::NONE; return true;
	case '{': push(set, TokenType::OPEN_CURLY); type = TokenType::NONE; return true;
	case '}': push(set, TokenType::CLOSE_CURLY); type = TokenType::NONE; return true;
	case '.': push(set, TokenType::DOT); type = TokenType::NONE; return true;
	case ' ': return true;
	default:
		return false;
	}
}

long long Tokenizer::convert_to_long(const std::string& str)
{
	long int result = 0;
	int sign = 1;
	std::size_t i = 0;

	// обработка знака числа
	if (str[i] == '-') {
		sign = -1;
		++i;
	}
	else if (str[i] == '+') {
		++i;
	}

	// обработка цифр числа
	for (; i < str.size(); ++i) {
		if (str[i] >= '0' && str[i] <= '9') {
			result = result * 10 + (str[i] - '0');
		}
		else {
			break;
		}
	}

	return sign * result;
}

double Tokenizer::convert_to_double(const std::string& str)
{
	return std::stod(str);
}

std::string Tokenizer::generateTokenErrorString(const char& c)
{
	return "Unexpected token \"" + c + '"';
}

bool Tokenizer::is_separator_char(const char& c)
{
	std::string separators = "<>!=:+-*/;()[]{}. ";
	return separators.find(c) != std::string::npos;
}

void Tokenizer::push_integer(TokenSet* set, std::string& buffer)
{
	long long number = convert_to_long(buffer);
	char* data = new char[sizeof(number)];
	std::memcpy(data, &number, sizeof(number));
	push(set, TokenType::INTEGER, data);
}

void Tokenizer::push_float(TokenSet* set, std::string& buffer)
{
	double number = convert_to_double(buffer);
	char* data = new char[sizeof(number)];
	std::memcpy(data, &number, sizeof(number));
	push(set, TokenType::FLOAT, data);
}

void Tokenizer::push_literal(TokenSet* set, std::string& buffer, TokenType type)
{
	size_t string_size = buffer.size();
	char* data = new char[string_size+1];
	data[string_size] = '\0';
	std::memcpy(data, buffer.c_str(), string_size);
	push(set, type, data);
}
