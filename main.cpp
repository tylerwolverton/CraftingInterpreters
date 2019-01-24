#include <iostream>
#include <fstream>
#include <sstream>

#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h"

bool run(std::string source) {
	auto scanner = Scanner(source);
	auto tokens = scanner.ScanTokens();

	std::cout << "Tokens:\n";
	for (auto& token : *tokens)
	{
		std::cout << token.GetLineNum() << ": type: " << token.GetType() << " lexeme: " << token.GetLexeme() << "\n";
	}

	std::cout << "AST Tree";
	auto parser = Parser(tokens);
	auto expr = parser.Parse();
	std::cout << AstPrinter().Print(expr);

	return true;
}

bool runFile(char* script)
{
	// read file into memory
	std::ifstream inFile(script);
	if (!inFile.is_open())
	{
		std::cout << "Input file couldn't be opened\n";
		return false;
	}

	/*std::streampos fileSize = inFile.tellg();
	if (fileSize < 1)
	{
		std::cout << "File is empty\n";
		return;
	}*/

	std::stringstream buffer;
	buffer << inFile.rdbuf();
	inFile.close();

	return run(buffer.str());
}

int main(int argc, char *argv[]) 
{  
	if (argc != 2)
	{
		std::cout << "Usage: lox [script]\n";
		exit(64);
	}
	else if (argc == 2) 
	{
		if (!runFile(argv[1]))
		{

		}
	}
	else 
	{
		//runPrompt();
	}

	/*auto expression = std::make_shared<BinaryExpr>(
		std::make_shared<UnaryExpr>(
			Token(ETokenType::MINUS, "-", 1),
			std::make_shared<LiteralExpr>(Token(ETokenType::NUMBER, "123", 1))),
		Token(ETokenType::STAR, "*", 1),
		std::make_shared<GroupingExpr>(
			std::make_shared<LiteralExpr>(Token(ETokenType::NUMBER, "45.67", 1))));

	std::cout << AstPrinter().Print(expression);*/
}