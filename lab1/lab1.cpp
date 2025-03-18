#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	fstream file;
	file.open("tb.txt",ios::out);
	file<<"T1 a b c e \n";
	file<<"T2 b d f \n";
	file<<"T3 a c d f \n";
	file<<"T4 d f \n";
	file<<"T5 c d e \n";
	file.close();
}