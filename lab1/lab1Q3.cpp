#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main()
{
	string str;
	fstream file,file2;
	file.open("tb.txt",ios::in);
	file2.open("1Q3.txt",ios::out);
	int arr[26];
	for(int i=0;i<26;i++)
	{
		arr[i] = 0;
	}
	while(getline(file,str))
	{
		for(int i=3;i<str.size();i += 2)
		{
			arr[str[i]-'a']++;
		}
	}
	for(int i=0;i<26;i++)
	{
		string res;
		res = "";
		res += 'a'+i;
		res += " ";
		if(arr[i] != 0)
		{
			res += to_string(arr[i]);
			res += "\n";
			file2<<res;
		}
	}
	file.close();
	file2.close();
}