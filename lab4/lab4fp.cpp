#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include<sstream>
#include<queue>

using namespace std;



struct trie
{
    int support_count;
    int val;
    trie* children[26];
    trie * next;
    trie * parent;
    trie()
    {
        val = 0;
        support_count = 0;
        for(int i=0;i<26;i++)
        {
            children[i] = NULL;
        }
        next = NULL;
        parent = NULL;
    }
};

bool cmp1(pair<int,int> &a,pair<int,int> & b)
{
	if(a.second == b.second)
	{
		return (a.first < b.first);
	}
    return (a.second >= b.second);
}

vector<int> sortcmp(vector<pair<int,int>> onefreq, vector<int>tran)
{
    vector<int> res;
    for(auto i: onefreq)
    {
        if(find(tran.begin(),tran.end(),i.first) != tran.end())
        {
            res.push_back(i.first);
        }
    }
    return res;
}
 
void insert_into_tree(trie *root,vector<int> tran,int inc)
{
    for(auto i: tran)
    {
        if(root->children[i] == NULL)
        {
            root->children[i] = new trie();
        }
        trie *temp = root;
        root = root->children[i];
        root->val = i;
        root->support_count += inc;
        root->parent = temp;
    }
}

void connect_link(trie *root,vector<int> tran, map<int,trie*> & hash)
{
    for(auto i: tran)
    {
        root = root->children[i];
        if(hash[i] == NULL)
        {
            hash[i] = root;
        }else{
            trie * curr = hash[i];
            bool isexit = false;
            while(curr->next != NULL)
            {
                if(curr == root)
                {
                    isexit = true;
                    break;
                }
                curr = curr->next;
            }
            if(!isexit && curr != root)
            {
                curr->next = root;
            }
        }
    }
}

void print_tree(trie* root, map<int,trie*>hash)
{
	fstream file3;
	file3.open("lab4fp_tree_link.txt",ios::app);
	if(root == NULL)
	{
		return;
	}

	file3<<"Print the tree "<<endl;
	queue<trie*>qu;
	qu.push(root);
	while(!qu.empty())
	{
		int si = qu.size();
		while(si--)
		{
			trie * curr = qu.front();
			qu.pop();
			if(curr->val == 0)
			{
				file3<<"root"<<" : "<<curr->support_count;
			}
			else
			{
				file3<<(char)(curr->val - 1 + 'a')<<" : "<<curr->support_count<<" ";
			}
			for(int i=0;i<26;i++)
	        {
	            if(curr->children[i] != NULL)
	            {
	            	qu.push(curr->children[i]);
				}
	        }
    	}
    	file3<<endl;
	}
	
	file3<<"Link of the tree "<<endl;
	
	for(auto i: hash)
	{
		
		trie *curr = i.second;
		while(curr)
		{
			file3<<"("<<(char)(curr->val-1 + 'a')<<": "<<curr->support_count<< " ) -->";
			curr = curr->next;
		}
		file3<<" NULL "<<endl;
	}
	file3<<endl;
	
	file3.close();
	
}

void minetree(map<int,trie*> hash, vector<pair<int,int>> onefreq,vector<int> &suffix, vector<vector<int>> &freqitemsets,int min_sup)
{
    for(int i = onefreq.size() - 1; i >= 0 ;i--)
    {
        
       
        suffix.push_back(onefreq[i].first);

        
        freqitemsets.push_back(suffix);

      
        trie * temp = hash[onefreq[i].first];

        

        
        vector<pair<vector<int>,int>> Conditional_Pattern_tran;

       
        vector<pair<int,int>> conditional_onefreq;

        
        map<int,int>count_freq;

       
        map<int,trie*>conditional_hash;


        
        while (temp)
        {
            trie *curr = temp->parent;
            vector<int>curr_tran;

           
            while (curr)
            {
                curr_tran.push_back(curr->val);
                curr = curr->parent;
            }
            
            curr_tran.pop_back();
            
            if(!curr_tran.empty())
            {
                
                Conditional_Pattern_tran.push_back({curr_tran,temp->support_count});
                for(auto i: curr_tran)
                {
                    
                    count_freq[i] += temp->support_count; 
                }
            }
            temp = temp->next;
        }



        for(auto j: count_freq)
        {
            if(j.second >= min_sup)
            {
                conditional_onefreq.push_back({j.first,j.second});
                conditional_hash.insert({j.first,NULL});
            }
        }

       
        sort(conditional_onefreq.begin(),conditional_onefreq.end(),cmp1);

         
        trie * root = new trie();

        for(auto j: Conditional_Pattern_tran)
        {
            
            vector<int> tran_after_sort = sortcmp(conditional_onefreq,j.first);
            
            
            insert_into_tree(root,tran_after_sort,j.second);

           
            connect_link(root,tran_after_sort,conditional_hash);

            
            
        }

		fstream file3;
		file3.open("lab4fp_tree_link.txt",ios::app);
		file3<<"condtional tree  :"<<"{";
		for(int ed: suffix)
		{
			file3<<(char)(ed -1 +'a')<<" ";
		}
		file3<<"}";
		file3<<endl;
		file3.close();
		
		print_tree(root,conditional_hash);
		
        
        minetree(conditional_hash,conditional_onefreq,suffix,freqitemsets,min_sup);

        
        suffix.pop_back();
    }

}


void FP_growth(vector<vector<int>>tran, int min_sup)
{
   
    map<int,int>temp;
    ofstream file2;
	file2.open("lab4fp_freq_Count.txt");
    for(auto i : tran)
    {
        for(auto j: i)
        {
            temp[j]++;
        }
    }
    
    vector<pair<int,int>>onefreq;

    
    map<int,trie*>hash;

    for(auto i: temp)
    {
        if(i.second >= min_sup)
        {
            onefreq.push_back({i.first,i.second});
            hash.insert({i.first,NULL});
        }
    }

 
    sort(onefreq.begin(),onefreq.end(),cmp1);
    
	
   
    trie *root = new trie();

    for(auto i: tran)
    {
       
        vector<int> tran_after_sort = sortcmp(onefreq,i);
        
        
       
        insert_into_tree(root,tran_after_sort,1);

        
        connect_link(root,tran_after_sort,hash);
    }
    
    print_tree(root,hash);
   
    vector<int>suffix;
   
    vector<vector<int>>freqitemsets;

    
    minetree(hash,onefreq,suffix,freqitemsets,min_sup);


    file2<<"frequent Item sets:"<<endl;
    for(auto i: freqitemsets)
    {
        sort(i.begin(),i.end());
        file2<<" { ";
        for(int j: i)
        {
            file2<<(char)(j-1+'a')<<" ";
        }
        file2<<"}";
        file2<<endl;
    }
    file2.close();
}

int main()
{

    ifstream file;
    int min_sup = 2;
    file.open("input.txt");
    string str;
    vector<vector<int>>tran;
    while(getline(file,str))
    {
        stringstream ss(str);
        string token;
        

        vector<int> t;
        ss >> token;
        while(ss >> token)
        {
            if (token.length() == 1 && isalpha(token[0]))
            {
                t.push_back(token[0] - 'a' + 1); 
            }
        }
        tran.push_back(t);
    }

    file.close();

    FP_growth(tran, min_sup);


    return 0;
}
