#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cmath>

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

vector<set<int>> filter_local_candidate(vector<set<int>> &candideds,vector<set<int>>&tran,int st,int ed,int loacl_sup)
{
	map<set<int>,int>sup_count;
	for(int i= st;i<ed;i++)
	{
		for(auto j : candideds)
		{
			if(issubset(tran[i],j))
			{
				sup_count[j]++;
			}
		}
	}
	
	vector<set<int>>freqitem;
	for(auto i: sup_count)
	{
		if(i.second >= loacl_sup)
		{
			freqitem.push_back(i.first);
		}
	}
	return freqitem;
}

vector<set<int>> loacl_partion(vector<set<int>> &tran,int st,int ed,int loacl_sup)
{
	vector<set<int>>freqitems;
	vector<set<int>>all_freqitemsets;
	map<int,int>itemcount;
	
	for(int i= st;i<ed;i++)
	{
		for(int j: tran[i])
		{
			itemcount[j]++;
		}
	}
	
	for(auto i: itemcount)
	{
		if(i.second >= loacl_sup)
		{
			freqitems.push_back({i.first});
			all_freqitemsets.push_back({i.first});
		}
	}
	int k = 1;
	vector<set<int>>candideds;
	vector<set<int>>maxfreq;
	
	while(!freqitems.empty())
	{
		maxfreq = freqitems;
		candideds = genrated_candidate(freqitems,++k);
		freqitems = filter_local_candidate(candideds,tran,st,ed,loacl_sup);
		for(auto i: freqitems)
		{
			all_freqitemsets.push_back(i);
		}
	}
	
	return all_freqitemsets;
}

void part_algo(vector<set<int>>&tran,int min_sup,int part_size)
{
	ofstream file2;
	file2.open("part_out.txt");
	vector<set<int>>freqitemsets;
	int n = tran.size();
	int local_sup = ceil(((double)min_sup*(double)part_size) / (double)n);
	
	for(int i=0;i<n;i= i+part_size)
	{
		int x = i/part_size;
		x++;
		file2<<" local frequent in partion "<<x<<endl;
		vector<set<int>> local_freq = loacl_partion(tran,i,i+part_size,local_sup);
		for(auto curr_loacl : local_freq)
		{
			file2<<" {";
			for(int j: curr_loacl)
			{
				file2<<(char)(j+'a')<<" ";
			}
			file2<<"}";
			file2<<endl;
			bool isexit = false;
			for(auto  prev_item: freqitemsets)
			{
				if(prev_item == curr_loacl)
				{
					isexit = true;
					break;
				}
			}
			if(!isexit)
			{
				freqitemsets.push_back(curr_loacl);
			}
		}
	}

	freqitemsets = filter_candidate(freqitemsets,tran,min_sup);
	
	file2<<" Frequent item sets:";
	file2<<endl;
	for(auto i: freqitemsets)
	{
		file2<<" {";
		for(int j: i)
		{
			file2<<(char)(j+'a')<<" ";
		}
		file2<<"}";
		file2<<endl;
	}
	
	file2.close();
	
}


int main()
{
	ifstream file1;
	file1.open("input.txt");
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

	part_algo(tran,3,3);
	return 0;
}