#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

using namespace std;

int hashfunction(set<int>curr,int bucketCount)
{
	int sum = 0;
	for(auto i: curr)
	{
		i = i;
		sum += i;
	}
	
	return (sum%bucketCount);
}

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

vector<set<int>> genrated_candidate(vector<set<int>>&freqitems,int k,int min_sup,int bucketCount)
{
	vector<set<int>>candideds;
	map<int,int>hashfreq;
	map<int,vector<set<int>>>hashTable;
	int len = freqitems.size();
	for(int i=0;i<len;i++)
	{
		for(int j = i+1;j<len;j++)
		{
			set<int>candided;
			set_union(freqitems[i].begin(),freqitems[i].end(),freqitems[j].begin(),freqitems[j].end(),inserter(candided,candided.begin()));
			if(candided.size() == k)
			{
				int hashValue = hashfunction(candided,bucketCount);
				hashfreq[hashValue]++;
				if(hashTable.find(hashValue) == hashTable.end())
				{
					vector<set<int>>temp;
					hashTable.insert({hashValue,temp});
				}
				hashTable[hashValue].push_back(candided);
			}
		}
	}
	
	for(auto i: hashTable)
	{
		if(hashfreq[i.first] >= min_sup)
		{
			for(auto j: i.second)
			{
				candideds.push_back(j);
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

void hash_algo(vector<set<int>>&tran,int min_sup)
{
	int bucketCount = 19;
	ofstream file2;
	file2.open("1Q71.txt");
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
		candideds = genrated_candidate(freqitems,++k,min_sup,bucketCount);
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

	hash_algo(tran,2);
	return 0;
}