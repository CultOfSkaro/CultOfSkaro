#pragma once

#include "stdafx.h"

using std::vector;
using std::string;

template <typename T>
class Register
{
public:
	Register(){}
	~Register(){}

	void clear()
	{
		m_data.clear(); 
		m_strings.clear();
	}
	
	size_t size()
	{
		return m_strings.size();
	}

	void addString(string str)
	{
		m_strings.push_back(str);
		m_data.resize(m_strings.size());
	}

	void setData(size_t index, T data)
	{
		m_data.at(index) = data;
	}
	
	void setAllData(size_t number, T* data)
	{	
		if(number > m_data.size())
		{
			m_strings.assign(number,string("EMPTY"));
			m_data.resize(number);
		}

		for(size_t i = 0 ; i < number ; i++)
			m_data.at(i) = data[i];
	}
	
	T getData(size_t index)
	{
		return m_data.at(index);
	}
	
	string getString(size_t index)
	{
		return m_strings.at(index);		
	}

protected:
	vector<T>		m_data;
	vector<string>	m_strings;
};



//////////////////////////////////////////////////////
template <>
class Register <uint32>
{
public:
	Register(){}
	~Register(){}

	void clear()	
	{
		m_data.clear(); 
		m_strings.clear();
	}
	
	size_t size()
	{
		return m_strings.size();
	}	

	void addString(string str)
	{
		m_strings.push_back(str);
		m_data.resize(m_strings.size());
	}

	void setData(size_t index, uint32 data)
	{
		m_data.at(index) = data;
	}

	void setAllData(size_t number, uint32* data)
	{
		if(number > m_data.size())
		{
			m_strings.assign(number,string("EMPTY"));
			m_data.resize(number);
		}

		fint fi;
		for(size_t i = 0 ; i < number ; i++)
		{			
			fi.u = data[i];
			fi.u = fi.c[0] << 24 | fi.c[1] << 16 | fi.c[2] << 8 | fi.c[3];
			m_data.at(i) = fi.i;
		}
	}
	
	uint32 getData(size_t index)
	{
		return m_data.at(index);
	}

	string getString(size_t index)
	{
		return m_strings.at(index);
	}

protected:
	vector<uint32>	m_data;
	vector<string>	m_strings;
};




///////////////////////////////////////////////////////
template <>
class Register <int>
{
public:
	Register(){}
	~Register(){}

	void clear()	
	{
		m_data.clear(); 
		m_strings.clear();
	}
	
	size_t size()
	{
		return m_strings.size();
	}	

	void addString(string str)
	{
		m_strings.push_back(str);
		m_data.resize(m_strings.size());
	}

	void setData(size_t index, int data)
	{
		m_data.at(index) = data;
	}

	void setAllData(size_t number, int* data)
	{
		if(number > m_data.size())
		{
			m_strings.assign(number,string("EMPTY"));
			m_data.resize(number);
		}

		fint fi;
		for(size_t i = 0 ; i < number ; i++)
		{			
			fi.i = data[i];
			fi.u = fi.c[0] << 24 | fi.c[1] << 16 | fi.c[2] << 8 | fi.c[3];
			m_data.at(i) = fi.i;
		}
	}
	
	int getData(size_t index)
	{
		return m_data.at(index);
	}

	string getString(size_t index)
	{
		return m_strings.at(index);
	}

protected:
	vector<int>		m_data;
	vector<string>	m_strings;
};





////////////////////////////////////////////////////////
template <>
class Register <float>
{
public:
	Register(){}
	~Register(){}

	void clear()	
	{
		m_data.clear(); 
		m_strings.clear();
	}
	
	size_t size()
	{
		return m_strings.size();
	}	

	void addString(string str)
	{
		m_strings.push_back(str);
		m_data.resize(m_strings.size());
	}

	void setData(size_t index, float data)
	{
		m_data.at(index) = data;
	}

	void setAllData(size_t number, float* data)
	{
		if(number > m_data.size())
		{
			m_strings.assign(number,string("EMPTY"));
			m_data.resize(number);
		}

		fint fi;
		for(size_t i = 0 ; i < number ; i++)
		{			
			fi.f = data[i];
			fi.u = fi.c[0] << 24 | fi.c[1] << 16 | fi.c[2] << 8 | fi.c[3];
			m_data.at(i) = fi.f;
		}
	}
	
	float getData(size_t index)
	{
		return m_data.at(index);
	}

	string getString(size_t index)
	{
		return m_strings.at(index);
	}

protected:
	vector<float>	m_data;
	vector<string>	m_strings;
};




