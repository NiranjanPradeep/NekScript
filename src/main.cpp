
#include "VirtualMachine.hpp"
#include <iostream>

int main()
{
	CGrammarTable grammarList;
	grammarList.Add("Print",			Function_t{ "none", {"string", "string"}, &Print });
	grammarList.Add("AddNumber",		Function_t{ "number",{ "number", "number" }, &AddNumber });
	grammarList.Add("_GetString",		Function_t{ "string", {}, &_GetString });
	grammarList.Add("_GetNumber",		Function_t{ "number", {}, &_GetNumber });
	grammarList.Add("NumberToString",	Function_t{ "string", {"number"}, &NumberToString }, true);
	grammarList.Add("Shreya",			Function_t{ "string", {}, &Shreya});

	CVirtualMachine vm;
	vm.AttachGrammar(&grammarList);
	
	//std::string script = R"( Print("Hello Shreya" Shreya()) )";
	std::string script;

	int i = 0;
	std::cout << "\nEnter script:\n";
	while (std::getline(std::cin, script))
	{
		if (script.empty()) break;
		std::cout << "\nScript is:\n" << script;

		i = vm.ExecuteScript(script);
		script.clear();
		std::cout << "\nEnter script:\n";
	}
	std::cout << "\nAlldone!";
	std::cin.get();
	return i;
}
