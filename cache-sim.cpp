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

const int toPass = 1024;

int LRU(int index, int way, int hit, int recent[][toPass])
{
	//if hit = -1, insert or replace
	if (hit != -1)
	{
		int former = recent[index][hit];
		recent[index][hit] = 0;
		for (int i=0; i<way; i++)
		{
			if (i != hit && recent[index][i] < former)
			{
				(recent[index][i])++;
			}
		}
		return -1;
	}
	else
	{
		int least;
		for (int j=0; j<way; j++)
		{
			if (recent[index][j] == -1)
			{
				recent[index][j] = 0;
				return j;
			}
			if (recent[index][j] == way-1)
			{
				least = j;
			}
		}
		for (int k=0; k<way; k++)
		{
			if (k == least)
			{
				recent[index][k] = 0;
			}
			else
			{
				(recent[index][k])++;
			}
		}
		return least;
	}
}

void setAssociative(string fileName, ofstream& outFile, int way)
{
	ifstream inFile(fileName);

	int sets = 512/way;
	int cache[sets][way];
	int recent[sets][toPass];
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
		int LRUret = -1;

		for (int k=0; k<way; k++)
		{
			if (cache[index][k] == tag)
			{
				hit++;
				LRUret = LRU(index, way, k, recent);
				goodHit = 1;
				break;
			}
		}
		if (goodHit == 0)
		{
			LRUret = LRU(index, way, -1, recent);
			cache[index][LRUret] = tag;
		}

		/*for (int z=0; z<way; z++)
		{
			cout << recent[index][z] << " ";
		}
		cout << endl;*/
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
	outFile << "; ";
	setAssociative(fileName, outFile, 4);
	outFile << "; ";
	setAssociative(fileName, outFile, 8);
	outFile << "; ";
	setAssociative(fileName, outFile, 16);
	outFile << ";" << endl;

	setAssociative(fileName, outFile, 512); //fully associative
	outFile << ";" << endl;

	return 0;
}
