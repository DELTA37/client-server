#include <string>
#include <iostream>
#include "DataBase.hpp"

#define N (1000)
using namespace std;


int main(void)
{
	CStudent * s = new CStudent[100];
	CTable *t = new CTable();
	for (int i = 0; i < 40; i++)
	{
		s[i].name = std::to_string(i);
		s[i].rating = i%10;
		s[i].group = i;
		t->Add(&(s[i]));
	}
	t = t->Select(std::vector<int>({3,4,5, 10, 13}));
	std::cout << t->ToString({"name", "group"}, "name") << std::endl;
	return 0;
}
