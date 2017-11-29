#include "VirtualMachine.hpp"

CVirtualMachine::CVirtualMachine()
{

}

void CVirtualMachine::AttachGrammar(CGrammarTable *ptr)
{
	pGrammarTable = ptr;
}

int CVirtualMachine::ExecuteScript(const std::string &script)
{
	TokenList_t tokenList;
	SemanticTokenList_t semanticTokenList;
	std::string byteCode;

	tokenizer.Assign(&script);
	int error = tokenizer.Tokenize(tokenList);
	if (error != -1)
	{
		std::cout << "\nError while parsing. i = " << error << ", " << std::string(script.begin() + error, script.end());
		return error;
	}
	else
	{
		for (auto &t : tokenList)
		{
			std::cout.put('\n');
			t.Display();
		}
	}

	error = semanticAnalyser.Analyze(tokenList, *pGrammarTable, semanticTokenList);
	if (error != -1)
	{ 
		std::cout << "\nError: Semantic error, i = " << error << ", " << std::string((error>=tokenList.size()) ? "unexpected end of file" : tokenList[error].Content);
		return error;
	}
	else
	{
		std::cout << "\n\nSemantically Correct!\n";
		for (auto &t : semanticTokenList)
		{
			t.Display();
			std::cout.put('\n');
		}
	}

	compiler.CreateByteCode(semanticTokenList, *pGrammarTable, byteCode);
	std::cout << "\nByteCode : " << byteCode;

	std::cout << "\nExecuting...";
	
	return interpreter.Execute(byteCode, *pGrammarTable);
}
