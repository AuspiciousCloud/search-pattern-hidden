/* *
 * Search Algorithm of our scheme
 * */

#include "BCP.h"
#include "PolynomialMatrix.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sys/time.h>

#include <fstream>
#include <iostream>
#include <string>

double Search(const char* outName, const char* epmName, const char* stName, int PolyNum, int MaxIdNum, int securitybit)
{
    // read encrypted polynomial matrix
    std::ifstream ifs(epmName);
    if(!ifs)
    {
        std::cout << "<Search> open '" << epmName << "' error." << std::endl;
        abort();
    }
    int col = MaxIdNum + 1;
    E** ematrix = new E*[PolyNum];
    E* edata = new E[PolyNum * col];
    for(int i = 0; i < PolyNum; i++)
    {
        ematrix[i] = &edata[i * col];
    }

    std::string strline;
    for(int i = 0; i < PolyNum; i++)
    {
        std::getline(ifs, strline);
        std::vector<std::string> items;
        split(items, strline, " ");
        for(int j = 0; j < col; j++)
        {
            std::vector<std::string> elements;
            split(elements, items[j], ",");
            ematrix[i][j].A = elements[0];
            ematrix[i][j].B = elements[1];
        }
    }
    ifs.close();
    // read encrypted search token
    ifs.open(stName);
    if(!ifs)
    {
        std::cout << "<Search> open '" << stName << "' error" << std::endl;
        abort();
    }
    E* SearchToken = new E[PolyNum];
    for(int i = 0; i < PolyNum; i++)
    {
        std::getline(ifs, strline);
        std::vector<std::string> elements;
        split(elements, strline, ",");
        SearchToken[i].A = elements[0];
        SearchToken[i].B = elements[1];
    }
    ifs.close();
    // calculate encrypted polynomial matrix times encrypted search token
    ZZZ g, h, N, p_prime, q_prime, k;
    read_key_from_file(g, std::string("g.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(h, std::string("Public_Key.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(N, std::string("N.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(p_prime, std::string("p_prime.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(q_prime, std::string("q_prime.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    read_key_from_file(k, std::string("k.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");

    // generate random polynomial
    /* initialize random seed: */
    srand (time(NULL));
    // create digit plaintext
    ZZZ** dmatrix = new ZZZ*[PolyNum];
    ZZZ* ddata = new ZZZ[PolyNum * col];
    for(int i = 0; i < PolyNum; i++)
    {
        dmatrix[i] = &ddata[i * col];
    }
    for(int i = 0; i < PolyNum; i++)
    {
        /* generate random number between 0 and PolyNum: */
        int randpow = rand() % PolyNum;
        for(int j = 0; j < col; j++)
        {
            if(j >= PolyNum - randpow)
            {
                gmp_randclass rr(gmp_randinit_default);
                rr.seed(time(NULL));
                dmatrix[i][j] = rr.get_z_range(N - 1); // generate number in [0, N - 2]
                dmatrix[i][j] = dmatrix[i][j] + 1;  // number in [1, N - 1]
            }
            else
            {
                dmatrix[i][j] = 0;
            }
        }
    }
    
    timeval start, end;
    gettimeofday(&start, 0);

    #pragma omp parallel for
    for(int i = 0; i < PolyNum; i++)
    {
        for (int j = 0; j < col; j++)
        {
            ematrix[i][j] = Mul_Homo(ematrix[i][j], SearchToken[i], g, h, N, p_prime, q_prime, k);
        }
    }
    
    // calculate new encrypted polynomial matrix
    int newcol = col + col + 1;
    E** NewPolyMatrix = new E*[PolyNum];
    E* NewPolyData = new E[PolyNum * newcol];
    for(int i = 0; i < PolyNum; i++)
    {
        NewPolyMatrix[i] = &NewPolyData[i * newcol];
    }
    #pragma omp parallel for
    for(int i = 0; i < PolyNum; i++)
    {
        int dcol = col;
        int count = 0;
        while(dmatrix[count] == 0) { count++; }
        dcol -= count;
        for(int j = 0; j < newcol; j++)
        {
            NewPolyMatrix[i][j].A = 1;
            NewPolyMatrix[i][j].B = 1;
            for(int k = 0; k <= j; k++)
            {
                if(k < col && j - k < dcol)
                {
                    NewPolyMatrix[i][j] = Add_Homo_1(NewPolyMatrix[i][j], Add_Homo_2(ematrix[i][k], dmatrix[i][j - k], N), N);
                }
            }
        }
    }

    gettimeofday(&end, 0);
    double cost_time_msec = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;
    
    std::ofstream ofs(outName);
    if(!ofs)
    {
        std::cout << "<Search> open '" << outName << "' error." << std::endl;
        abort();
    }
    for(int i = 0; i < PolyNum; i++)
    {
        for(int j = 0; j < newcol; j++)
        {
            ofs << (j ? " " : "") << NewPolyMatrix[i][j].A << "," << NewPolyMatrix[i][j].B;
        }
        ofs << std::endl;
    }
    ofs.close();
    
    delete[] ematrix;
    delete[] edata;
    delete[] SearchToken;
    delete[] dmatrix;
    delete[] ddata;
    delete[] NewPolyMatrix;
    delete[] NewPolyData;

    return cost_time_msec;
}

double SearchSum(E* out, const char* inName, int PolyNum, int PolyLen, int securitybit)
{
    E** PolyMatrix = new E*[PolyNum];
    E* PolyData = new E[PolyNum * PolyLen];
    for (int i = 0; i < PolyNum; i++)
    {
        PolyMatrix[i] = &PolyData[i * PolyLen];
    }
    
    std::ifstream ifs(inName);
    if(!ifs)
    {
        std::cout << "<SearchSum> open '" << inName << "' error." << std::endl;
        abort();
    }
    std::string strline;
    for (int i = 0; i < PolyNum; i++)
    {
        std::getline(ifs, strline);
        std::vector<std::string> items;
        split(items, strline, " ");
        for (int j = 0; j < PolyLen; j++)
        {
            std::vector<std::string> elements;
            split(elements, items[j], ",");
            PolyMatrix[i][j].A = elements[0];
            PolyMatrix[i][j].B = elements[1];
        }
    }
    ZZZ N;
    read_key_from_file(N, std::string("N.") + to_string(securitybit) + "." + to_string(PolyNum) + ".txt");
    timeval start, end;
    gettimeofday(&start, 0);
    for (int i = 0; i < PolyLen; i++)
    {
        out[i].A = 1;
        out[i].B = 1;
        for (int j = 0; j < PolyNum; j++)
        {
            out[i] = Add_Homo_1(out[i], PolyMatrix[j][i], N);
        }
    }
    gettimeofday(&end, 0);
    double cost_time_msec = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;
    return cost_time_msec;
}
