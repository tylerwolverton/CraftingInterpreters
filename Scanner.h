#pragma once
#include "Types.h"
#include <vector>
#include <map>
#include <memory>

class Scanner
{
// TODO: Remove the local vector and create the return on the fly
public:
	Scanner(std::string source);
	~Scanner();

	std::shared_ptr<std::vector<Token>> ScanTokens();

private:
	std::string m_loxSource;
	std::shared_ptr<std::vector<Token>> m_tokens;
	std::shared_ptr<std::map<std::string, ETokenType>> m_keywords;
	int m_startIdx = 0;
	int m_curIdx = 0;
	int m_lineNum = 1;

	void initKeywords();

	void scanToken();
	char advance();
	bool match(char expected);
	char peek();
	char peekNext();

	void addToken(ETokenType type);

	bool isAtEnd();
	bool isDigit(char c);
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);

	void processString();
	void processNumber();
	void processIdentifier();
};

