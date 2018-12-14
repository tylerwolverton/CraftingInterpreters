#include <iostream>
#include <fstream>
#include <sstream>

#include "Scanner.h"

void run(std::string source) {
	auto scanner = Scanner(source);
	auto tokens = scanner.ScanTokens();

	// For now, just print the tokens.        
	for (const auto& token : tokens) 
	{
		std::cout << token.ToString() << std::endl;
	}
}

void runFile(char* script)
{
	// read file into memory
	std::ifstream inFile(script);
	if (!inFile.is_open())
	{
		std::cout << "Input file couldn't be opened\n";
		return;
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

	run(buffer.str());
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
		runFile(argv[1]);
	}
	else 
	{
		//runPrompt();
	}
}