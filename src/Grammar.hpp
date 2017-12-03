
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <iostream>

struct Variable_t
{
	void		*Data;
	void		(*Deleter)(void *&);
};

struct StackFrameController
{
	std::function <void()>				PushNext;
	std::function <void(Variable_t)>	PushData;
	std::function <Variable_t &(int)>	GetVariable;

	template <typename T>
	T &GetValue(int i)
	{
		return *static_cast<T *>(GetVariable(i).Data);
	}
};

struct Function_t
{
	std::string					return_type;
	std::vector<std::string>	argument_types;
	std::function<void(StackFrameController *)> function_ptr;
};

struct CastHash
{
	std::size_t operator () (const std::pair<std::string, std::string> &p) const
	{
		return std::hash<std::string>{}(p.first + p.second);
	}
};

class CGrammarTable
{
public:
	bool				IsFunction(const std::string &s);
	std::string			IsCastable(const std::string &from, const std::string &to);
	std::string			HasOperator(const std::string & type, const std::string & op);

	void				Add(const std::string &Name, Function_t &&func, std::string Type = std::string("regular"));
	void				Display();
	const Function_t &	Get(const std::string &s) { return m_Table[s]; }

private:

private:
	using Table_t		= std::unordered_map <std::string, Function_t >;
	using CastingPair_t = std::unordered_map <std::pair<std::string, std::string >, std::string, CastHash >;
	using Operator_t	= std::unordered_map <std::pair<std::string, std::string >, std::string, CastHash >;

	Table_t			m_Table;
	CastingPair_t	m_TableCaster;
	Operator_t		m_TableOperator;
};
