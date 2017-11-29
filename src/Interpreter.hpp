

#pragma once

#include "Grammar.hpp"
#include "Presets.hpp"
#include <sstream>
#include <iostream>

class CInterpreter
{
public:
	CInterpreter() :
		sfc{ PushNext, PushData, GetVariable }
	{
		m_pCurrentInterpreter = this;
	}
	int Execute(std::string ByteCode, CGrammarTable &gl);
private:
	static void PushNext()
	{
		//std::cout << "\nPushNext()";
		std::string temp;
		std::getline(m_pCurrentInterpreter->ss, temp, '#');
		m_pCurrentInterpreter->stackFrame.push_back({ new std::string(std::move(temp)), StringDeleter });
	}
	static void PushData(Variable_t v)
	{
		//std::cout << "\nPushData()";
		m_pCurrentInterpreter->stackFrame.push_back(v);
	}
	static Variable_t & GetVariable(int i)
	{
		return
			//&*(m_pCurrentInterpreter->stackFrame.end()-m_pCurrentInterpreter->stackFrameOffset+i)
			//&*(m_pCurrentInterpreter->stackFrame.end()-1)
			(m_pCurrentInterpreter->stackFrame[m_pCurrentInterpreter->stackFrameOffset+i]);
			//(*(m_pCurrentInterpreter->stackFrame.end() - m_pCurrentInterpreter->stackFrameOffset + i)).Data;
		;
	}
private:
	std::stringstream ss;
	StackFrameController sfc;
	int stackFrameOffset;
	std::vector<Variable_t> stackFrame;
	static CInterpreter	*m_pCurrentInterpreter;
};
