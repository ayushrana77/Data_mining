#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

bool isSubset(const set<int>& transaction, const set<int>& candidate) {
    for (int item : candidate) {
        if (transaction.find(item) == transaction.end()) {
            return false;
        }
    }
    return true;
}

vector<set<int>> generateCandidates(const vector<set<int>>& frequentItemsets, int k) {
    vector<set<int>> candidates;
    int len = frequentItemsets.size();

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            
            set<int> candidate;
            set_union(frequentItemsets[i].begin(), frequentItemsets[i].end(),
                      frequentItemsets[j].begin(), frequentItemsets[j].end(),
                      inserter(candidate, candidate.begin()));

            
            if (candidate.size() == k) {
                bool isexit = false;
                for(auto k: candidates)
                {
                    if(k == candidate)
                    {
                        isexit = true;
                        break;
                    }
                }
                if(!isexit)
                {
                    candidates.push_back(candidate);
                }
                
            }
        }
    }
    return candidates;
}

vector<set<int>> filterCandidates(const vector<set<int>>& candidates, const vector<set<int>>& transactions, int minSupport) {
    map<set<int>, int> supportCount;

    for (const auto& transaction : transactions) {
        for (const auto& candidate : candidates) {
            if (isSubset(transaction, candidate)) {
                supportCount[candidate]++;
            }
        }
    }

    vector<set<int>> frequentItemsets;
    for (const auto& candidate : supportCount) {
        if (candidate.second >= minSupport) {
            frequentItemsets.push_back(candidate.first);
        }
    }

    return frequentItemsets;
}

//removing the maxfreq which are frequnet from  infrequnrt.  return from top which are still infrequnrt. 
vector<set<int>> find_infrequent(vector<set<int>> maxfreq,vector<set<int>> infrequnet)
{
    vector<set<int>>res;
    for(auto i: infrequnet)
    {
        bool isexit = false;
        for(auto j: maxfreq)
        {
            if(i == j)
            {
                isexit = true;
                break;
            }
        }
        if(!isexit)
        {
            res.push_back(i);
        }
    }
    return res;
}

//removing the itemsets from the bottom with the help of top item . top item = maxset and frequentitemssets.
void removefreq(vector<set<int>> &maxset,vector<set<int>> &frequentItemsets)
{
    vector<set<int>>to_remove;
    for(auto i: maxset)
    {
        for(auto j : frequentItemsets)
        {
            if(isSubset(i,j))
            {
                to_remove.push_back(j);
            }
        }
    }
    for(auto i: to_remove)
    {
        auto it = find(frequentItemsets.begin(),frequentItemsets.end(),i);
        if(it != frequentItemsets.end())
        {
            frequentItemsets.erase(it);
        }
    }
}

// removing the item from top if some bottom item is infrequnrt we remove it. 
void remove_top(vector<set<int>> &infrequent,vector<set<int>> &bottominfreq)
{
    vector<set<int>>to_remove;
    for(auto i: bottominfreq)
    {
        for(auto j : infrequent)
        {
            if(isSubset(j,i))
            {
                to_remove.push_back(j);
            }
        }
    }
    for(auto i: to_remove)
    {
        auto it = find(infrequent.begin(),infrequent.end(),i);
        if(it != infrequent.end())
        {
            infrequent.erase(it);
        }
    }

}

// generating the new infrequnrt from top;
vector<set<int>> find_new_infreq(vector<set<int>> infrequent,vector<set<int>> allmaxset)
{
    vector<set<int>> res;
    for(auto i: infrequent)
    {
        for(int j: i)
        {
            set<int>temp = i;
            temp.erase(j);
            bool isexit = false;
            for(auto item: res)
            {
                if(temp == item)
                {
                    isexit = true;
                    break;
                }
            }
            for(auto t : allmaxset)
            {
                if(isSubset(t,temp))
                {
                    isexit = true;
                    break;
                }
            }
            if(!isexit)
            {
                res.push_back(temp);
            }
        }
    }
    return res;
}


