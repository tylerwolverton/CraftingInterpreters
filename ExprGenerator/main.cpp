#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void defineType(std::stringstream& buffer, 
				std::string baseName, 
				std::string className,
				std::string fields)
{
	buffer << "  class " << className << " : " << baseName << " {\n";

	// Constructor
	buffer << "    " << className << "(" << fields << " ) \n";

	std::stringstream fieldStream(fields);
	std::vector<std::string> fieldVec;
	std::string fieldStr;
	while (std::getline(fieldStream, fieldStr, ','))
	{
		fieldVec.push_back(fieldStr.substr(fieldStr.find_first_not_of(" "), fieldStr.find_last_not_of(" ") + 1));
	}

	buffer << "      : ";
	// Store parameters in fields
	for (auto field : fieldVec)
	{
		buffer << "m_" << field.substr(field.find_first_of(" ") + 1) << "(" << field.substr(field.find_first_of(" ") + 1) << ")";
		if (strcmp(field.c_str(), (fieldVec.back()).c_str()))
		{
			buffer << ",";
		}

		buffer << "\n      ";
	}

	buffer << "  {}\n\n";

	for (auto field : fieldVec)
	{
		buffer << "      " << field.substr(0, field.find_first_of(" ")) << " m_" << field.substr(field.find_first_of(" ") + 1) << ";\n";
	}

	buffer << "\n  };\n\n";
}

bool defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types)
{
	std::string path = outputDir + "/" + baseName + ".h";
	std::ofstream outFile(path);
	if (!outFile.is_open())
	{
		std::cout << "Input file couldn't be opened\n";
		return false;
	}

	std::stringstream buffer;
	buffer << "#include \"Types.h\"\n";
	buffer << "class " << baseName << " {\n";

	buffer << "};\n\n";
	for (const auto& type : types)
	{
		std::string className = type.substr(0, type.find_first_of(" "));
		std::string fields = type.substr(type.find_first_of(":") + 1);
		defineType(buffer, baseName, className, fields);
	}

	outFile.write((buffer.str()).c_str(), strlen((buffer.str()).c_str()));

	return true;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ExprGenerator <out_dir>\n";
		exit(64);
	}

	std::string outputDir = argv[1];
	if (defineAst(outputDir, "Expr", std::vector<std::string>{
		"Binary   : Expr left, Token op, Expr right",
		"Grouping : Expr expr",
		"Literal  : Token literal",
		"Unary    : Token op, Expr right"
	}) == false)
	{
		std::cout << "Generation failed\n";
		exit(64);
	}
}