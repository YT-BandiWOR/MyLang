#pragma once
#include <string>


class BasicException {
protected:
	std::string error_type;

	void AddHeaderLine(std::string& text) {
		text += "Program error. Exception caught.\n";
	}

	void AddErrorTypeLine(std::string& text) {
		text += "ErrorType: ";
		text += this->error_type;
		text += '\n';
	}

public:
	BasicException(const std::string& error_type) {
		this->error_type = error_type;
	}
	
	const std::string what() {
		std::string str(64, 0);
		AddHeaderLine(str);
		AddErrorTypeLine(str);

		return str;
	}
};


class ProgramError : public BasicException {
protected:
	size_t line;
	size_t column;

	void AddPositionLine(std::string& text) {
		text += "Line: \t";
		text += std::to_string(line);
		text += "\nChar: \t";
		text += std::to_string(column);
		text += '\n';
	}

public:
	ProgramError(const std::string& error_type, size_t line, size_t column) : BasicException(error_type) {
		this->line = line;
		this->column = column;
	}

	const std::string what() {
		std::string str(128, 0);
		AddHeaderLine(str);
		AddErrorTypeLine(str);
		AddPositionLine(str);
	}
};

class ErrorExtensionFilename {
protected:
	std::string filename;

	ErrorExtensionFilename(const std::string filename) {
		this->filename = filename;
	}

	void AddFilenameLine(std::string& str) {
		str += "Filename: ";
		str += this->filename;
		str += '\n';
	}
};

class ErrorWithFullArgs : public ProgramError, public ErrorExtensionFilename {
protected:
	std::string message;

	void AddMessageLine(std::string& str) {
		str += "Description: ";
		str += this->message;
		str += '\n';
	}

public:
	ErrorWithFullArgs(const std::string& error_type, const std::string& filename, const std::string& message, size_t line, size_t column) : ProgramError(error_type, line, column), ErrorExtensionFilename(filename) {
		this->message = message;
	}

	const std::string what() {
		std::string str(300, 0);
		AddHeaderLine(str);
		AddErrorTypeLine(str);
		AddFilenameLine(str);
		AddPositionLine(str);
		AddMessageLine(str);
		return str;
	}
};

class SyntaxError : public ErrorWithFullArgs {
public:
	SyntaxError(const std::string& filename, const std::string& message, size_t line, size_t column) : ErrorWithFullArgs("SyntaxError", filename, message, line, column) {}
};

class FileReadError : public BasicException, public ErrorExtensionFilename {
public:
	FileReadError(const std::string& filename) : BasicException("FileReadError"), ErrorExtensionFilename(filename) {}

	const std::string what() {
		std::string str(256, 0);
		AddHeaderLine(str);
		AddErrorTypeLine(str);
		AddFilenameLine(str);
		return str;
	}
};