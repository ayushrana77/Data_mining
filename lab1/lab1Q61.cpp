#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>

using namespace std;

void genrate_sub_seq(vector<vector<int>>&subseq,vector<int>&seq,vector<int>&item,int i)
{
	if(i == item.size())
	{
		if(!seq.empty())
		{
			subseq.push_back(seq);
		}
		return;
	}
	seq.push_back(item[i]);
	genrate_sub_seq(subseq,seq,item,i+1);
	seq.pop_back();
	genrate_sub_seq(subseq,seq,item,i+1);
}

bool isfill(vector<int> &data,set<int> &tans)
{
	for(int i: data)
	{
		if(tans.find(i) == tans.end())
		{
			return false;
		}
	}
	return true;
}
void own_algo(vector<set<int>>&tran,vector<int>&item,int min_sup)
{
	vector<vector<int>>subseq;
	vector<int>seq;
	genrate_sub_seq(subseq,seq,item,0);
	for(auto i: subseq)
	{
		for(int j: i)
		{
			cout<<(char)(j+'a')<<" ";
		}
		cout<<endl;
	}
	vector<vector<int>>freq;
	
	for(auto i: subseq)
	{
		int frequ = 0;
		for(auto j: tran)
		{
			if(isfill(i,j))
			{
				frequ++;
			}
		}
		if(frequ >= min_sup)
		{
			freq.push_back(i);
		}
	}
	ofstream file2;
	file2.open("1Q61.txt");
	for(auto i: freq)
	{
	
		for(int j: i)
		{
			file2<<(char)(j+'a')<<" ";
		}
		file2<<endl;
		
	}
	file2.close();
	return;
	
}
int main()
{
	ifstream file1;
	file1.open("1Q5.txt");
	vector<set<int>>tran;
	set<int> item;
	string str;
	while(getline(file1,str))
	{
		int i = 0;
		while(str[i] != ' ')
		{
			i++;
		}
		i++;
		string temp = "";
		set<int>st;
		
		while(i < str.size())
		{
			if(str[i] == ' ')
			{
				int x = temp[0] - 'a';
				st.insert(x);
				item.insert(x);
				temp = "";
			}	
			else{
				temp += str[i];
			}
			i++;
		}
		int x = temp[0] - 'a';
		st.insert(x);
		item.insert(x);
		tran.push_back(st);		
	}
	vector<int>arr;
	for(int i:item)
	{
		arr.push_back(i);
	}

	own_algo(tran,arr,3);
	return 0;
}