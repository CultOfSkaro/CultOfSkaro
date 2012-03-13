#pragma once

#include "stdafx.h"

using std::deque;
using std::string;

class Text
{
public:
	Text(int max_num_strings);
	~Text(void);
	
	void	clear();
	void	add(string str);
	size_t	size();
	string	get();

protected:
	size_t			m_max_num_strings;
	deque<string>	m_strings;	
};
