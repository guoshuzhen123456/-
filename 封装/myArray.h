#pragma once
#include<iostream>
#include<initializer_list>
using namespace std;

class myArray
{
	int *p;
	int n;//��ַ������
	
public: 
	myArray(initializer_list<int> list);
	~myArray();
	void show();//����
	void sort();
	void operator+(myArray &my);

};