void princer_algo(vector<set<int>>& database, int minsup) {
    
    ofstream file2;
	file2.open("princer_out.txt");
	
    map<int, int> itemCount; // for one item count
    vector<set<int>> candidates; // to genrate the new candided
    vector<set<int>> maxFrequentItemsets; // till now all the elemnet which are frequnet not removed by the 
    vector<set<int>> allmaxset;
    vector<set<int>> maxset;
    vector<set<int>> bottominfreq;
    vector<set<int>> frequentItemsets;
    vector<set<int>> infrequent;
    set<int>temp;

    for (const auto& transaction : database) {
        for (int item : transaction) {
            itemCount[item]++;
        }
    }

    

    for (const auto& item : itemCount) {
        if (item.second >= minsup) {
            temp.insert(item.first);
            frequentItemsets.push_back({item.first});
        }
    }

    maxFrequentItemsets = frequentItemsets;
    int k = 1;
    infrequent.push_back(temp);
    maxset = filterCandidates(infrequent, database, minsup);

    if(maxset.size() != 0)
    {
        frequentItemsets.clear();
    }
    
    while (!frequentItemsets.empty()) {

        infrequent = find_new_infreq(infrequent,allmaxset);
        
        cout<<"infrequnet gen"<<endl;
        for(auto i: infrequent)
        {
        	for(int j: i)
        	{
        		cout<<j<<" ";
			}
			cout<<endl;
		}
		
        maxset = filterCandidates(infrequent, database, minsup);
        
		cout<<"maxset"<<endl;
		for(auto i: maxset)
		{
			for(int j: i)
			{
				cout<<j<<" ";
			}
			cout<<endl;
		}
        
        infrequent = find_infrequent(maxset,infrequent);

		cout<<"find infrequent"<<endl;
		
		for(auto i: infrequent)
		{
			for(int j: i)
			{
				cout<<j<<" ";
			}
			cout<<endl;
		}
        

        candidates = generateCandidates(frequentItemsets, ++k);

		cout<<"candidate"<<endl;
		for(auto i: candidates)
		{
			for(int j: i)
			{
				cout<<j<<" ";
			}
			cout<<endl;
		}
        
        frequentItemsets = filterCandidates(candidates, database, minsup);

		cout<<"freqitem set"<<endl;
		for(auto i: frequentItemsets)
		{
			for(int j : i)
			{
				cout<<j<<" ";
			}
			cout<<endl;
		}
        
        bottominfreq = find_infrequent(frequentItemsets,candidates);

        cout<<"bottom freq "<<endl;
        for(auto i: bottominfreq)
        {
        	for(int j: i)
        	{
        		cout<<j<<" ";
			}
			cout<<endl;
		}
        

        removefreq(maxset,frequentItemsets);

        cout<<"frequnet set after remove";
        for(auto i: frequentItemsets)
        {
        	for(int j: i)
        	{
        		cout<<j<<" ";
			}
			cout<<endl;
		}
        
        removefreq(maxset,maxFrequentItemsets);

		cout<<"all curr frequnet set after remove";
        for(auto i: maxFrequentItemsets)
        {
        	for(int j: i)
        	{
        		cout<<j<<" ";
			}
			cout<<endl;
		}
        
        remove_top(infrequent,bottominfreq);
        
        
        if(infrequent.empty())
        {
            removefreq(frequentItemsets,maxFrequentItemsets);
        }
        for(auto i: frequentItemsets)
        {
            maxFrequentItemsets.push_back(i);
        }
        for(auto i: maxset)
        {
            allmaxset.push_back(i);
        }

    }
    for(auto i: allmaxset)
    {
        maxFrequentItemsets.push_back(i);
    }

  
    if (!maxFrequentItemsets.empty()) {
        file2 << "princer element:" << endl;
        for (auto itemset : maxFrequentItemsets) {
            file2 << "{ ";
            for (int item : itemset) {
                file2 << (char)(item + 'a' - 1) << " ";
            }
            file2 << "}" << endl;
        }
    } else {
        file2<< "no itemsets found." << endl;
    }
    
    
	file2.close();
}

int main()
{
    ifstream file;
    int min_sup = 2;
    file.open("input.txt");
    string str;
    vector<set<int>> tran;
    
    while (getline(file, str))
    {
        stringstream ss(str);
        string token;
        set<int> t;

       
        ss >> token;  
        while (ss >> token)
        {
            
            if (token.length() == 1 && isalpha(token[0]))
            {
                t.insert(token[0] - 'a' + 1); 
            }
        }
        tran.push_back(t);
    }

    file.close();

    princer_algo(tran, min_sup);

    return 0;
}