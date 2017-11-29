
#include "VirtualMachine.hpp"
#include <iostream>

int main()
{
	std::string script = R"( Print("Hello Shreya" Print) )";
	std::cout << "\nScript is:\n" << script;
	
	CGrammarTable grammarList;
	grammarList.Add("Print",			Function_t{ "none", {"string", "string"}, &Print });
	grammarList.Add("AddNumber",		Function_t{ "number",{ "number", "number" }, &AddNumber });
	grammarList.Add("_GetString",		Function_t{ "string", {}, &_GetString });
	grammarList.Add("_GetNumber",		Function_t{ "number", {}, &_GetNumber });
	grammarList.Add("NumberToString",	Function_t{ "string", {"number"}, &NumberToString }, true);
	grammarList.Add("Shreya",			Function_t{ "string", {}, &Shreya});

	CVirtualMachine vm;
	vm.AttachGrammar(&grammarList);
	int i = vm.ExecuteScript(script);
	
	std::cout << "\nAlldone!";
	std::cin.get();
	return i;
}
