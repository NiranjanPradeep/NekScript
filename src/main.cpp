
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
	grammarList.Add("Shreya",		Function_t{ "string",{}, &Shreya });
	return grammarList;
}

struct Player_t
{
	std::string Name;
	int ID;
	static int Count;
	Player_t() : ID(Count), Name(std::string("Name ")+std::to_string(Count))
	{
		Count++;
	}
}playerList[100];

int Player_t::Count = 0;

void PlayerDeleter(void *&data)
{
	delete (Player_t *)data;
}

void PlayerToString(StackFrameController *pSfc)
{
	Player_t &p = pSfc->GetValue<Player_t>(0);
	pSfc->PushData({ new std::string(p.Name), &StringDeleter });
}

void GetPlayer(StackFrameController *pSfc)
{
	int index = pSfc->GetValue<float>(0);
	pSfc->PushData({ new Player_t(playerList[index]), &PlayerDeleter });
}

int main()
{
	CGrammarTable grammarList = GetBasicGrammar();
	grammarList.Add("PlayerToString", Function_t{ "string", {"player"}, &PlayerToString }, true);
	grammarList.Add("GetPlayer", Function_t{ "player",{ "number" }, &GetPlayer});
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
