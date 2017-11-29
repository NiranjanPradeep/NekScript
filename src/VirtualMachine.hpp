

#pragma once

#include "Grammar.hpp"
#include "Compiler.hpp"
#include "Interpreter.hpp"

class CVirtualMachine
{
public:
	CVirtualMachine();
	void AttachGrammar(CGrammarTable *ptr);
	int ExecuteScript(const std::string &script);
private:
	CTokenizer tokenizer;
	CSemanticAnalyser semanticAnalyser;
	CCompiler compiler;
	CInterpreter interpreter;
	CGrammarTable *pGrammarTable;
};