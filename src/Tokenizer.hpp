
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cctype>

struct Token_t
{
	std::string		Content;
	std::string		Type;
public:
	void 		Display()
	{
		std::cout << std::boolalpha
			<< "\nContent = " << Content
			<< "\nType = " << Type;
	}
};

using TokenList_t = std::vector<Token_t>;

class CTokenizer
{
public:
	void				Assign(const std::string *Script) { m_pScript = Script; }
	int					Tokenize(TokenList_t &tokenList);
private:
	const std::string 	*m_pScript;
};
