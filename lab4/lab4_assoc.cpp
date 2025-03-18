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

void genrate_sub_seq(vector<vector<int>>&subseq,vector<int>&seq,vector<int>&item,int i)
{
	if(i == item.size())
	{
		if(!seq.empty() && !(item.size() == seq.size()))
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
				allsup_count[j]++;
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

void apri_algo(vector<set<int>>&tran,int min_sup,int conf)
{
	ofstream file2;
	file2.open("lab4_out.txt");
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
			all_freq.push_back({i.first});
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
		for(auto i: freqitems)
		{
			all_freq.push_back(i);
		}
	}

	for(auto i: all_freq)
	{
		if(i.size() == 1)
		{
			continue;
		}
		vector<set<int>>confup;
		vector<int>freq;
	
		for(int item: i)
		{
			
			freq.push_back(item);
		}
		
		vector<vector<int>>subseq;
		vector<int>seq;
		genrate_sub_seq(subseq,seq,freq,0);
		for(auto k: subseq)
		{
			set<int>temp;
			for(int j: k)
			{
				temp.insert(j);
			}
			set<int>diff;
			set_difference(i.begin(),i.end(),temp.begin(),temp.end(),inserter(diff,diff.begin()));
			
			
			
			
			double currconf = (double)allsup_count[i]/(double)allsup_count[temp];
			currconf *= 100;
			if(currconf >=  conf)
			{
				bool isexit = false;
				for(auto i: confup)
				{
					if(temp == i)
					{
						isexit = true;
						break;
					}
				}
				if(!isexit)
				{
					file2<<"{ ";
					for(int j: k)
					{
						file2<<(char)(j+'a')<<" ";
					}
					file2<<"} -->";
					file2<<"{ ";
					for(int j: diff)
					{
						file2<<(char)(j+'a')<<" ";
					}
					file2<<"}";
					file2<<" confidence "<<currconf;
					file2<<endl;
				}
				confup.push_back(temp);
			}
			
		}
		
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

	apri_algo(tran,4,50);
	return 0;
}