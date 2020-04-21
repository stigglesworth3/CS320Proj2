#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void directMapped(string fileName, ofstream& outFile, int size)
{
	ifstream inFile(fileName);

	int cache[size];
	for (int i=0; i<size; i++)
	{
		cache[i] = -1;
	}

	int hit = 0;
	int total = 0;
	int index;
	string ldStr;
	int addr;
	while (inFile >> ldStr >> hex >> addr)
	{
		total++;
		index = addr % size;
		if (cache[index] != addr)
		{
			cache[index] = addr;
		}
		else
		{
			hit++;
		}
	}

	outFile << hit  << "," << total;
}

int main(int argc, char *argv[])
{
	string fileName = argv[1];
	ofstream outFile;
	outFile.open(argv[2]);

	directMapped(fileName, outFile, 32);
	return 0;
}
