#include <iostream>
#include <fstream>
#include <sstream>

#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "Interpreter.h"

bool run(std::string source) 
{
	auto scanner = Scanner(source);
	auto tokens = scanner.ScanTokens();

	auto parser = Parser(tokens);
	auto statements = parser.Parse();
	
	Interpreter().Interpret(statements);

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
			return 1;
		}
	}
	else 
	{
		//runPrompt();
	}
}