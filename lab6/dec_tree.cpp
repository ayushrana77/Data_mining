#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

struct table {
    int rid;
    map<string, string> attributes; 
};

struct de_tree{
    string field;
    map<string,de_tree*>children;
};

double get_entopy(vector<table>data,string att)
{
    map<string,int>freq;
    for(auto i: data)
    {
        freq[i.attributes[att]]++;
    }
	
    int n = data.size();
    double entopy = 0;

    for(auto i: freq)
    {
        double prob = (double)i.second/(double)n;
        entopy -= prob*log2(prob);
    }
    return entopy;
}

vector<table> filter_data(vector<table>data,string att_val,string curr_att)
{
    vector<table>res;
    for(auto i: data)
    {
        if(i.attributes[curr_att] == att_val)
        {
            res.push_back(i);
        }
    }
    return res;
}

string find_majority(vector<table>data,string att)
{
    map<string,int>freq;
    for(auto i: data)
    {
        freq[i.attributes[att]]++;
    }
    int  majority_class_freq = 0;
    string  majority_class;
    for(auto i : freq)
    {
        if(majority_class_freq < i.second)
        {
            majority_class_freq = i.second;
            majority_class = i.first;
        }
    }
    return majority_class;
}

double get_gain(vector<table> data,string att,double entopy)
{
    int n = data.size();
    map<string,int>att_freq;
    double gain = 0;
    for(auto i: data)
    {
        att_freq[i.attributes[att]]++;
    }
    for(auto i: att_freq)
    {
        vector<table> curr_att_data = filter_data(data,i.first,att);
        double att_val_entopy = get_entopy(curr_att_data,"buyscomputer");
        gain += ((double)i.second/(double)n)*att_val_entopy;
    }
    return (entopy - gain);
}

void get_all_path(de_tree* root, vector<vector<string>>&allpath, vector<string> path)
{
    if(root == NULL)
    {
        return;
    }
    if(root->field == "yes" || root->field  == "no")
    {
        path.push_back(root->field);
        allpath.push_back(path);
        return;
    }
    for(auto i: root->children)
    {
        string temp = root->field;
        temp += ":";
        temp += i.first;
        path.push_back(temp);
        get_all_path(i.second,allpath,path);
        path.pop_back();
    }
}

de_tree * create_tree(vector<string> attribute,vector<table> data,string root_val,string val = "")
{
    fstream file2;
	file2.open("output.txt",ios::app);
	
	
    double entopy = get_entopy(data,"buyscomputer");
    if(entopy == 0)
    {
        de_tree *root = new de_tree();
        root->field = data[0].attributes["buyscomputer"];
        return root;
    }

    if(attribute.size() == 0)
    {
        de_tree *root = new de_tree();
        root->field = find_majority(data,"buyscomputer");
        return root;
    }


    double higest_gain = 0;
    string higest_gain_att = "";

    
    for(auto att: attribute)
    {
        double gain = get_gain(data,att,entopy);
        file2<<"Infromation gain of when root "<< root_val <<":"<<val<< " and attribute "<<att<<" Gain "<<gain<<endl;
        if(gain > higest_gain)
        {
            higest_gain = gain;
            higest_gain_att = att;
        }
    }

    

    de_tree *root = new de_tree();
    root->field = higest_gain_att;

  
    auto it = find(attribute.begin(),attribute.end(),higest_gain_att);
    attribute.erase(it);

    map<string,int>root_att_freq;
    for(auto i: data)
    {
        root_att_freq[i.attributes[higest_gain_att]]++;
    }

    for(auto i: root_att_freq)
    {
        vector<table> curr_att_data = filter_data(data,i.first,higest_gain_att);
        root->children.insert({i.first,NULL});
      
        root->children[i.first] = create_tree(attribute,curr_att_data,higest_gain_att,i.first);
        
    }
    file2.close();
    return root;
}

void print_all_path(de_tree *root)
{
	fstream file2;
	file2.open("output.txt",ios::app);
    vector<vector<string>>allpath;
    vector<string> path;
    get_all_path(root,allpath,path);

    for(auto i: allpath)
    {
        for(int j=0;j<i.size();j++)
        {
            if(j == i.size()-1)
            {
                file2<<i[j];
            }
            else{
                file2<<i[j]<<" ---> ";
            }
            
        }
        file2<<endl;
    }
    file2<<endl;
    
    file2.close();
}

string slove_query(de_tree *root,table &query)
{
    if(root == NULL)
    {
        return "";
    }
    if(root->children.size() == 0)
    {
        return root->field;
    }
    return slove_query(root->children[query.attributes[root->field]],query);
}

void handel_query(de_tree *root)
{ 
    ifstream file;
    file.open("query.txt");
    
    fstream file2;
	file2.open("output.txt",ios::app);
	
	
    string str;
    while(getline(file,str))
    {
        stringstream ss(str);
        string token;
        table query;
        getline(ss, query.attributes["age"], ' ');
        getline(ss, query.attributes["income"], ' ');
        getline(ss, query.attributes["student"], ' ');
        getline(ss, query.attributes["credit_rating"], ' ');
        file2<<"query : "<<str<<" ans : "<<slove_query(root,query)<<endl;
    }
	file2.close();
}

int main()
{
    ifstream file;
    file.open("input.txt");
    
    fstream file2;
	file2.open("output.txt",ios::out);
	file2.close();

    vector<table>data;

    string str;
    getline(file,str);
    while (getline(file, str)) {
        stringstream ss(str);
        table t;
        string value;
        getline(ss, value, ' ');
        t.rid = stoi(value); 
        getline(ss, t.attributes["age"], ' ');
        getline(ss, t.attributes["income"], ' ');
        getline(ss, t.attributes["student"], ' ');
        getline(ss, t.attributes["credit_rating"], ' ');
        getline(ss, t.attributes["buyscomputer"], ' ');

        data.push_back(t);
    }

    vector<string>attribute{"age","income","student","credit_rating"};
    sort(attribute.begin(),attribute.end());
    
    file2;
	file2.open("output.txt",ios::app);
    
    file2<<"INFROMATION GAIN"<<endl;
    
    file2.close();
    
    
    de_tree *root = create_tree(attribute,data,"");
    
	file2.open("output.txt",ios::app);
    file2<<endl<<"ALL PATH"<<endl;
    file2.close();
    print_all_path(root);
    handel_query(root);
    file.close();
}