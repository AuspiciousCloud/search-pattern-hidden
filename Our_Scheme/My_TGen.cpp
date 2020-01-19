/* *
 * TGen Algorithm in our scheme
 * For supporting searching using key word instead of its id number, we implement TGen_API for searching using key word
 * */

#include "BCP.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/time.h>

double TGen(const char* outName, int* SearchKeyIds, int SearchKeyNum, int MaxKeyNum, int PolyNum, int securitybit)
{
    ZZZ N;
    read_key_from_file(N, std::string("N.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    ZZZ g, h;
    read_key_from_file(g, std::string("g.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(h, std::string("Public_Key.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    ZZZ* tokensp = new ZZZ[MaxKeyNum];
    for(int i = 0; i < MaxKeyNum; i++)
    {
        tokensp[i] = 0;
    }

    timeval start, end;
    gettimeofday(&start, 0);
    gmp_randclass rr(gmp_randinit_default);
    rr.seed(time(NULL));
    for(int i = 0; i < SearchKeyNum; i++)
    {
        // select r in [1, N-1]
        tokensp[SearchKeyIds[i]] = rr.get_z_range(N - 1); // generate r in [0, N - 2]
        tokensp[SearchKeyIds[i]] = tokensp[SearchKeyIds[i]] + 1; 
    }
    E* tokensc = new E[MaxKeyNum];
    std::ofstream ofs(outName);
    if(!ofs)
    {
        std::cout << "<TGen> open '" << outName << "' error." << std::endl;
        abort();
    }
    for(int i = 0; i < MaxKeyNum; i++)
    {
        BCP_Enc(tokensc[i], N, g, h, tokensp[i]);
        ofs << tokensc[i].A << "," << tokensc[i].B << std::endl;
    }
    gettimeofday(&end, 0);
    double cost_time_msec = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;

    ofs.close();
    delete[] tokensp;
    delete[] tokensc;
    return cost_time_msec;
}

int FindId(const std::vector<std::string>& KeysTable, std::string sk)
{
    int i = 0;
    for (auto &&item : KeysTable)
    {
        if (item == sk)
        {
            return i;
        }
        else
        {
            i++;
        }
    }
    std::cout << "<FindId> error, can't find " << sk  << " in keys table." << std::endl;
    abort();
}

double TGen_API(const char* outName, const std::vector<std::string>& KeysTable, const std::vector<std::string>& SearchKeys, int MaxKeyNum, int PolyNum, int securitybit)
{
    int SearchKeyNum = SearchKeys.size();
    int* SearchKeyIds = new int[SearchKeyNum];
    int i = 0;
    for (auto &&sk : SearchKeys)
    {
        SearchKeyIds[i] = FindId(KeysTable, sk);
    }
    double time = TGen(outName, SearchKeyIds, SearchKeyNum, MaxKeyNum, PolyNum, securitybit);
    delete[] SearchKeyIds;
    return time;
}
