#include "StdAfx.h"
#include "Text.h"

using std::vector;
using std::string;

Text::Text(int max_num_strings)
{
	m_max_num_strings = max_num_strings;
}

Text::~Text(void){}

void Text::clear()
{
	m_strings.clear();
}

void Text::add(string str)
{
	//printf(str.c_str());

	if(m_strings.size() >= m_max_num_strings)
		m_strings.clear();

	m_strings.push_back(str);
}

size_t Text::size()
{
	return m_strings.size();
}

string Text::get()
{
	string return_value = m_strings.front();
	m_strings.pop_front();
	return return_value;
}