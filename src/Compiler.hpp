

#pragma once

#include "Semantic_Analyser.hpp"
#include <sstream>

class CCompiler
{
public:
	int CreateByteCode(SemanticTokenList_t &stl, std::string &ByteCode);
private:
	void Display_PostFix_SemanticTokenList_t(SemanticToken &st);
	std::stringstream m_ss;
};
