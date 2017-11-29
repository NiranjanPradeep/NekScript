
#include "Interpreter.hpp"
#include <iostream>

CInterpreter *CInterpreter::m_pCurrentInterpreter = nullptr;

int CInterpreter::Execute(std::string ByteCode, CGrammarTable & gl)
{
	std::string current;
	ss.clear();
	ss.str(ByteCode);
	while (std::getline(ss, current, '#'))
	{
		//std::cout << "\nCurrent word = " << current;
		auto &func = gl.Get(current);

		stackFrameOffset = stackFrame.size()-func.expected_type.size();
		if(func.function_ptr)
			func.function_ptr(&m_pCurrentInterpreter->sfc);
		stackFrame.erase(stackFrame.begin() + stackFrameOffset, stackFrame.begin() + stackFrameOffset + func.expected_type.size());
		/*
		for (auto &t : func.expected_type)
		{
			if(stackFrame.back().Deleter)
				stackFrame.back().Deleter(stackFrame.back().Data);
			stackFrame.pop_back();
		}
		*/
		//std::cout << "\nStack frame is : "; for (auto &t : stackFrame) std::cout << *(std::string *)t.Data << "@";
	}
	for (auto &t : stackFrame)
		t.Deleter(t.Data);
	stackFrame.clear();
	// std::cout << "\nStack frame is : "; for (auto &t : stackFrame) std::cout << *(std::string *)t.Data << " ";
	return -1;
}
