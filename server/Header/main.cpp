#include "DataBase.hpp"

int main(void)
{
	CStudent s[912];
	CDataBase data;
	for (int i = 0; i < 911; i++)
	{
        std::vector<char> vecname(7);
        for (size_t j = 0; j < 8; j++)
        {
            vecname[j] = (char)( (int)(random() % 26) + 'a');
        }
        std::string str(vecname.begin(), vecname.end());

		s[i].GetName() = str;
		s[i].GetRating() = i % 5;
		s[i].GetGroup() = 100 * (i % 3) + i % 20;
		data.Add(s[i]);
	}
	data.GetMainTable()->print();
	return 0;
}
