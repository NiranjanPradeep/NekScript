

#pragma once

#include "Tokenizer.hpp"
#include "Grammar.hpp"

struct SemanticToken : public Token_t
{
	std::string DataType;
	std::vector<SemanticToken> ChildList;
	std::string TypecastTo;
	void Display()
	{
		Token_t::Display();
		std::cout << "\nDataType = " << DataType << std::endl;
		for (auto &t : ChildList)
			t.Display();
	}
};

using SemanticTokenList_t = std::vector<SemanticToken>;

class CSemanticAnalyser
{
public:
	int Analyze(TokenList_t &tl, CGrammarTable &gl, SemanticTokenList_t &stl)
	{
		int i = 0;
		int error = -1;
		while (i < static_cast<int>(tl.size()))
		{
			SemanticToken st;
			error = ProcessToken(tl, gl, st, i);
			if (error != -1) { std::cout << "\nError in DoGrammar() "; return error; }
			stl.push_back(st);
		}
		return error;
	}
private:
	int			ProcessToken(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, int &i);
	int			ProcessLiteral(Token_t &t, SemanticToken &st, int &i);
	int			ProcessVariable(Token_t &t, CGrammarTable &gl, SemanticToken &st, int &i);
	int			ProcessIdentifier(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, int &i);
	int			ProcessFunction(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, int &i);
};
