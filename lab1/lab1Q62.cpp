#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>

using namespace std;

bool issubset(set<int> &tans,set<int> &data)
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

vector<set<int>> genrated_candidate(vector<set<int>>&freqitems,int k)
{
	vector<set<int>>candideds;
	int len = freqitems.size();
	for(int i=0;i<len;i++)
	{
		for(int j = i+1;j<len;j++)
		{
			set<int>candidate = freqitems[i];
			candidate.insert(freqitems[j].begin(),freqitems[j].end());
			
			if(candidate.size() == k)
			{
				bool issubset_exits = false;
				for(auto exting: candideds)
				{
					
				if(issubset(exting,candidate))
					{
						issubset_exits = true;
						break;
					}
				}	
				
				if(!issubset_exits)
				{
					candideds.push_back(candidate);
				}
			}
		}
	}
	
	return candideds;
}

vector<set<int>> filter_candidate(vector<set<int>> &candideds,vector<set<int>>&tran,int min_sup)
{
	map<set<int>,int>sup_count;
	for(auto i: tran)
	{
		for(auto j : candideds)
		{
			if(issubset(i,j))
			{
				sup_count[j]++;
			}
		}
	}
	
	vector<set<int>>freqitem;
	for(auto i: sup_count)
	{
		if(i.second >= min_sup)
		{
			freqitem.push_back(i.first);
		}
	}
	return freqitem;
}

void apri_algo(vector<set<int>>&tran,int min_sup)
{
	ofstream file2;
	file2.open("1Q62.txt");
	map<int,int>itemcount;
	
	for(auto i: tran)
	{
		for(int j: i)
		{
			itemcount[j]++;
		}
	}
	vector<set<int>>freqitems;
	for(auto i: itemcount)
	{
		if(i.second >= min_sup)
		{
			freqitems.push_back({i.first});
			file2<<(char)(i.first+'a')<<endl;
		}
	}
	int k = 1;
	vector<set<int>>candideds;
	vector<set<int>>maxfreq;
	
	while(!freqitems.empty())
	{
		maxfreq = freqitems;
		candideds = genrated_candidate(freqitems,++k);
		freqitems = filter_candidate(candideds,tran,min_sup);
		for(auto i: freqitems)
		{
			for(int j: i)
			{
				file2<<(char)(j+'a')<<" ";
			}
		file2<<endl;
		}
	}

	file2.close();
	
	
	
	
}
int main()
{
	ifstream file1;
	file1.open("1Q5.txt");
	vector<set<int>>tran;
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
				temp = "";
			}	
			else{
				temp += str[i];
			}
			i++;
		}
		int x = temp[0] - 'a';
		st.insert(x);
		tran.push_back(st);		
	}

	apri_algo(tran,3);
	return 0;
}