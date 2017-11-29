

#include "Compiler.hpp"

int CCompiler::CreateByteCode(SemanticTokenList_t & stl, std::string & ByteCode)
{
	for (auto &t : stl)
		Display_PostFix_SemanticTokenList_t(t);

	ByteCode = m_ss.str();
	return -1;
}

void CCompiler::Display_PostFix_SemanticTokenList_t(SemanticToken & st)
{
	for (auto &t : st.ChildList)
		Display_PostFix_SemanticTokenList_t(t);

	if (st.Type == "string")
		m_ss << "_GetString#" << st.Content << "#";
	if (st.Type == "number")
	{
		float num = std::stof(st.Content);
		
		m_ss << "_GetNumber#";
		m_ss.write(reinterpret_cast<char *>(&num), sizeof(float));
		m_ss.put('#');
	}
	if (st.Type == "function")
		m_ss << st.Content << "#";

	if (st.TypecastTo.empty() == false)
	{
		std::cout << "Implicit casting : " << st.DataType << " to " << st.TypecastTo << " made by compiler.";
		m_ss << st.DataType << "To" << st.TypecastTo << "#";
	}
}
