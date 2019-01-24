#include "Scanner.h"
#include "Error.h"

Scanner::Scanner(std::string source)
	: m_loxSource(source)
{
	m_tokens = std::make_shared<std::vector<Token>>();
	initKeywords();
}

Scanner::~Scanner()
{
}

void Scanner::initKeywords()
{
	m_keywords = std::make_shared<std::map<std::string, ETokenType>>();
	m_keywords->insert(std::make_pair("and", ETokenType::AND));
	m_keywords->insert(std::make_pair("class", ETokenType::CLASS));
	m_keywords->insert(std::make_pair("else", ETokenType::ELSE));
	m_keywords->insert(std::make_pair("false", ETokenType::FALSE));
	m_keywords->insert(std::make_pair("for", ETokenType::FOR));
	m_keywords->insert(std::make_pair("fun", ETokenType::FUN));
	m_keywords->insert(std::make_pair("if", ETokenType::IF));
	m_keywords->insert(std::make_pair("nil", ETokenType::NIL));
	m_keywords->insert(std::make_pair("or", ETokenType::OR));
	m_keywords->insert(std::make_pair("print", ETokenType::PRINT));
	m_keywords->insert(std::make_pair("return", ETokenType::RETURN));
	m_keywords->insert(std::make_pair("super", ETokenType::SUPER));
	m_keywords->insert(std::make_pair("this", ETokenType::THIS));
	m_keywords->insert(std::make_pair("true", ETokenType::TRUE));
	m_keywords->insert(std::make_pair("var", ETokenType::VAR));
	m_keywords->insert(std::make_pair("while", ETokenType::WHILE));
}

std::shared_ptr<std::vector<Token>> Scanner::ScanTokens()
{
	while (!isAtEnd())
	{
		m_startIdx = m_curIdx;
		scanToken();
	}

	m_tokens->emplace_back(ETokenType::END_OF_FILE, "", m_lineNum);

	return m_tokens;
}

void Scanner::scanToken() 
{
	char c = advance();
	switch (c) 
	{
		case '(': addToken(ETokenType::LEFT_PAREN); break;
		case ')': addToken(ETokenType::RIGHT_PAREN); break;
		case '{': addToken(ETokenType::LEFT_BRACE); break;
		case '}': addToken(ETokenType::RIGHT_BRACE); break;
		case ',': addToken(ETokenType::COMMA); break;
		case '.': addToken(ETokenType::DOT); break;
		case '-': addToken(ETokenType::MINUS); break;
		case '+': addToken(ETokenType::PLUS); break;
		case ';': addToken(ETokenType::SEMICOLON); break;
		case '*': addToken(ETokenType::STAR); break;
		case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
		case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
		case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
		case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
		case '/':
			if (match('/')) 
			{
				// A comment goes until the end of the line.                
				while (peek() != '\n' && !isAtEnd())
				{
					advance();
				}
			}
			// TODO: throw error if comment not closed
			else if (match('*'))
			{
				while (!isAtEnd() && !(match('*') && match('/')))
				{
					if (peek() == '\n')
					{
						m_lineNum++;
					}
					advance();
				}
			}
			else 
			{
				addToken(SLASH);
			}
			break;
		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace.                      
			break;

		case '\n':
			m_lineNum++;
			break;
		case '"':
			processString();
			break;
		default: 
			if (isDigit(c)) 
			{
				processNumber();
			}
			else if (isAlpha(c))
			{
				processIdentifier();
			}
			else 
			{
				Error::DisplayError(m_lineNum, "Unexpected character.");
			}
			break;
	}
}

char Scanner::advance()
{
	m_curIdx++;
	return m_loxSource[m_curIdx - 1];
}

bool Scanner::match(char expected)
{
	if (isAtEnd())
	{
		return false;
	}

	if (m_loxSource[m_curIdx] != expected)
	{
		return false;
	}

	m_curIdx++;
	return true;
}

char Scanner::peek()
{
	if (isAtEnd())
		return '\0';

	return m_loxSource[m_curIdx];
}

char Scanner::peekNext() 
{
	if (m_curIdx + 1 >= m_loxSource.size()) 
		return '\0';
	
	return m_loxSource[m_curIdx + 1];
}

void Scanner::addToken(ETokenType type)
{
	std::string text = m_loxSource.substr(m_startIdx, m_curIdx);
	m_tokens->emplace_back(type, text, m_lineNum);
}

bool Scanner::isAtEnd()
{
	return m_curIdx >= m_loxSource.size();
}

bool Scanner::isAlpha(char c)
{
	return c >= 'a' && c <= 'z'
		|| c >= 'A' && c <= 'Z'
		|| c == '_';
}

bool Scanner::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool Scanner::isAlphaNumeric(char c)
{
	return isAlpha(c) || isDigit(c);
}

void Scanner::processString()
{
	// TODO: support escaped characters?
	while (peek() != '"' && !isAtEnd())
	{
		if (peek() == '\n')
		{
			m_lineNum++;
		}
		advance();
	}

	if (isAtEnd())
	{
		// Throw error
		return;
	}

	// closing "
	advance();

	addToken(ETokenType::STRING);	
}

void Scanner::processNumber()
{
	while (isDigit(m_loxSource[m_curIdx]))
	{
		advance();
	}

	if (peek() == '.' && isDigit(peekNext()))
	{
		// consume decimal point
		advance();

		while (isDigit(peek()))
		{
			advance();
		}
	}

	addToken(ETokenType::NUMBER);
}

void Scanner::processIdentifier()
{
	while (isAlphaNumeric(peek())) 
		advance();

	std::string text = m_loxSource.substr(m_startIdx, m_curIdx);

	ETokenType type;
	auto iter = m_keywords->find(text);
	if (iter == m_keywords->end())
		type = ETokenType::IDENTIFIER;
	else
		type = iter->second;

	addToken(type);
}