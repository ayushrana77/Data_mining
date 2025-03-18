#include<bits/stdc++.h>

using namespace std;

void k_mean(vector<pair<double,double>> data,int k)
{
	ofstream file2;
	file2.open("k_mean_out.txt");
	
    map<int,vector<pair<double,double>>>prev_cluserts;
    map<int,vector<pair<double,double>>>curr_cluserts;
    map<int,pair<double,double>>centroid;

    for(int i=0;i<k;i++)
    {
        vector<pair<double,double>>temp;
        curr_cluserts[i] = temp;
        centroid[i] = data[i];
    }
    
    while(prev_cluserts != curr_cluserts)
    {
        prev_cluserts = curr_cluserts;
        for(auto &i: curr_cluserts)
        {
            i.second.clear();
        }
        for(auto i: data)
        {
            int index = 0;
            double min_distance = DBL_MAX;
            for(auto j: centroid)
            {
                double curr_diatnce = sqrt(pow((i.first-j.second.first),2.0) + pow((i.second-j.second.second),2.0));
                if(min_distance > curr_diatnce)
                {
                    index = j.first;
                    min_distance = curr_diatnce;
                }
            }
            curr_cluserts[index].push_back(i);
        }
        for(auto &i: centroid)
        {
            double x_sum = 0.0;
            double y_sum = 0.0;
            for(auto j: curr_cluserts[i.first])
            {
                x_sum += j.first;
                y_sum += j.second;
            }
            x_sum = x_sum /curr_cluserts[i.first].size();
            y_sum = y_sum /curr_cluserts[i.first].size();
            i.second.first = x_sum;
            i.second.second = y_sum; 
        }
        int  x = k;
        file2<<"CLUSTER ARE DIVIED INTO"<<endl;
        for(auto i: curr_cluserts)
        {
            file2<<"Cluster "<<x<<endl;
            for(auto j: i.second)
            {
                file2<<j.first<<" "<<j.second<<endl;
            }
            file2<<endl;
            x--;
        }
    }

    file2.close();
    
}

int main()
{
    ifstream file1;
    file1.open("input1.txt");
    vector<pair<double,double>>data;
    string str;
    while(getline(file1,str))
    {
        pair<double,double>curr;
        stringstream ss(str);
        ss >> curr.first;
        ss >> curr.second;
        data.push_back(curr);
    }
    k_mean(data,3);
}