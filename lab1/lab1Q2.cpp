#include<iostream>
#include<fstream>
#include<string>

using namespace std;
int main()
{
	string str;
	fstream file;
	fstream file2;
	file.open("tb.txt",ios::in);
	file2.open("1Q2.txt",ios::out);
	while(getline(file,str))
	{
		str += "\n";
		file2<<str;
	}
	file.close();
}