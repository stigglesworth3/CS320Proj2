#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdio.h>

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
	int tag;
	int logSize = log2(size);

	while (inFile >> ldStr >> hex >> addr)
	{
		total++;

		addr = addr>>5;
		index = addr % size;
		tag = addr>>logSize;
		
		if (cache[index] != tag)
		{
			cache[index] = tag;
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
	outFile << "; ";
	directMapped(fileName, outFile, 128);
	outFile << "; ";
	directMapped(fileName, outFile, 512);
	outFile << "; ";
	directMapped(fileName, outFile, 1024);
	outFile << ";" << endl;
	return 0;
}
