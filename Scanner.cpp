#include "Scanner.h"

Scanner::Scanner(std::string source)
	: loxSource(source)
{
}

Scanner::~Scanner()
{
}

std::vector<Scanner::Token> Scanner::ScanTokens()
{
	std::vector<Token> tokens;

	std::string curWord = "";
	for (const auto& character : loxSource)
	{
		if (character == ' ')
		{
			tokens.push_back(Token(curWord));
			curWord = "";
		}
		else
		{
			curWord += character;
		}
	}

	if (curWord != " ")
	{
		tokens.push_back(Token(curWord));
	}

	return tokens;
}