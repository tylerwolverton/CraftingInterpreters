#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <locale>

void defineVisitor(std::stringstream& buffer,
				   std::string baseName,
				   std::vector<std::string> types)
{
	buffer << "    template <typename T>\n";
	buffer << "    class Visitor { \n";
	buffer << "    public:\n";

	for (auto type : types)
	{
		auto typeName = type.substr(0, type.find_first_of(" "));
		auto baseNameLower = baseName;
		std::transform(baseNameLower.begin(), baseNameLower.end(), baseNameLower.begin(), ::tolower);
		buffer << "        T visit" << typeName << baseName << "(" << typeName << " " << baseNameLower << ");\n";
	}

	buffer << "    };\n\n";
}

void defineType(std::stringstream& buffer, 
				std::string baseName, 
				std::string className,
				std::string fields,
				std::vector<std::string> fieldVec)
{
	buffer << "  class " << className << " : " << baseName << " {\n";
	buffer << "    public:\n";
	
	// Constructor
	buffer << "    " << className << "(" << fields << " ) \n";

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

	buffer << "    template <typename T>\n";
	buffer << "    T accept(Visitor<T> visitor) {\n";
	buffer << "        return visitor.visit" << className << baseName << "(this);\n";
	buffer << "    }\n";

	// Local variables
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
	buffer << "#include \"Types.h\"\n\n";
	buffer << "    template <typename T>\n";
	buffer << "    class Visitor;\n\n";

	// Define base class
	buffer << "class " << baseName << " {\n";
	buffer << "    template <typename T>\n";
	buffer << "    T accept(Visitor<T> visitor);\n";
	buffer << "};\n\n";

	// Define each subclass
	for (const auto& type : types)
	{
		std::string className = type.substr(0, type.find_first_of(" "));
		std::string fields = type.substr(type.find_first_of(":") + 1);
		std::stringstream fieldStream(fields);
		std::vector<std::string> fieldVec;
		std::string fieldStr;
		while (std::getline(fieldStream, fieldStr, ','))
		{
			fieldVec.push_back(fieldStr.substr(fieldStr.find_first_not_of(" "), fieldStr.find_last_not_of(" ") + 1));
		}

		defineType(buffer, baseName, className, fields, fieldVec);
	}

	defineVisitor(buffer, baseName, types);

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