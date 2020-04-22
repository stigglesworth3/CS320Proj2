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

/*int LRU(int sets, int way, int hit, int recent)
{
	//if hit = -1, insert or replace
	if (hit != -1)
	{
		int former = recent[sets][hit];
		recent[sets][hit] = 0;
		for (int i=0; i<way; i++)
		{
			if (i != hit && recent[sets][i] < former)
			{
				(recent[sets][i])++;
			}
		}
		return -1;
	}
	else
	{
		int least;
		for (int j=0; j<way; j++)
		{
			if (recent[sets][j] == -1)
			{
				return j;
			}
			if (recent[sets][j] == way-1)
			{
				least = j;
			}
		}
		for (int k=0; k<way; k++)
		{
			if (k == least)
			{
				recent[sets][k] = 0;
			}
			else
			{
				(recent[sets][k])++;
			}
		}
		return least;
	}
}*/

void setAssociative(string fileName, ofstream& outFile, int way)
{
	ifstream inFile(fileName);

	int sets = 512/way;
	int cache[sets][way];
	int recent[sets][way];
	for (int i=0; i<sets; i++)
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
		index = addr % sets;
		tag = addr>>9;
		int goodHit = 0;
		int LRUret;

		for (int k=0; k<way; k++)
		{
			if (cache[index][k] == tag)
			{
				hit++;
				int former = recent[sets][hit];
				recent[sets][hit] = 0;
				for (int i=0; i<way; i++)
				{
					if (i != hit && recent[sets][i] < former && recent[sets][i] != -1)
					{
						(recent[sets][i])++;
					}
				}
				goodHit = 1;
				break;
			}
		}
		if (goodHit != 1)
		{
			int least;
			int added = 0;
			for (int j=0; j<way; j++)
			{
				if (recent[sets][j] == -1)
				{
					cache[sets][j] = tag;
					recent[sets][j] = 0;
					for (int w=0; w<way; w++)
					{
						if (w != j && recent[sets][w] != 0)
						{
							(recent[sets][w])++;
						}
					}
					added = 1;
					break;
				}
				if (recent[sets][j] == way-1)
				{
					least = j;
				}
			}
			if (added == 0)
			{
				for (int k=0; k<way; k++)
				{
					if (k == least)
					{
						recent[sets][k] = 0;
					}
					else if (recent[sets][k] != -1)
					{
						(recent[sets][k])++;
					}
				}
			}
			cache[sets][LRUret] = tag;
		}
	}
	outFile << hit << "," << total;
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

	setAssociative(fileName, outFile, 2);
	return 0;
}
