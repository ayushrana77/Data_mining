#include<sstream>
int main()
{
		ifstream file1;
	file1.open("input.txt");
	vector<set<int>>tran;
	string str;
	while(getline(file1,str))
	{
		int pos = str.find(' ');
		str = str.substr(pos+1);
		set<int>t;
		
		
	}

	part_algo(tran,2,2);
	return 0;
}