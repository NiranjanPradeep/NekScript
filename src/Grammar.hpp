
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
			//// took this from http://en.cppreference.com/w/cpp/utility/hash
			//auto h1 = std::hash<std::string>{}(p.first);
			//auto h2 = std::hash<std::string>{}(p.second);

			//return h1 ^ h2;
			std::cout << "\nHashed : " << std::hash<std::string>{}(p.first + p.second);
			return std::hash<std::string>{}(p.first + p.second);
		}
	};
	bool IsFunction(const std::string &s)
	{
		if (m_Table.find(s) == m_Table.end()) return false;
		else return true;
	}
	bool IsCastable(const std::string &from, const std::string &to)
	{
		if (m_TableCaster.find({ from, to }) == m_TableCaster.end()) return false;
		else return true;
	}

	void Add(const std::string &Name, Function_t &&func)
	{
		m_Table.insert(std::make_pair(Name, std::move(func)));
	}
	
	void Add(Function_t &&func)
	{
		auto t = std::make_pair(func.expected_type[0]+"To", func.return_type);
		m_TableCaster.insert(
			std::make_pair(
				t,
				func
			)
		);
		this->Get(func.expected_type[0] + "To" + func.return_type);
	}

	const Function_t & Get(const std::string &s)
	{
		if(IsFunction(s)) return m_Table[s];

			return m_TableCaster[std::make_pair(s, std::string{})];
	}
private:

private:
	using Table_t = std::unordered_map <std::string, Function_t >;
	using TableCaster_t = std::unordered_map <std::pair<std::string, std::string >, Function_t, CastHash >;
	Table_t			m_Table;
	TableCaster_t	m_TableCaster;
};
