#include "myArray.h"

myArray::myArray(initializer_list<int> list)
{
	this->n = list.size();//获取大小，开辟内存
	this->p = new int[this->n];
	int id = 0;
	for (auto i : list)
	{
		this->p[id] = i;//拷贝数据
		id++;
	}

}


myArray::~myArray()
{
}

void myArray::myArray::show()
{
	for (int i = 0; i < this->n; i++)
	{
		cout << this->p[i] << endl;
	}
}

void myArray::sort()
{
	for(int i=0;i<this->n-1;i++)
		for (int j = 0; j < this->n-1-j; j++)
		{
			if (p[j] > p[j + 1])
			{
				int temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
			}
		}
}

void myArray:: operator+(myArray & my)
{
	for (int i = 0; i < this->n; i++)
	{
		this->p[i] += my.p[i];
	}
}

