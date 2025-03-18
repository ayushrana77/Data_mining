#include<bits/stdc++.h>

using namespace std;

struct CFEntry 
{
    vector<double> linear_sum;  
    double squared_sum = 0.0;        
    int count = 0;                  
    vector<vector<double>> points;  

    CFEntry(int dim) : linear_sum(dim, 0.0) {}

    void add_point(const vector<double>& point) 
	{
        points.push_back(point);
        for (int i = 0; i < linear_sum.size(); ++i) 
		{
            linear_sum[i] += point[i];
        }
        for (double coord : point) 
		{
            squared_sum += coord * coord;
        }
        count++;
    }

    double radius() const 
	{
        if (count == 0) return 0.0;
        double mean_square = squared_sum / count;
        double square_of_mean = 0.0;
        for (double sum : linear_sum) 
		{
            square_of_mean += (sum / count) * (sum / count);
        }
        return sqrt(mean_square - square_of_mean);
    }
};

struct CFNode 
{
    vector<CFEntry> entries;
    int max_entries;

    CFNode(int dim, int max_entries) : max_entries(max_entries) 
	{
        entries.reserve(max_entries);
    }

    CFEntry* find_fitting_entry(const vector<double>& point, double threshold) 
	{
        for (auto &entry : entries) 
		{
            CFEntry temp = entry;
            temp.add_point(point);
            if (temp.radius() <= threshold) return &entry;
        }
        return nullptr;
    }

    void add_entry(const vector<double>& point, double threshold) 
	{
        CFEntry* entry = find_fitting_entry(point, threshold);
        if (entry) 
		{
            entry->add_point(point);
        } else 
		{
            CFEntry new_entry(point.size());
            new_entry.add_point(point);
            entries.push_back(new_entry);
            
        }
    }
};

int main() {
    int dimension = 2;
    double radius_threshold = 2.0;
    int max_entries = 3;

    CFNode root(dimension, max_entries);

    vector<vector<double>> data;
    
    ifstream file1;
	file1.open("input.txt");
	string str;
	while(getline(file1,str))
	{
		stringstream ss(str);
		vector<double>temp(2);
		ss >> temp[0];
		ss >> temp[1];
		data.push_back(temp);
	}

    	
	

    for (const auto &point : data) 
	{
        root.add_entry(point, radius_threshold);
    }

	ofstream file2;
	file2.open("output2.txt");

    for (int i = 0; i < root.entries.size(); ++i) 
	{
        file2 << "Cluster " << i + 1 << ": Count = " << root.entries[i].count
                  << ", Radius = " << root.entries[i].radius() << "\n";
        file2 << "Points in this cluster:\n";
        for (const auto& point : root.entries[i].points) 
		{
            file2 << "(";
            for (size_t j = 0; j < point.size(); ++j) 
			{
                file2 << point[j];
                if (j < point.size() - 1) file2 << ", ";
            }
            file2 << ")\n";
        }
    }
    file2.close();
	file1.close();
    return 0;
}
