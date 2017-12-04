
#include "VirtualMachine.hpp"
#include <iostream>

CGrammarTable GetBasicGrammar()
{
	CGrammarTable grammarList;

	grammarList.Add("Print",		Function_t{ "void",{ "string"}, &Print });
	grammarList.Add("Add",			Function_t{ "number",{ "number", "number" }, &AddNumber },		"operator +");
	grammarList.Add("Subtract",		Function_t{ "number",{ "number", "number" }, &SubtractNumber }, "operator -");
	grammarList.Add("Multiply",		Function_t{ "number",{ "number", "number" }, &MultiplyNumber }, "operator *");
	grammarList.Add("Divide",		Function_t{ "number",{ "number", "number" }, &DivideNumber },	"operator /");
	grammarList.Add("Concatenate",	Function_t{ "string",{ "string", "string" }, &AddString },		"operator +");
	grammarList.Add("_GetString",	Function_t{ "string",{}, &_GetString });
	grammarList.Add("_GetNumber",	Function_t{ "number",{}, &_GetNumber });
	grammarList.Add("NumberToString", Function_t{ "string",{ "number" }, &NumberToString },			"typecast");
	grammarList.Add("StringToNumber", Function_t{ "number",{ "string" }, &StringToNumber },			"typecast");

	return grammarList;
}

int main()
{
	CGrammarTable grammarList = GetBasicGrammar();
	grammarList.Display();
	grammarList.Add("Shreya", Function_t{ "string",{}, &Shreya });

	CVirtualMachine vm;
	vm.AttachGrammar(&grammarList);

	std::string script;

	while (true)
	{
		std::cout << "\n\nEnter script:\n";
		std::getline(std::cin, script);
		if (script.empty()) break;
		vm.ExecuteScript(script);
	}

	std::cout << "\nAlldone!";
	std::cin.get();
}
