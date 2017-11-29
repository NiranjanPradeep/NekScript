

#include "Grammar.hpp"

bool CGrammarTable::IsFunction(const std::string & s)
{
	if (m_Table.find(s) == m_Table.end()) return false;
	else return true;
}

std::string CGrammarTable::IsCastable(const std::string & from, const std::string & to)
{
	if (m_TableCaster.find({ from, to }) == m_TableCaster.end()) return "";
	else
	{
		//std::cout << "\nReturning function with name = " << m_TableCaster[{from, to}];
		return m_TableCaster[{from, to}];
	}
}

void CGrammarTable::Add(const std::string & Name, Function_t && func, bool TypeCaster)
{
	//std::cout << "\nAdding function with Name = " << Name;
	if (TypeCaster)
	{
		auto t = std::make_pair(func.argument_types[0], func.return_type);
		m_TableCaster.insert(
			std::make_pair(
				t,
				Name
			)
		);
	}
	m_Table.insert(std::make_pair(Name, std::move(func)));
}

void CGrammarTable::Display()
{
	std::cout << "\nFunctions registered: ";
	for (auto &t : m_Table)
	{
		std::cout << std::endl << t.second.return_type << "\t" << t.first << "( ";
		for (auto &t2 : t.second.argument_types)
			std::cout << t2 << " ";
		std::cout << ")";
	}
}
