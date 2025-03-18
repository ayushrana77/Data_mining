#include<bits/stdc++.h>

using namespace std;

double get_curr_freq(vector<map<string,string>> &data,string curr_att,string curr_att_val, string curr_class_val)
{
    int res = 0;
    for(auto i: data)
    {
        if(i[curr_att] == curr_att_val && i["buyscomputer"] == curr_class_val)
        {
            res++;
        }
    }
    return res;
}

void navie_base(vector<map<string,string>> &data,map<string,string>&query,int count)
{
    fstream file2;
    file2.open("navie_base_out.txt",ios::app);
    file2<<"Query  "<<count<<endl<<endl;
    
    map<string,int>class_freq;
    
    for(auto i: data)
    {
        class_freq[i["buyscomputer"]]++;
    }

    int n = data.size();
    string higest_prob_class_val;
    double higest_prob = 0.0;
    for(auto curr_val: class_freq)
    {
        double curr_val_prob = (double)curr_val.second/(double)n;
        file2<<curr_val.first<<" "<<curr_val.second<<endl;
        for(auto j : query)
        {
            double curr_att_freq = get_curr_freq(data,j.first,j.second,curr_val.first);
            if(curr_att_freq == 0)
            {
                set<string>unique;
                for(auto i: data)
                {
                    unique.insert(i[j.first]);
                }
                curr_val_prob *= 1.0/(double)(curr_val.second+unique.size());
                file2<<"( "<<j.second<<" : "<<curr_val.first<<" ) : "<<1.0/(double)(curr_val.second+unique.size())<<endl;
            }else{
                curr_val_prob *= curr_att_freq/(double)curr_val.second;
                file2<<"( "<<j.second<<" : "<<curr_val.first<<" ) : "<<curr_att_freq/(double)curr_val.second<<endl;
            }
            
           
             
        }
        file2<<curr_val_prob<<endl;
        if(curr_val_prob > higest_prob)
        {
            higest_prob = curr_val_prob;
            higest_prob_class_val = curr_val.first;
        }
    }
    file2<<"It was predicted the value : "<<higest_prob_class_val<<endl<<endl;
    file2.close();
}

int main()
{
    vector<map<string,string>>data;
    vector<map<string,string>>query;
    ifstream file1;
    file1.open("input.txt");
    string str;
    getline(file1,str);
    
    while(getline(file1,str))
    {
        stringstream ss(str);
        map<string,string>t;
        getline(ss,t["RID"],' ');
        getline(ss,t["age"],' ');
        getline(ss,t["income"],' ');
        getline(ss,t["student"],' ');
        getline(ss,t["credit_rating"],' ');
        getline(ss,t["buyscomputer"],' ');
        data.push_back(t);
    }

    file1.close();

    file1.open("query.txt");
    int count = 1;
    
    ofstream file2;
    file2.open("navie_base_out.txt");
    file2.close();
    
    while(getline(file1,str))
    {
    	stringstream ss(str);
	    map<string,string>query;
	    getline(ss,query["age"],' ');
	    getline(ss,query["income"],' ');
	    getline(ss,query["student"],' ');
	    getline(ss,query["credit_rating"],' ');
	    navie_base(data,query,count);
	    count++;
	}
    
    
    file1.close();

    return 0;
}