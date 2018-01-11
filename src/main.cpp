
#include "VirtualMachine.hpp"
#include <iostream>

CGrammarTable GetBasicGrammar() 
{
	CGrammarTable grammarList;

	grammarList.Add("Print", Function_t{ "void",{ "string"}, &Print });
	grammarList.Add("Add", Function_t{ "number",{ "number", "number" }, &AddNumber }, "operator +");
	grammarList.Add("Subtract", Function_t{ "number",{ "number", "number" }, &SubtractNumber }, "operator -");
	grammarList.Add("Multiply", Function_t{ "number",{ "number", "number" }, &MultiplyNumber }, "operator *");
	grammarList.Add("Divide", Function_t{ "number",{ "number", "number" }, &DivideNumber }, "operator /");
	grammarList.Add("Concatenate", Function_t{ "string",{ "string", "string" }, &AddString }, "operator +");
	grammarList.Add("_GetString", Function_t{ "string",{}, &_GetString });
	grammarList.Add("_GetNumber", Function_t{ "number",{}, &_GetNumber });
	grammarList.Add("NumberToString", Function_t{ "string",{ "number" }, &NumberToString }, "typecast");
	grammarList.Add("StringToNumber", Function_t{ "number",{ "string" }, &StringToNumber }, "typecast");

	return grammarList;
}

struct Player_t
{
	std::string Name;
	int ID;
};

std::vector<Player_t> g_PlayerList;

void Player_Creator(StackFrameController *pSfc)
{
	auto &name = pSfc->GetValue<std::string>(0);
	g_PlayerList.push_back({ name, static_cast<int>(g_PlayerList.size()) + 1 });
}

void Player_Show(StackFrameController *pSfc)
{
	for (auto &t : g_PlayerList)
		std::cout << '\n' << t.Name << "(" << t.ID << ")";
}

int main()
{
	CGrammarTable grammarList = GetBasicGrammar();
	
	grammarList.Add("Shreya", Function_t{ "string",{}, &Shreya });
	grammarList.Add("CreatePlayer", Function_t{ "player", {"string"}, &Player_Creator });
	grammarList.Add("Show", Function_t{ "void",{  }, &Player_Show });
	grammarList.Display();

	CVirtualMachine vm;
	vm.AttachGrammar(&grammarList);

	// Print(NumberToString(123) + "hah")

	while (true)
	{
		std::string script;
		std::string line;

		std::cout << "\n\nEnter script:\n";
		while (true)
		{
			char ch = std::cin.get();
			if (ch == '#') break;
			else if (ch == '\n') continue;
			else script.push_back(ch);
		}
		if (script.empty()) break;
		vm.ExecuteScript(script);
	}

	std::cout << "\nAlldone!";
	std::cin.get();
}
