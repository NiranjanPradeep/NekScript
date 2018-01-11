

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
		static int i = 0;
		//Token_t::Display();
		//std::cout << "\nDataType = " << DataType << std::endl;
		std::cout << "\n";
		for (int j = 0; j < i; j++) std::cout.put(i);
		std::cout << "Content = " << Content;
		i += 4;
		for (auto &t : ChildList) t.Display();
		i -= 4;
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
		OpeningBrackets = 1;
		OperatorStack.push_back('(');
		tl.push_back({")", "operator"});

		while (i < static_cast<int>(tl.size()))
		{
			SemanticToken st;
			// std::cout << "\nCurrent token = " << tl[i].Content;
			error = ProcessToken(tl, gl, st, stl, i);
			if (error != -1) { std::cerr << "\nError in DoGrammar() "; return error; }
			if (st.Type == "operator" && st.DataType == "unassigned") continue;
			if (st.Type == "seperator") continue;
			stl.push_back(st);
		}
		return error;
	}
private:
	int			ProcessToken(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, SemanticTokenList_t &stl, int &i);
	int			ProcessLiteral(Token_t &t, SemanticToken &st, int &i);
	int			ProcessVariable(Token_t &t, CGrammarTable &gl, SemanticToken &st, int &i);
	int			ProcessIdentifier(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, SemanticTokenList_t &stl, int &i);
	int			ProcessFunction(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, SemanticTokenList_t &stl,int &i);
	int			ProcessOperator(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, SemanticTokenList_t &stl, int &i);
private:
	std::vector<char>		OperatorStack;
	int						OpeningBrackets;
};
