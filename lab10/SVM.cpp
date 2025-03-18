#include<bits/stdc++.h>

using namespace std;

void svm(vector<vector<int>> data)
{
	for(auto i: data)
	{
		for(auto j: i)
		{
			cout<<j<<" ";
		}
		cout<<endl;
	}
}

int main()
{
	ifstream file1;
	file1.open("input2.txt");
	string str;
	vector<vector<int>>data;
	while(getline(file1,str))
	{
		stringstream ss(str);
		int x;
		vector<int>temp;
		while(ss >> x)
		{
			temp.push_back(x);
		}
		data.push_back(temp);
	}
	
	svm(data);
	return 0;
}