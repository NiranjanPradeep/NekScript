
#include "VirtualMachine.hpp"
#include <iostream>

int main()
{
	std::string script = R"( Print ("Number is" 50) )";
	std::cout << "\nScript is:\n" << script;
	
	CGrammarTable grammarList;
	grammarList.Add("Print",		Function_t{ "string", {"string", "string"}, &Print });
	grammarList.Add("_GetString",	Function_t{ "string", {}, &_GetString });
	grammarList.Add("_GetNumber",	Function_t{ "number", {}, &_GetNumber });
	grammarList.Add(Function_t{ "string", {"number"}, &NumberToString });
	grammarList.Get("numberTostring");

	CVirtualMachine vm;
	vm.AttachGrammar(&grammarList);
	int i = vm.ExecuteScript(script);
	
	std::cout << "\nAlldone!";
	std::cin.get();
	return i;
}
