/* *
 * Setup Algorithm in our scheme
 * */

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/time.h>

#include "BCP.h"
#include "PolynomialMatrix.h"

double Setup(const char* outFileName, const char* FileName, std::vector<std::string>& KeysTable, int PolyNum, int MaxIdNum, int securitybit)
{
    BCP_Setup(securitybit, PolyNum);
    ZZZ N, g;
    read_key_from_file(N, string("N.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(g, string("g.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    BCP_KeyGen(N, g, PolyNum, securitybit);
    // read public key from file.
    ZZZ h;
    read_key_from_file(N, std::string("N.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(g, std::string("g.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(h, std::string("Public_Key.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");

    int col = MaxIdNum + 1;
    // create E matrix
    E** ematrix = new E*[PolyNum];
    E* edata = new E[PolyNum * col];
    for(int i = 0; i < PolyNum; i++)
    {
        ematrix[i] = &edata[i * col];
    }

    std::ifstream ifs(FileName);
    if(!ifs)
    {
        std::cout << "<Setup> open file '" << FileName << "' error." << std::endl;
        abort();
    }

    double total = 0.0;
    for(int i = 0; i < PolyNum; i++)
    {
        std::vector<std::string> idlist;
        std::string idstr;
        std::getline(ifs, idstr);
        split(idlist, idstr, std::string(" "));
        // insert key into table
        KeysTable.push_back(idlist.front());
        // remove key
        idlist.erase(idlist.begin());
        int polyc = 0;
        ZZZ* ids = new ZZZ[MaxIdNum];
        for(auto it = idlist.cbegin(); it != idlist.cend(); polyc++, it++)
        {
            ids[polyc] = (*it).c_str();
        }
        while(polyc < MaxIdNum)
        {
            ids[polyc++] = 0;
        }

        
        // compute
        polynomial result = One(N);
        ZZZ icoef[2] = {0, 1};
        for(int j = 0; j < MaxIdNum; j++)
        {
            icoef[0] = ids[j];
            result = result * polynomial(icoef, 2, N);
        }
        timeval start, end;
        gettimeofday(&start, 0);
        for(int j = 0; j < col; j++)
        {
            BCP_Enc(ematrix[i][j], N, g, h, result.coef(MaxIdNum - j));
        }
        gettimeofday(&end, 0);
        total += 1000*1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    }
    ifs.close();
    
    std::ofstream ofs(outFileName);
    if(!ofs)
    {
        std::cout << "<Setup> open '" << outFileName << "' error." << std::endl;
        abort();
    }
    for(int i = 0; i < PolyNum; i++)
    {
        for(int j = 0; j < col; j++)
        {
            ofs << (j ? " " : "") << ematrix[i][j].A << "," << ematrix[i][j].B;
        }
        ofs << std::endl;
    }
    ofs.close();
    delete[] ematrix;
    delete[] edata;
    return total/1000;
}