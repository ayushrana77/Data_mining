#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<algorithm>

using namespace std;

bool issubset(set<int> tans,set<int> data)
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

void inc_count(map<set<int>,int> &freqTemp, map<set<int>,int> &supportCount,set<int> currset)
{
	for(auto i: freqTemp)
	{
		if(issubset(currset,i.first))
		{
			supportCount[i.first]++;
		}
	}
}


vector<set<int>> gen_candided(vector<set<int>> &freq,vector<set<int>> &dead,set<int> currset,vector<int> &oneitemsets,map<set<int>,int> &Numberofsubset)
{
	vector<set<int>>candideds;
	for(auto i: oneitemsets)
	{
		set<int> candided = currset;
		if(candided.find(i) != candided.end())
		{
			continue;
		}
		candided.insert(i);
		Numberofsubset[candided]++;
		bool isexit = true;
		for(auto j: dead)
			{	
				if(candided == j)
				{
					isexit = false;
					break;
				}
			}
			if(isexit && Numberofsubset[candided] >= candided.size())
			{
				candideds.push_back(candided);
			}
	}
	return candideds;
}


void DIC_algo(vector<set<int>> tran, int min_sup)
{
	ofstream file2;
	file2.open("dic_out.txt");

	int n = tran.size();
	int partion = 0;

	map<int,int>itemcount;
	map<set<int>,int>freqTemp;
	map<set<int>,int>supportCount;
	vector<set<int>>freq;
	vector<set<int>>dead;
	map<set<int>,int>Numberofsubset;
	vector<int>oneitemsets;
	vector<set<int>>solid_squer;
	
	for(auto i: tran)
	{
		for(int j: i)
		{
			itemcount[j]++;
		}
	}
	for(auto i: itemcount)
	{
		if(i.second >= min_sup)
		{
			freqTemp.insert({{i.first},n});
			dead.push_back({i.first});
			supportCount.insert({{i.first},0});
			oneitemsets.push_back(i.first);
			file2<<" {";
			file2<<(char)((i.first-1)+'a')<<" ";
			file2<<"}";
			file2<<" Add in dotted circle";
			file2<<endl;
		}
	}
	
	int check = 0;
	
	while(!freqTemp.empty())
	{
		inc_count(freqTemp,supportCount,tran[partion]);
		file2<<" After partion "<<partion;
		file2<<endl;
		file2<<endl;
		
		vector<set<int>> to_remove;
		vector<set<int>> new_candided;
		
		for(auto &i: freqTemp)
		{
			if(supportCount[i.first] >= min_sup)
			{
				solid_squer.push_back(i.first);
				file2<<" {";
				for(auto j: i.first)
				{
					file2<<(char)((j-1)+'a')<<" ";
				}
				file2<<"}";
				file2<<" Add in dotted squar";
				file2<<endl;
				vector<set<int>> curr_candided = gen_candided(freq,dead,i.first,oneitemsets,Numberofsubset);
				
				freq.push_back(i.first);
				to_remove.push_back(i.first);
				
				for(auto j: curr_candided)
				{
					file2<<" {";
					for(auto k: j)
					{
						file2<<(char)((k-1)+'a')<<" ";
					}
					file2<<"}";
					file2<<" Add in dotted circle";
					file2<<endl;
					new_candided.push_back(j);
					supportCount.insert({j,0});
					dead.push_back(j);
				}
			}else{
				i.second--;
				if(i.second <= 0)
				{
					to_remove.push_back(i.first);
					file2<<" {";
					for(auto k: i.first)
					{
						file2<<(char)((k-1)+'a')<<" ";
					}
					file2<<"}";
					file2<<" Add in solid circle";
					file2<<endl;
				}
			}
	
		}
		
		for(auto i: new_candided)
		{
			freqTemp.insert({i,n});
		}
		
		for(auto i: to_remove)
		{
			supportCount.erase(i);
			freqTemp.erase(i);
		}
		partion += 1;
		if(partion == n)
		{
			for(auto k: solid_squer)
			{
				file2<<" {";
				for(auto t: k)
				{
					file2<<(char)((t-1)+'a')<<" ";
				}
				file2<<"}";
				file2<<" Add in solid squre";
				file2<<endl;
			}
			while(!solid_squer.empty())
			{
				solid_squer.pop_back();
			}
		}
		partion = partion%n;
		
		
		
	}
	file2<<endl;
	
	file2<<" FreqItem Set: ";
	
	file2<<endl;
	for(auto i: freq)
	{
		file2<<" {";
		for(auto j: i)
		{
			file2<<(char)((j-1)+'a')<<" ";
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
				x++;
				st.insert(x);	
				temp = "";
			}	
			else{
				temp += str[i];
			}
			i++;
		}
		int x = temp[0] - 'a';
		x++;
		st.insert(x);
		tran.push_back(st);	
	}
	
	DIC_algo(tran,3);
	return 0;
	
}