

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
	CTokenizer			m_Tokenizer;
	CSemanticAnalyser	m_SemanticAnalyser;
	CCompiler			m_Compiler;
	CInterpreter		m_Interpreter;
	CGrammarTable *		m_pGrammarTable;
};