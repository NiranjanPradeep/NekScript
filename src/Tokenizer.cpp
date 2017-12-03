

#include "Tokenizer.hpp"

int CTokenizer::Tokenize(TokenList_t & tokenList)
{
	auto &script = *m_pScript;
	int error = -1;
	bool wordStart = true;

	std::string currentToken;
	int paranthesisCounter = 0;

	int i = 0;
	auto t = [&]()->char { if (i >= static_cast<int>(script.size())) return 0; /*std::cout << script[i];*/ return script[i]; };
	auto AddToken = [&](std::string &&a) {
		tokenList.push_back({ std::move(currentToken), std::move(a) });
	};
	auto AddLexeme = [&]() { currentToken.push_back(t()); i++; };
	while (true)
	{
		if (error != -1) break;
		if (t() == 0) break;
		if (wordStart && std::isalpha(t()))
		{
			while (t() == '_' || std::isalnum(t()) || std::isalpha(t()))
				AddLexeme();

			AddToken("identifier");
			wordStart = false;
			continue;
		}
		if (std::isdigit(t()) && wordStart && t())
		{
			while (std::isdigit(t()) || t() == '.')
				AddLexeme();

			AddToken("number");
			wordStart = false;
			continue;
		}
		if (wordStart && t() == '\"')
		{
			i++;
			while (t() != '\"')
			{
				if (t() == 0)
				{
					error = i;
					break;
				}
				AddLexeme();
			}
			i++;
			AddToken("string");
			wordStart = false;
			continue;
		}
		if (t() == ',')
		{
			AddLexeme();
			AddToken("seperator");
			paranthesisCounter++;
			wordStart = true;
			continue;
		}
		if (t() == '(')
		{
			AddLexeme();
			AddToken("operator");
			paranthesisCounter++;
			wordStart = true;
			continue;
		}
		if (t() == ')')
		{
			if (paranthesisCounter > 0)
			{
				paranthesisCounter--;
				AddLexeme();
				AddToken("operator");
				wordStart = true;
			}
			else error = i;
			continue;
		}
		if (IsOperator(t()))
		{
			AddLexeme();
			AddToken("operator");
			wordStart = true;
			continue;
		}
		if (std::isspace(t()))
		{
			wordStart = true;
			i++;
			continue;
		}
		error = i;
	}
	if (paranthesisCounter != 0) return i;
	return error;
}
