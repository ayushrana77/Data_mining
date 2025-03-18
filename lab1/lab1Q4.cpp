#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>

using namespace std;

int main()
{

	map<int,vector<int>>mp;
	fstream file1,file2;
	file1.open("tb.txt",ios::in);
	file2.open("1Q4.txt",ios::out);
	string str;
	vector<vector<char>>arr(5);
	
	while(getline(file1,str))
	{
		int x = str[1] - '0';
		for(int i= 3;i<str.size();i=i+2)
		{
			if(mp.find(str[i]) == mp.end())
			{
				vector<int>temp;
				mp[str[i]] = temp;
			}
			mp[str[i]].push_back(x);
		}

	}
	file1.close();
	
	for(auto i: mp)
	{
		file2 <<(char)i.first ;
		for(auto j: i.second)
		{

			file2<<" T"<<j;
		}
		file2<<endl;
	}
	
	
	file2.close();
	
	return 0;
}