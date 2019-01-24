#pragma once
#include "Types.h"

#include <iostream>
#include <string>
#include <exception>

class Error
{
public:
	static void DisplayError(int line, std::string msg)
	{
		report(line, "", msg);
	}

	static void DisplayError(Token token, std::string msg)
	{
		if (token.GetType() == ETokenType::END_OF_FILE) 
		{
			report(token.GetLineNum(), " at end", msg);
		}
		else 
		{
			report(token.GetLineNum(), " at '" + token.GetLexeme() + "'", msg);
		}
	}

private:
	static void report(int line, std::string where, std::string msg)
	{
		std::cout << "[line " << line << "] Error" << where << ": " << msg << std::endl;
	}
};

class ParserError : std::nested_exception
{
public:
	ParserError(Token token, std::string msg)
	{
		Error::DisplayError(token, msg);
	}
};