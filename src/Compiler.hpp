

#pragma once

#include "Semantic_Analyser.hpp"
#include "Grammar.hpp"
#include <sstream>

class CCompiler
{
public:
	int CreateByteCode(SemanticTokenList_t &stl, CGrammarTable &gl, std::string &ByteCode);
private:
	void PostFixConverter(SemanticToken &st, CGrammarTable &gl);
	std::stringstream m_ss;
};
