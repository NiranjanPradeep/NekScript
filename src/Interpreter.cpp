
#include "Interpreter.hpp"
#include <iostream>

int CInterpreter::Execute(std::string ByteCode, CGrammarTable & gl)
{
	std::string current;
	m_ss.clear();
	m_ss.str(ByteCode);
	while (std::getline(m_ss, current, '#'))
	{
		//std::cout << "\nCurrent word = " << current;
		auto &func = gl.Get(current);

		m_StackFrameOffset = m_StackFrame.size()-func.argument_types.size();
		if(func.function_ptr)
			func.function_ptr(&m_Sfc);
		m_StackFrame.erase(m_StackFrame.begin() + m_StackFrameOffset, m_StackFrame.begin() + m_StackFrameOffset + func.argument_types.size());
		//std::cout << "\nStack frame is : "; for (auto &t : stackFrame) std::cout << *(std::string *)t.Data << "@";
	}
	for (auto &t : m_StackFrame)
		t.Deleter(t.Data);
	m_StackFrame.clear();
	
	return -1;
}
