#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <bitset>

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
		if (former != 0)
		{
			recent[index][hit] = 0;
			for (int i=0; i<way; i++)
			{
				if (i != hit && recent[index][i] < former && recent[index][i] != -1)
				{
					(recent[index][i])++;
				}	
			}
		}	
		return -1;
	}
	else
	{
		int least = -1;
		for (int j=0; j<way; j++)
		{
			if (recent[index][j] == -1)
			{
				recent[index][j] = 0;
				for (int c=0; c<j; c++)
				{
					(recent[index][c])++;
				}
				return j;
			}
			if (recent[index][j] == way-1)
			{
				least = j;
				break;
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
	int logSize = log2(sets);
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
		tag = addr>>logSize;
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
	}
	outFile << hit << "," << total;
}

int hotCold(int hit, int coldness[][toPass])
{
	if (hit == -1)
	{
		int checkIndex = 0;
		for (int h=0; h<9; h++)
		{
			if (coldness[h][checkIndex] == 0)
			{
				coldness[h][checkIndex] = 1;
				checkIndex = (checkIndex*2)+1;
			}
			else
			{
				coldness[h][checkIndex] = 0;
				checkIndex = checkIndex*2;
			}
		}
		return checkIndex;
	}
	else
	{
		int oneZero = 0;
		if (hit%2 == 1)
		{
			oneZero = 1;
			hit--;
		}
		hit /= 2;
		for (int g=8; g>=0; g--)
		{
			coldness[g][hit] = oneZero;
			if (hit%2 == 1)
			{
				oneZero = 1;
				hit--;
			}
			else
			{
				oneZero = 0;
			}
			hit /= 2;
		}
		return -1;
	}
}

void fullyHotCold(string fileName, ofstream& outFile, int way)
{
	ifstream inFile(fileName);

	int sets = 512/way;
	int cache[sets][toPass];
	int coldness[9][toPass];
	int logSize = log2(sets);
	for (int i=0; i<sets; i++)
	{
		for (int j=0; j<way; j++)
		{	
			cache[i][j] = -1;
		}
	}
	for (int a=0; a<9; a++)
	{
		for (int b=0; b<256; b++)
		{
			coldness[a][b] = 0;
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
		tag = addr>>logSize;
		int goodHit = 0;
		int LRUret = -1;

		for (int k=0; k<way; k++)
		{
			if (cache[index][k] == tag)
			{
				hit++;
				LRUret = hotCold(k, coldness);
				goodHit = 1;
				break;
			}
		}
		if (goodHit == 0)
		{
			LRUret = hotCold(-1, coldness);
			cache[index][LRUret] = tag;
		}
	}
	outFile << hit << "," << total;
}

void setNoWriteMiss(string fileName, ofstream& outFile, int way)
{
	ifstream inFile(fileName);

	int sets = 512/way;
	int cache[sets][way];
	int recent[sets][toPass];
	int logSize = log2(sets);
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
		tag = addr>>logSize;
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
		if (goodHit == 0 && ldstr == "L")
		{
			LRUret = LRU(index, way, -1, recent);
			cache[index][LRUret] = tag;
		}
	}
	outFile << hit << "," << total;
}

void setPrefetching(string fileName, ofstream& outFile, int way)
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

    	string ldstr;
    	int hit = 0; 
	int total = 0;
    	int logSize = log2(sets);
   	int index;
	int nIndex;
	int addrOff;
	int addr; 
	int tag; 
	int nTag;

    	while (inFile >> ldstr >> hex >> addrOff) 
	{
		total++;

        	addr = addrOff>>5;
        	index = addr%sets;
        	tag = addr>>logSize;
        	int goodHit = 0;
        	int nGoodHit = 0;
       	 	int LRUret = -1;

        	nIndex = (addr+1)%sets;
        	nTag = (addrOff+32)>>(logSize+5);
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
        	for (int k=0; k<way; k++) 
		{
            		if (cache[nIndex][k] == nTag) 
			{
                		LRUret = LRU(nIndex, way, k, recent);
                		nGoodHit = 1;
                		break;
            		}	
        	}
        	if (goodHit == 0) 
		{
            		LRUret = LRU(index, way, -1, recent);
            		cache[index][LRUret] = tag;
        	}
        	if (nGoodHit == 0) 
		{
            		LRUret = LRU(nIndex, way, -1, recent);
            		cache[nIndex][LRUret] = nTag;
        	}
    	}

    	outFile << hit << "," << total;
}

void setPrefetchingMiss(string fileName, ofstream& outFile, int way)
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

    	string ldstr;
    	int hit = 0;
	int total = 0;
    	int logSize = log2(sets);
    	int index;
	int nIndex;
	int addrOff;
	int addr;
	int tag;
	int nTag;

    	while (inFile >> ldstr >> hex >> addrOff) 
	{
		total++;

        	addr = addrOff>>5;
        	index = addr%sets;
        	tag = addr>>logSize;
        	int goodHit = 0;
        	int nGoodHit = 0;
        	int LRUret = -1;

        	nIndex = (addr+1)%sets;
        	nTag = (addrOff+32)>>(logSize+5);
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

            		for (int k=0; k<way; k++) 
			{
                		if (cache[nIndex][k] == nTag) 
				{
                    			LRUret = LRU(nIndex, way, k, recent);
                    			nGoodHit = 1;
                    			break;
                		}
            		}

            		if (nGoodHit == 0) 
			{
                		LRUret = LRU(nIndex, way, -1, recent);
                		cache[nIndex][LRUret] = nTag;
            		}
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
	outFile << "; ";
	setAssociative(fileName, outFile, 4);
	outFile << "; ";
	setAssociative(fileName, outFile, 8);
	outFile << "; ";
	setAssociative(fileName, outFile, 16);
	outFile << ";" << endl;

	setAssociative(fileName, outFile, 512); //fully associative LRU
	outFile << ";" << endl;
	fullyHotCold(fileName, outFile, 512);
	outFile << ";" << endl;

	setNoWriteMiss(fileName, outFile, 2);
	outFile << "; ";
	setNoWriteMiss(fileName, outFile, 4);
	outFile << "; ";
	setNoWriteMiss(fileName, outFile, 8);
	outFile << "; ";
	setNoWriteMiss(fileName, outFile, 16);
	outFile << ";" << endl;

	setPrefetching(fileName, outFile, 2);
	outFile << "; ";
	setPrefetching(fileName, outFile, 4);
	outFile << "; ";
	setPrefetching(fileName, outFile, 8);
	outFile << "; ";
	setPrefetching(fileName, outFile, 16);
	outFile << ";" << endl;

	setPrefetchingMiss(fileName, outFile, 2);
	outFile << "; ";
	setPrefetchingMiss(fileName, outFile, 4);
	outFile << "; ";
	setPrefetchingMiss(fileName, outFile, 8);
	outFile << "; "; 
	setPrefetchingMiss(fileName, outFile, 16);
	outFile << ";" << endl;

	outFile.close();


	return 0;
}
