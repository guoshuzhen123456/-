#include<iostream>
#include<initializer_list>
#include"myArray.h"

using namespace std;

int main()
{
	int a[5] = {1,2,3,4,5};
	//myArray my{11,23,4,15};
	myArray my{1,2,3,4,5};
	my.sort();
	my.show();
	myArray *p = new myArray{11,12,13,14,15};
	my + (*p);
	my.show();


	cin.get();
}

