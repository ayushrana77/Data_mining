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

void inc_count(map<set<int>,int> freqTemp, map<set<int>,int> supportCount,set<int> currset)
{
	for(auto i: freqTemp)
	{
		if(issubset(currset,i.first))
		{
			supportCount[i.first]++;
		}
	}
}


vector<set<int>> gen_candided(vector<set<int>> &freq,vector<set<int>>dead,set<int> currset)
{
	vector<set<int>>candideds;
	
	for(auto i: freq)
	{
		set<int> candided;
		if(i.size() == currset.size())
		{
			set_union(i.begin(),i.end(),currset.begin(),currset.end(),inserter(candided,candided.begin()));
			if(candided.size() == currset.size()+1)
			{
				bool isexit = true;
				for(auto j: dead)
				{
					if(candided == j)
					{
						isexit = false;
						break;
					}
				}
				if(isexit)
				{
					candideds.push_back(candided);
				}
			} 
		}
	}
	
	return candideds;
}


void DIC_algo(vector<set<int>>&tran,int min_sup)
{
//	ofstream file2;
//	file2.open("1Q72.txt");

	int n = tran.size();
	int partion = 0;

	map<int,int>itemcount;
	map<set<int>,int>freqTemp;
	map<set<int>,int>supportCount;
	vector<set<int>>freq;
	vector<set<int>>dead;
	cout<<itemcount.size();
	for(auto i: itemcount)
	{

		if(i.second >= min_sup)
		{
			freqTemp.insert({{i.first},n});
			dead.push_back({i.first});
			supportCount.insert({{i.first},0});
			cout<<(char)(i.first+'a')<<endl;
		}
	}
	cout<<"run3\n";
	
	int check = 0;
	
	
	while(!freqTemp.empty())
	{
		inc_count(freqTemp,supportCount,tran[partion]);
		vector<set<int>> to_remove;
		vector<set<int>> new_candided;
		
		for(auto i: freqTemp)
		{
			if(supportCount[i.first] >= min_sup)
			{
				vector<set<int>> curr_candided = gen_candided(freq,dead,i.first);
				freq.push_back(i.first);
				to_remove.push_back(i.first);
				
				for(auto j: curr_candided)
				{
					new_candided.push_back(j);
					dead.push_back(j);
				}
			}else{
				i.second--;
				if(i.second <= 0)
				{
					to_remove.push_back(i.first);
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
		partion = partion%n;
	if(check++<9)
	{
		break;
	}
	}
	
	for(auto i: freq)
	{
		for(auto j: i)
		{
			cout<<j<<" ";
		}
		cout<<endl;
	}

//	file2.close();
	
	
	
	
}


int main()
{
	ifstream file1;
	file1.open("input.txt");
	vector<set<int>>tran;
	string str;
	while(getline(file1,str))
	{
		cout<<str;
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
	DIC_algo(tran,2);
	return 0;
	
}