#pragma once
#include <string>
#include <vector>

class Scanner
{
public:
	Scanner(std::string source);
	~Scanner();

	class Token
	{
	public:
		std::string tokenStr;
		Token(std::string token)
			: tokenStr(token)
		{}

		const std::string ToString() const { return tokenStr; }
	};

	std::vector<Token> ScanTokens();

private:
	std::string loxSource;
};

