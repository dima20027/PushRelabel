#pragma once
#include<iostream>
#include<stdio.h>
using namespace std;

template<typename T>
class OURvector
{
public:

	OURvector()
	{
		arr = nullptr;
		sizeArr = 0;
	}
	~OURvector()
	{
		delete[]arr;
	}

	void push_back(T data)
	{
		if (arr == nullptr)
		{
			sizeArr = sizeArr + 1;
			arr = new T[sizeArr];
			arr[sizeArr - 1] = data;
		}
		else
		{
			sizeArr++;
			T* tmp;
			tmp = (T*) realloc(arr, sizeArr * sizeof(T));
			if (tmp != nullptr)
			{
				arr = tmp;
				arr[sizeArr - 1] = data;
			}

		}
	}
	T& operator[](const int index)
	{
		return arr[index];
	}

	int size()
	{
		return sizeArr;
	}

	T back()
	{
		if (sizeArr == 0)
		{
			throw out_of_range("vector size equal 0!");
		}
		return arr[sizeArr - 1];
	}

private:
	T* arr;
	int sizeArr;
};

