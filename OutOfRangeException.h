#pragma once
#include <exception>

#ifndef OOP_LR_2_OutOfRangeException_h
#define OOP_LR_2_OutOfRangeException_h

class OutOfRangeException : public std::exception
{
private:
	int trying_to_get;
	int size_of_obj;


public:
	OutOfRangeException(const char* _Exception, int _trying_to_get, int _of_size) :exception(_Exception)
	{
		trying_to_get = _trying_to_get;
		size_of_obj = _of_size;
	}

	int GetSize()
	{
		return size_of_obj;
	}

	int GetTrying()
	{
		return trying_to_get;
	}

};




#endif