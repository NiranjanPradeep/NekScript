#include "Semantic_Analyser.hpp"

int CSemanticAnalyser::ProcessToken(TokenList_t & tl, CGrammarTable & gl, SemanticToken & st, int & i)
{
	int error = -1;
	if (i >= static_cast<int>(tl.size())) { std::cout << "\nNo tokens available."; return i; }
	//std::cout << "\n\ni = " << i;
	//std::cout << "\ntoken = " << tl[i].Content;
	if (tl[i].Type == "syntaxer")
	{
		st.Type = "syntaxer";
		st.DataType = "paranthesis";
		i++;
		return error;
	}
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

int CSemanticAnalyser::ProcessLiteral(Token_t & t, SemanticToken & st, int & i)
{
	//std::cout << "\nProcess Literal = " << t.Type;
	st.Content = t.Content;
	st.DataType = st.Type = t.Type;
	i++;
	return -1;
}

int CSemanticAnalyser::ProcessVariable(Token_t & t, CGrammarTable & gl, SemanticToken & st, int & i)
{
	return i;
	st.Content = t.Content;
	st.DataType = st.Type = t.Type;
	i++;
	return -1;
}

int CSemanticAnalyser::ProcessIdentifier(TokenList_t & tl, CGrammarTable & gl, SemanticToken & st, int & i)
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

int CSemanticAnalyser::ProcessFunction(TokenList_t & tl, CGrammarTable & gl, SemanticToken & st, int & i)
{
	SemanticToken syntaxer;
	//std::cout << "\nProcess Function = " << tl[i].Content;
	int error = -1;
	auto &functionToken = tl[i];
	st.Content = functionToken.Content;
	st.DataType = gl.Get(functionToken.Content).return_type;
	st.Type = "function";

	i++;
	ProcessToken(tl, gl, syntaxer, i);
	if (syntaxer.Type != "syntaxer") { std::cout << "\nExpected syntaxer but got " << syntaxer.Type; i; }

	auto &argTypeList = gl.Get(functionToken.Content).argument_types;
	for (auto &argType : argTypeList)
	{
		// std::cout << "\nNext arg = " << argType;
		SemanticToken argToken;
		error = ProcessToken(tl, gl, argToken, i);
		if (error != -1) { std::cout << "\nError in ProcessFunction() "; return error; }
		if (argType != argToken.DataType)
		{
			// check if it can be converted
			if (gl.IsCastable(argToken.DataType, argType) != "")
			{
				//std::cout << "\nImplicit typecast from " << argToken.DataType << " to " << argType;
				argToken.TypecastTo = argType;
			}
			else
			{
				std::cout << "\nFunction expected argument of type " << argType << ", but got " << argToken.Content << " of type " << argToken.DataType;
				error = i;
				return error;
			}
		}
		st.ChildList.push_back(argToken);
	}

	ProcessToken(tl, gl, syntaxer, i);
	if (syntaxer.Type != "syntaxer") { std::cout << "\nExpected syntaxer but got " << syntaxer.Type; i; }
	//std::cout << "\nFunction done!!!!";
	return error;
}
