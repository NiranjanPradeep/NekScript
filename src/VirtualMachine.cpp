#include "VirtualMachine.hpp"

CVirtualMachine::CVirtualMachine()
{

}

void CVirtualMachine::AttachGrammar(CGrammarTable *ptr)
{
	m_pGrammarTable = ptr;
}

int CVirtualMachine::ExecuteScript(const std::string &script)
{
	TokenList_t tokenList;
	SemanticTokenList_t semanticTokenList;
	std::string byteCode;

	m_Tokenizer.Assign(&script);
	int error = m_Tokenizer.Tokenize(tokenList);
	if (error != -1)
	{
		std::cout << "\nError while parsing. i = " << error << ", " << std::string(script.begin() + error, script.end());
		return error;
	}
	else
	{
		/*
		std::cout << "\nSyntactically Correct!";
		for (auto &t : tokenList)
		{
			std::cout.put('\n');
			t.Display();
		}
		*/
	}

	error = m_SemanticAnalyser.Analyze(tokenList, *m_pGrammarTable, semanticTokenList);
	if (error != -1)
	{ 
		std::cout << "\nError: Semantic error, i = " << error << std::string(
				(error>=static_cast<int>(tokenList.size())) ? 
				(". Got unexpected end of file") : 
				(". Error at " + tokenList[error].Content)
			);
		return error;
	}
	else
	{
		/*
		std::cout << "\nSemantically Correct!";
		for (auto &t : semanticTokenList)
		{
			std::cout.put('\n');
			t.Display();
		}
		*/
	}

	m_Compiler.CreateByteCode(semanticTokenList, *m_pGrammarTable, byteCode);
	// std::cout << "\nByteCode : " << byteCode;

	//std::cout << "\nExecuting...";
	return m_Interpreter.Execute(byteCode, *m_pGrammarTable);
}
