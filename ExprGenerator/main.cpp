#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <locale>

void defineVisitor(std::stringstream& buffer,
				   std::string baseName,
				   std::string returnType,
				   std::vector<std::string> types)
{
	buffer << "class " << baseName << "Visitor { \n";
	buffer << "public:\n";

	for (auto type : types)
	{
		auto typeName = type.substr(0, type.find_first_of(" "));
		auto baseNameLower = baseName;
		std::transform(baseNameLower.begin(), baseNameLower.end(), baseNameLower.begin(), ::tolower);
		buffer << "    virtual " << returnType << " visit" << typeName << "(const std::shared_ptr<" << typeName << ">& " << baseNameLower << ") = 0;\n";
	}

	buffer << "};\n\n";
}

void defineType(std::stringstream& buffer, 
				std::string baseName,
				std::string returnType,
				std::string className,
				std::string fields,
				std::vector<std::string> fieldVec)
{
	buffer << "class " << className << " : public " << baseName << " {\n";
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

	buffer << "    " << returnType << " accept(" << baseName << "Visitor& visitor) override {\n";
	buffer << "        return visitor.visit" << className << "(std::make_shared<" << className << ">(*this));\n";
	buffer << "    }\n\n";

	// Local variables
	for (auto field : fieldVec)
	{
		buffer << "    " << field.substr(0, field.find_first_of(" ")) << " m_" << field.substr(field.find_first_of(" ") + 1) << ";\n";
	}

	buffer << "};\n\n";
}

bool defineAst(std::string outputDir, 
			   std::string baseName, 
			   std::string returnType,
	           std::vector<std::string> types)
{
	std::string path = outputDir + "/" + baseName + ".h";
	std::ofstream outFile(path);
	if (!outFile.is_open())
	{
		std::cout << "Input file couldn't be opened\n";
		return false;
	}

	std::stringstream buffer;
	buffer << "#pragma once\n";
	buffer << "#include \"Types.h\"\n";
	buffer << "#include <memory>\n\n";
	buffer << "#include <vector>\n";

	buffer << "// This file was generated from ExprGenerator.exe. \n";
	buffer << "// If changes are needed, modify ExprGenerator/main.cpp \n\n";

	// Forward declarations fro expr types
	for (const auto& type : types)
	{
		std::string className = type.substr(0, type.find_first_of(" "));
		buffer << "class " << className << ";\n";
	}

	buffer << "\n";

	defineVisitor(buffer, baseName, returnType, types);

	// Define base class
	buffer << "class " << baseName << " {\n";
	buffer << "    public:\n";
	buffer << "    virtual " << returnType << " accept(" << baseName << "Visitor& visitor) = 0;\n";
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

		defineType(buffer, baseName, returnType, className, fields, fieldVec);
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
	if (defineAst(outputDir, "Expr", "std::shared_ptr<void>", std::vector<std::string>{
		"AssignExpr   : Token name, std::shared_ptr<Expr> value",
		"BinaryExpr   : std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
		"CallExpr     : std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments",
		"GroupingExpr : std::shared_ptr<Expr> expr",
		"LiteralExpr  : Token literal",
	    "LogicalExpr  : std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
		"UnaryExpr    : Token op, std::shared_ptr<Expr> right",
		"VariableExpr : Token name"
	}) == false)
	{
		std::cout << "Expr generation failed\n";
		exit(64);
	}

	if (defineAst(outputDir, "Stmt", "void", std::vector<std::string>{
		"BlockStmt      : std::vector<std::shared_ptr<Stmt>> statements",
		"ExpressionStmt : std::shared_ptr<Expr> expr",
		"IfStmt         : std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch",
		"PrintStmt      : std::shared_ptr<Expr> expr",
		"VarStmt        : Token name, std::shared_ptr<Expr> initializer",
		"WhileStmt      : std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body"
	}) == false)
	{
		std::cout << "Stmt generation failed\n";
		exit(64);
	}
}