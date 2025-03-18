#include<bits/stdc++.h>
using namespace std;


const int UNCLASSIFIED = -1;
const int NOISE = -2;


struct Point {
    double x, y;  
    int clusterID; 

    Point(double x, double y) : x(x), y(y), clusterID(UNCLASSIFIED) {}
};


double euclideanDistance(const Point &p1, const Point &p2) 
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}


vector<int> regionQuery(const vector<Point> &points, int pointIndex, double eps) 
{
    vector<int> neighbors;
    for (int i = 0; i < points.size(); ++i) 
	{
        if (euclideanDistance(points[pointIndex], points[i]) <= eps) 
		{
            neighbors.push_back(i);
        }
    }
    return neighbors;
}


bool expandCluster(vector<Point> &points, int pointIndex, int clusterID, double eps, int minPts) 
{
    vector<int> seeds = regionQuery(points, pointIndex, eps);
    if (seeds.size() < minPts) 
	{
        points[pointIndex].clusterID = NOISE;
        return false;
    }
    
    for (int seedIndex : seeds) 
	{
        points[seedIndex].clusterID = clusterID;
    }

    for (size_t i = 0; i < seeds.size(); ++i) 
	{
        int currentPointIndex = seeds[i];
        vector <int>result = regionQuery(points, currentPointIndex, eps);

        if (result.size() >= minPts) 
		{
            for (int resultPointIndex : result) 
			{
                if (points[resultPointIndex].clusterID == UNCLASSIFIED || points[resultPointIndex].clusterID == NOISE) 
				{
                    if (points[resultPointIndex].clusterID == UNCLASSIFIED) 
					{
                        seeds.push_back(resultPointIndex);
                    }
                    points[resultPointIndex].clusterID = clusterID;
                }
            }
        }
    }
    return true;
}

void dbscan(vector<Point> &points, double eps, int minPts) 
{
    int clusterID = 0;
    for (int i = 0; i < points.size(); ++i) 
	{
        if (points[i].clusterID == UNCLASSIFIED) 
		{
            if (expandCluster(points, i, clusterID, eps, minPts)) 
			{
                clusterID++;
            }
        }
    }
}


vector<Point>loadPointsFromFile(const string &filename) 
{
    vector<Point> points;
    ifstream inputFile(filename);
    double x, y;

    if (inputFile.is_open()) 
	{
        while (inputFile >> x >> y) 
		{
            points.emplace_back(x, y);
        }
        inputFile.close();
    } else {
        cerr << "Error: Unable to open input file." << endl;
    }

    return points;
}


void saveClustersToFile(const string &filename, const vector<Point> &points) 
{
    ofstream outputFile(filename);

    if (outputFile.is_open()) 
	{
        for (const auto &point : points) 
		{
            outputFile << point.x << " " << point.y << " -> Cluster " << point.clusterID << endl;
        }
        outputFile.close();
    } else 
	{
        cerr << "Error: Unable to open output file." << endl;
    }
}


int main() 
{
    string inputFilename = "input.txt";
    string outputFilename = "output.txt";

    vector<Point>points = loadPointsFromFile(inputFilename);

   
    double eps = 4;    
    int minPts = 2;      

 
    dbscan(points, eps, minPts);

  
    saveClustersToFile(outputFilename, points);

    cout << "Clustering results have been saved to " << outputFilename << endl;

    return 0;
}
