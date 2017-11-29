

#include "Compiler.hpp"

int CCompiler::CreateByteCode(SemanticTokenList_t & stl, CGrammarTable &gl, std::string & ByteCode)
{
	m_ss.str("");
	m_ss.clear();
	for (auto &t : stl)
		PostFixConverter(t, gl);

	ByteCode = m_ss.str();
	return -1;
}

void CCompiler::PostFixConverter(SemanticToken & st, CGrammarTable &gl)
{
	for (auto &t : st.ChildList)
		PostFixConverter(t, gl);

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
		//std::cout << "\nImplicit casting : " << st.DataType << " to " << st.TypecastTo << " made by compiler.";
		m_ss << gl.IsCastable(st.DataType, st.TypecastTo) << "#";
	}
}
