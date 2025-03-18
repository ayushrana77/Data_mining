#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>

using namespace std;

int main()
{

	
	fstream file1;
	file1.open("1Qans5.txt",ios::out);
	int n = 10;
	srand(0);
	int x = rand();
	for(int i=1;i<=x;i++)
	{
		file1 <<"T"<<i<<" ";
		int y = rand()%n;
		y++;
		set<int>st;
		while(st.size() < y)
		{
			st.insert((rand()%n)+1);
		}
		for(auto i: st)
		{
			file1<<i<<" ";
		}
		file1<<endl;
	}
	
	file1.close();
	
	return 0;
}