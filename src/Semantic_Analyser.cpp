

#include "Semantic_Analyser.hpp"

int					Precedence(char ch)
{
	switch (ch)
	{
	case '+': return 1;
	case '-': return 1;
	case '*': return 2;
	case '/': return 2;
	case '(': return -1;
	case ')': return -1;
	}
	return -1;
}

int CSemanticAnalyser::ProcessToken(TokenList_t & tl, CGrammarTable & gl, SemanticToken & st, SemanticTokenList_t &stl, int & i)
{
	int error = -1;
	
	if (i >= static_cast<int>(tl.size())) { std::cout << "\nNo tokens available."; return i; }
	//
	//std::cout << "\n\ni = " << i;
	//std::cout << "\ntoken = " << tl[i].Content;
	//std::cout << "\none : "; for (auto &t : stl) t.Display();
	
	if (tl[i].Type == "seperator")
	{
		st.DataType = "N/A";
		st.Content = tl[i].Content;
		st.Type = "seperator";
		i++;
		return error;
	}
	if (tl[i].Type == "operator")
	{
		error = ProcessOperator(tl, gl, st, stl, i);
		return error;
	}
	if (tl[i].Type == "identifier")  // could be function or variable
	{
		error = ProcessIdentifier(tl, gl, st, stl, i);
		return error;
	}
	else // a literal
	{
		error = ProcessLiteral(tl[i], st, i);
		return error;
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

int CSemanticAnalyser::ProcessIdentifier(TokenList_t & tl, CGrammarTable & gl, SemanticToken & st, SemanticTokenList_t &stl, int & i)
{
	int error = -1;
	if (gl.IsFunction(tl[i].Content)) // function
	{
			error = ProcessFunction(tl, gl, st, stl, i);
	}
	else // variable
	{
		error = ProcessVariable(tl[i], gl, st, i);
	}
	if (error != -1) { std::cout << "\nError in ProcessIdentifier() "; return error; }
	return error;
}

int CSemanticAnalyser::ProcessFunction(TokenList_t & tl, CGrammarTable & gl,  SemanticToken & st, SemanticTokenList_t &stl, int & i)
{
	//std::cout << "\nProcess Function = " << tl[i].Content;
	int error = -1;
	auto &functionToken = tl[i];
	SemanticToken para;
	st.Content = functionToken.Content;
	st.DataType = gl.Get(functionToken.Content).return_type;
	st.Type = "function";

	i++;
	int ParaCount = OpeningBrackets;
	error = ProcessToken(tl, gl, para, stl, i);
	if (error != -1 || para.Content != "(") { std::cout << "\nExpected ( but got " << para.Content; return i; }

	auto &argTypeList = gl.Get(functionToken.Content).argument_types;
	int argCount = 0;
	//for (auto &argType : argTypeList)
	SemanticToken argToken;
	

	bool exit = false;
	int argumentsLeft = 0;

	auto &AddArgument = [&]() {
		argToken = stl.back();
		stl.pop_back();
		auto &argType = argTypeList[argumentsLeft];
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
	};
	int index = stl.size();
	while  (true)
	{
		error = ProcessToken(tl, gl, argToken, stl, i);
		if (error != -1) return error;
		int n = stl.size();
		if (argToken.Type == "seperator")
		{
			//std::cout << "\nGot seperator";
			if (argumentsLeft == argTypeList.size())
			{
				std::cout << "\nError: extra arguments";
				return i;
			}
			if (index == n - 1)
			{
				AddArgument();
				argumentsLeft++;
				continue;
			}
			else
			{
				std::cout << "\nError: extra stack";
				return i;
			}
		}
		if (
			argToken.Type == "operator" &&
			argToken.Content == ")"
			)
		{
			//std::cout << "\nGot )";
			if (ParaCount == OpeningBrackets)
			{
				// Try to end function call
				if (index == n)
				{
					// all arguments were added
					if (argumentsLeft == argTypeList.size())
					{
						break;
					}
					else
					{
						std::cout << "\nError: function requires more arguments.";
						return i;
					}
				}
				if (index > n)
				{
					// popped too many
					std::cout << "\nError: strange error.";
					return i;
				}
				if (index+1 == n)
				{
					// there exists an argument
					if (argumentsLeft == argTypeList.size())
					{
						std::cout << "\nError: too many arguments.";
						return i;
					}
					else
					{
						AddArgument();
						//std::cout << "\nGot argument.";
						argumentsLeft++;
						if (argumentsLeft == argTypeList.size()) break;
						continue;
					}
				}
				std::cout << "\nError: extra stack";
				return i;
			}
		}
		if (argToken.Type != "operator")
		{
			stl.push_back(argToken);
			continue;
		}
	}

	return error;
}

int CSemanticAnalyser::ProcessOperator(TokenList_t & tl, CGrammarTable &gl, SemanticToken & st, SemanticTokenList_t & stl, int & i)
{
	st.Type = "operator";
	st.DataType = "unassigned";
	st.Content = tl[i].Content;
	
	if (st.Content == "(")
	{
		OperatorStack.push_back('(');
		i++;
		OpeningBrackets++;
		return -1;
	}
	if (st.Content == ")")
	{
		if (OperatorStack.back() == '(')
		{
			OperatorStack.pop_back();
			i++;
			OpeningBrackets--;
			return -1;
		}
	}

	char op = st.Content.back();
	if (Precedence(op) > Precedence(OperatorStack.back()))
	{
		OperatorStack.push_back(op);
		i++;
		return -1;
	}
	else
	{
		char opSt = OperatorStack.back();
		OperatorStack.pop_back();
		if (stl.size() < 2) { std::cout << "\nError: operator " << opSt << " requires 2 operands"; return i; }
		SemanticToken operand1 = stl[stl.size() - 2];
		SemanticToken operand2 = stl[stl.size() - 1];
		
		std::string DataType = gl.HasOperator(operand1.DataType, std::string(1, opSt));
		if (DataType == "")
		{
			std::cout << "\nError: operator " << std::string(1, opSt) << " undefined for " << operand1.DataType;
			return i;
		}
		if (operand2.DataType != operand1.DataType) operand2.TypecastTo = operand1.DataType;
		stl.pop_back();
		stl.pop_back();
		SemanticToken result;
		result.Type = "operator";
		result.DataType = gl.Get(DataType).return_type;
		result.Content = opSt;
		result.ChildList.push_back(std::move(operand1));
		result.ChildList.push_back(std::move(operand2));
		stl.push_back(result);
		//std::cout << "\nOperator is " << st.Content;
		//std::cout << "\nStack is after popping twice:"; for (auto &t : OperatorStack) std::cout << t << " "; std::cout << "\n";
		return -1;
	}
	return -1;
}
