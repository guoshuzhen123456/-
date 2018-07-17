#pragma once
#include<iostream>
#include<initializer_list>
using namespace std;

class myArray
{
	int *p;
	int n;//地址，长度
	
public: 
	myArray(initializer_list<int> list);
	~myArray();
	void show();//声明
	void sort();
	void operator+(myArray &my);

};

