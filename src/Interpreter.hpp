

#pragma once

#include "Grammar.hpp"
#include "Presets.hpp"
#include <sstream>
#include <iostream>

class CInterpreter
{
public:
	CInterpreter() 
	{
		m_Sfc.PushNext		= [this]() {PushNext();};
		m_Sfc.PushData		= [this](Variable_t t) {PushData(t); };
		m_Sfc.GetVariable	= [this](int i)->Variable_t & {return GetVariable(i); };
	}
	int Execute(std::string ByteCode, CGrammarTable &gl);
private:
	void PushNext()
	{
		//std::cout << "\nPushNext()";
		std::string temp;
		std::getline(m_ss, temp, '#');
		m_StackFrame.push_back({ new std::string(std::move(temp)), StringDeleter });
	}
	void PushData(Variable_t v)
	{
		//std::cout << "\nPushData()";
		m_StackFrame.push_back(v);
	}
	Variable_t & GetVariable(int i)
	{
		return (m_StackFrame[m_StackFrameOffset+i]);
	}
private:
	std::stringstream		m_ss;
	StackFrameController	m_Sfc;
	int						m_StackFrameOffset;
	std::vector<Variable_t> m_StackFrame;
};
