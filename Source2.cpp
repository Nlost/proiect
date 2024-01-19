#include<iostream>
#include<Windows.h>
#include<string.h>
#include<vector>
#include<fstream>

using namespace std;

class Class2;

class Class1
{
	string n;
	float p;
public:
	Class1(string n, float p)
	{
		this->n = n;
		this->p = p;
	}

	friend void show(Class1* c1, Class2* c2);
};

class Class2 
{
	int a;
	bool b;
public:
	Class2(int a, bool b)
	{
		this->a = a;
		this->b = b;
	}
};



void show(Class1* c1, Class2* c2)
{
	cout << c1->n;

}




int main() {

}