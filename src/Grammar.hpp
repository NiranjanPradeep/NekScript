
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <iostream>

struct Variable_t
{
	void *Data;
	void(*Deleter)(void *&);
};


struct StackFrameController
{
	void(&PushNext)();
	void(&PushData)(Variable_t);
	Variable_t &(&GetVariable)(int i);

	template <typename T>
	T &GetValue(int i)
	{
		return *static_cast<T *>(GetVariable(i).Data);
	}
};

struct Function_t
{
	std::string return_type;
	std::vector<std::string> expected_type;
	std::function<void(StackFrameController *)> function_ptr;
};

class CGrammarTable
{
public:
	struct CastHash
	{
		std::size_t operator () (const std::pair<std::string, std::string> &p) const
		{
			//std::cout << "\nHashed : " << std::hash<std::string>{}(p.first + p.second);
			return std::hash<std::string>{}(p.first + p.second);
		}
	};
	bool IsFunction(const std::string &s)
	{
		if (m_Table.find(s) == m_Table.end()) return false;
		else return true;
	}
	std::string IsCastable(const std::string &from, const std::string &to)
	{
		if (m_TableCaster.find({ from, to }) == m_TableCaster.end()) return "";
		else
		{
			//std::cout << "\nReturning function with name = " << m_TableCaster[{from, to}];
			return m_TableCaster[{from, to}];
		}
	}

	void Add(const std::string &Name, Function_t &&func, bool TypeCaster = false)
	{
		//std::cout << "\nAdding function with Name = " << Name;
		if (TypeCaster)
		{
			auto t = std::make_pair(func.expected_type[0], func.return_type);
			m_TableCaster.insert(
				std::make_pair(
					t,
					Name
				)
			);
		}
		m_Table.insert(std::make_pair(Name, std::move(func)));
	}
	
	const Function_t & Get(const std::string &s)
	{
		if(IsFunction(s)) return m_Table[s];
	}
private:

private:
	using Table_t = std::unordered_map <std::string, Function_t >;
	using CastingPair_t = std::unordered_map <std::pair<std::string, std::string >, std::string, CastHash >;
	Table_t			m_Table;
	CastingPair_t	m_TableCaster;
};
