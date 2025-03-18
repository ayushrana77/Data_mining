#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

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

vector<set<int>> filter_candidate(vector<set<int>> &candideds,vector<set<int>>&tran,int min_sup,map<set<int>,int>&allsup_count)
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
		allsup_count[i.first] = i.second;
		if(i.second >= min_sup)
		{
			freqitem.push_back(i.first);
		}
	}
	return freqitem;
}

void find_max(vector<set<int>> &prev,vector<set<int>> &curr,map<set<int>,int>&allsup_count)
{
	vector<set<int>> not_closed_prev;
	for(auto item: curr)
	{
		set<int>temp;
		
		for(auto i: item)
		{
			temp = item;
			temp.erase(i);
			
			bool isexit = false;
			for(auto j: prev)
			{
				
				if(temp == j)
				{
					isexit = true;
					break;
				}
			}
			
			if(isexit)
			{
				not_closed_prev.push_back(temp);
			}
		}
	}
	

	for(auto i: not_closed_prev)
	{
	
		auto it = find(prev.begin(),prev.end(),i);
		if(it != prev.end())
		{
			prev.erase(it);
		}
	}

}

void max_algo(vector<set<int>>&tran,int min_sup)
{
	ofstream file2;
	file2.open("max_out.txt");
	map<int,int>itemcount;
	vector<set<int>>all_freq;
	map<set<int>,int>allsup_count;
	
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
			allsup_count.insert({{i.first},i.second});
		}
	}
	int k = 1;
	vector<set<int>>candideds;
	vector<set<int>>maxfreq;
	
	while(!freqitems.empty())
	{
		maxfreq = freqitems;
		candideds = genrated_candidate(freqitems,++k);
		freqitems = filter_candidate(candideds,tran,min_sup,allsup_count);
		find_max(maxfreq,freqitems,allsup_count);
		
		for(auto i: maxfreq)
		{
			all_freq.push_back(i);
		}
	}
	
	file2<<"Maxiume item sets :"<<endl;
	for(auto i: all_freq)
	{
		file2<<"{";
		for(auto j: i)
		{
			file2<<(char)(j+'a')<<" ";
		}
		file2<<"} support count:"<<allsup_count[i]<<endl;
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

	max_algo(tran,2);
	return 0;
}