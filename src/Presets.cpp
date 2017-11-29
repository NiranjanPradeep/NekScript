
#include "Presets.hpp"

#include "Grammar.hpp"
#include <string>
#include <iostream>
#include <sstream>

void StringDeleter(void *& Data)
{
	delete (std::string *)Data;
}

void NumberDeleter(void *& Data)
{
	delete (float *)Data;
}

// Type Casting
void StringToNumber(StackFrameController * pSfc)
{
	float num = std::stof(pSfc->GetValue<std::string>(0));
	pSfc->PushData({ new float(num), NumberDeleter });
}

void NumberToString(StackFrameController * pSFc)
{
	std::cout << "\nNumberToString()";
	std::string num = std::to_string(pSFc->GetValue<float>(0));
	pSFc->PushData({ new std::string(std::move(num)), StringDeleter });
}

// Library Functions
void Print(StackFrameController * pSfc)
{
	std::string str = pSfc->GetValue<std::string>(0) + pSfc->GetValue<std::string>(1);
	std::cout << "\nPrint() : " << str;
	pSfc->PushData({ new std::string(std::move(str)), StringDeleter});
}

// System Functions
void _GetString(StackFrameController * pSfc)
{
	std::cout << "\n_GetString()";
	pSfc->PushNext();
}

void _GetNumber(StackFrameController * pSfc)
{
	std::cout << "\n_GetNumber()";
	pSfc->PushNext();
	Variable_t *p = &pSfc->GetVariable(0);
	auto num = *reinterpret_cast<float *>(&pSfc->GetValue<std::string>(0)[0]);
	p->Deleter(p->Data);
	p->Data = new float(num);
	p->Deleter = NumberDeleter;
	std::cout << "\nBlob = " << num;
}