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

int LRU(int[][] recent, int set, int way, int hit)
{
	//if hit = -1, insert or replace
	if (hit != -1)
	{
		former = recent[set][hit];
		recent[set][hit] = 0;
		for (int i=0; i<way; i++)
		{
			if (i != hit && recent[set][i] < former)
			{
				(recent[set][i])++;
			}
		}
		return -1;
	}
	else
	{
		int least;
		for (int j=0; j<way; j++)
		{
			if (recent[set][j] == -1)
			{
				return j;
			}
			if (recent[set][j] == way-1)
			{
				least = j;
			}
		}
		for (int k=0; k<way; k++)
		{
			if (k == least)
			{
				recent[set][k] = 0;
			}
			else
			{
				(recent[set][k])++;
			}
		}
		return least;
	}
}

void setAssociative(string fileName, ofstream& outFile, int way)
{
	ifstream inFile(fileName);

	int set = 512/way;
	int cache[set][way];
	int recent[set][way];
	for (int i=0; i<set; i++)
	{
		for (int j=0; j<way; j++)
		{
			cache[i][j] = -1;
			recent[i][j] = -1;
		}
	}

	int hit = 0;
	int total = 0;
	int index;
	string ldstr;
	int addr;
	int tag;

	while (inFile >> ldstr >> hex >> addr)
	{
		total++;

		addr = addr>>5;
		index = addr % set;
		tag = addr>>9;

		for (int k=0; k<way; k++)
		{
			if (cache[index][k] == tag)
			{
				hit++;
				break;
			}
		}
	}

		
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
