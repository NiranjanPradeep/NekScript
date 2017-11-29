
#include "VirtualMachine.hpp"
#include <iostream>

CGrammarTable GetBasicGrammar()
{
	CGrammarTable grammarList;

	grammarList.Add("Print",		Function_t{ "void",{ "string"}, &Print });
	grammarList.Add("Add",			Function_t{ "number",{ "number", "number" }, &AddNumber });
	grammarList.Add("Concatenate",	Function_t{ "string",{ "string", "string" }, &AddString });
	grammarList.Add("_GetString",	Function_t{ "string",{}, &_GetString });
	grammarList.Add("_GetNumber",	Function_t{ "number",{}, &_GetNumber });
	grammarList.Add("NumberToString", Function_t{ "string",{ "number" }, &NumberToString }, true);
	grammarList.Add("StringToNumber", Function_t{ "number",{ "string" }, &StringToNumber }, true);

	return grammarList;
}

int main()
{
	CGrammarTable grammarList = GetBasicGrammar();
	grammarList.Add("Shreya", Function_t{ "string",{}, &Shreya });
	grammarList.Display();

	CVirtualMachine vm;
	vm.AttachGrammar(&grammarList);

	std::string script;

	while (true)
	{
		std::cout << "\nEnter script:\n";
		std::getline(std::cin, script);
		if (script.empty()) break;
		vm.ExecuteScript(script);
	}
	std::cout << "\nAlldone!";
	std::cin.get();
}
