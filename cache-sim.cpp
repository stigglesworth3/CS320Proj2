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

    int sets = 512 / way;
    int cache[sets][way];
    int recent[sets][toPass];
    for (int i = 0; i < sets; i++) {
        for (int j = 0; j < way; j++) { 
            cache[i][j] = -1;
            recent[i][j] = -1;
        }
    }

    string str;
    int hit = 0, total = 0;
    int log_size = log2(sets);
    int index, next_index, full_addr, addr, tag, next_tag;

    while (inFile >> str >> hex >> full_addr) {
        addr = full_addr >> 5;
        index = addr % sets;
        tag = addr >> log_size;
        int in_cache = 0;
        int next_in_cache = 0;
        int lru_ret = -1;

        next_index = (addr + 1) % sets;
        next_tag = (full_addr + 32) >> (log_size + 5);
        for (int k = 0; k < way; k++) {
            if (cache[index][k] == tag) {
                hit++;
                lru_ret = LRU(index, way, k, recent);
                in_cache = 1;
                break;
            }
        }
        for (int k = 0; k < way; k++) {
            if (cache[next_index][k] == next_tag) {
                lru_ret = LRU(next_index, way, k, recent);
                next_in_cache = 1;
                break;
            }
        }
        if (in_cache == 0) {
            lru_ret = LRU(index, way, -1, recent);
            cache[index][lru_ret] = tag;
        }
        if (next_in_cache == 0) {
            lru_ret = LRU(next_index, way, -1, recent);
            cache[next_index][lru_ret] = next_tag;
        }

        total++;
    }

    outFile << hit << "," << total;
}

void setPrefetchingMiss(string fileName, ofstream& outFile, int way)
{
	ifstream inFile(fileName);

    int sets = 512 / way;
    int cache[sets][way];
    int recent[sets][toPass];
    for (int i = 0; i < sets; i++) {
        for (int j = 0; j < way; j++) { 
            cache[i][j] = -1;
            recent[i][j] = -1;
        }
    }

    string str;
    int hit = 0, total = 0;
    int log_size = log2(sets);
    int index, next_index, full_addr, addr, tag, next_tag;

    while (inFile >> str >> hex >> full_addr) {
        addr = full_addr >> 5;
        index = addr % sets;
        tag = addr >> log_size;
        int in_cache = 0;
        int next_in_cache = 0;
        int lru_ret = -1;

        next_index = (addr + 1) % sets;
        next_tag = (full_addr + 32) >> (log_size + 5);
        for (int k = 0; k < way; k++) {
            if (cache[index][k] == tag) {
                hit++;
                lru_ret = LRU(index, way, k, recent);
                in_cache = 1;
                break;
            }
        }
        if (in_cache == 0) {
            lru_ret = LRU(index, way, -1, recent);
            cache[index][lru_ret] = tag;

            for (int k = 0; k < way; k++) {
                if (cache[next_index][k] == next_tag) {
                    lru_ret = LRU(next_index, way, k, recent);
                    next_in_cache = 1;
                    break;
                }
            }

            if (next_in_cache == 0) {
                lru_ret = LRU(next_index, way, -1, recent);
                cache[next_index][lru_ret] = next_tag;
            }
        }

        total++;
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
