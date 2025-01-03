#include <iostream>
#include <fstream>
#include <sstream>

#include "Scanner.h"
#include "Parser.h"
#include "Resolver.h"
#include "AstPrinter.h"
#include "Interpreter.h"

bool run(std::string source) 
{
	try 
	{
		auto scanner = Scanner(source);
		auto tokens = scanner.ScanTokens();

		auto parser = Parser(tokens);
	
		auto statements = parser.Parse();

		auto interpreter = std::make_shared<Interpreter>();

		auto resolver = Resolver(interpreter);
		resolver.Resolve(statements);

		interpreter->Interpret(statements);
	}
	catch (...)
	{
		return false;
	}

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
}