

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
	int ProcessToken(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, int &i)
	{
		int error = -1;
		if (i >= tl.size()) return i;
		std::cout << "\n\ni = " << i;
		std::cout << "\ntoken = " << tl[i].Content;
		if (tl[i].Type == "identifier")  // could be function or variable
		{
			error = ProcessIdentifier(tl, gl, st, i);
			if (error != -1) return error;
		}
		else // a literal
		{
			error = ProcessLiteral(tl[i], st, i);
			if (error != -1) return error;
		}
		return error;
	}

	int ProcessLiteral(Token_t &t, SemanticToken &st, int &i)
	{
		//std::cout << "\nProcess Literal = " << t.Type;
		st.Content = t.Content;
		st.DataType = st.Type = t.Type;
		i++;
		return -1;
	}

	int ProcessVariable(Token_t &t, CGrammarTable &gl, SemanticToken &st, int &i)
	{
		//std::cout << "\nProcess Variable_t = " << t.Content;
		st.Content = t.Content;
		st.DataType = "unassigned";
		st.Type = t.Type;
		i++;
		return -1;
	}

	int ProcessIdentifier(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, int &i)
	{
		int error = -1;
		if (gl.IsFunction(tl[i].Content)) // function
		{
			error = ProcessFunction(tl, gl, st, i);
		}
		else // variable
		{
			error = ProcessVariable(tl[i], gl, st, i);
		}
		if (error != -1) { std::cout << "\nError in ProcessIdentifier() "; return error; }
		return error;
	}

	int ProcessFunction(TokenList_t &tl, CGrammarTable &gl, SemanticToken &st, int &i)
	{
		//std::cout << "\nProcess Function = " << tl[i].Content;
		int error = -1;
		auto &functionToken = tl[i];
		st.Content = functionToken.Content;
		st.DataType = gl.Get(functionToken.Content).return_type;
		st.Type = "function";

		i++;
		if (tl[i].Type != "syntaxer") { std::cout << "\nExpected snytaxer"; i; }

		i++;
		auto &argTypeList = gl.Get(functionToken.Content).expected_type;
		for (auto &argType : argTypeList)
		{
			std::cout << "\nNext arg = " << argType;
			SemanticToken argToken;
			error = ProcessToken(tl, gl, argToken, i);
			if (error != -1) { std::cout << "\nError in ProcessFunction() "; return error; }
			if (argType != argToken.DataType)
			{
				// check if it can be converted
				if (gl.IsCastable(argToken.DataType, argType) != "")
				{
					std::cout << "\nImplicit typecast from " << argToken.DataType << " to " << argType;
					argToken.TypecastTo = argType;
				}
				else
				{
					std::cout << "\nFunction expected argument of type " << argType << ", but got " << argToken.DataType;
					error = i;
					return error;
				}
			}
			st.ChildList.push_back(argToken);
		}

		if (tl[i].Type != "syntaxer") { std::cout << "\nExpected snytaxer"; i; }
		i++;
		//std::cout << "\nFunction done!!!!";
		return error;
	}
};
