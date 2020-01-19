/* *
 * Testing program of our scheme
 * compilation command:
g++ PrGlib.cpp PolynomialMatrix.cpp BCP_Setup.cpp BCP_KeyGen.cpp BCP_Enc.cpp BCP_Dec_1.cpp BCP_Dec_2.cpp Add_Homo.cpp Mul_Homo.cpp Setup.cpp TGen.cpp Search.cpp Decrypt.cpp BCP_Test_main.cpp -lgmpxx -lgmp -lpthread -lntl -fopenmp -o BCP_Test_main.exe
 * run:
./BCP_Test_main.exe
* */

#include "PrGlib.h"
#include "BCP.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    const int securitybit[1] = { 128 };
    const int polyNum[1] = { 420 };
    const int MaxIdNum[1] = { 50 };
    const char* inFile[1] = { "newtext.001.txt" };
    const char* SearchKey[1] = { "list" };
    const int repeatTime = 1;
    for (int sb = 0; sb < 1; sb++)
    {
        for (int pn = 0; pn < 1; pn++)
        {
            for (size_t i = 0; i < repeatTime; i++)
            {
                vector<string> KeysTable;
                int WordNum = polyNum[pn];
                // cout << "setting up" << endl;
                Setup((string("Setup.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt").c_str(), 
                    inFile[pn], KeysTable, WordNum, MaxIdNum[pn], securitybit[sb]);
                ZZZ N, g;
                read_key_from_file(N, string("N.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt");
                read_key_from_file(g, string("g.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt");

                // cout << "set up" << endl;
                vector<string> SearchKeys;
                SearchKeys.push_back(SearchKey[pn]);
                // cout << "token generating" << endl;
                TGen_API((string("TGen.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt").c_str(), 
                    KeysTable, SearchKeys, WordNum, polyNum[pn], securitybit[sb]);
                // cout << "token generated" << endl;
                // cout << "searching" << endl;
                Search((string("Search.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt").c_str(), 
                    (string("Setup.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt").c_str(), 
                    (string("TGen.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt").c_str(), 
                    WordNum, 
                    MaxIdNum[pn], 
                    securitybit[sb]
                );
                E* result = new E[MaxIdNum[pn] * 2 + 1];
                SearchSum(result, (string("Search.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) + ".txt").c_str(), 
		            WordNum, MaxIdNum[pn] * 2 + 1, 
                    securitybit[sb]
		        );
                //save result
                ofstream ofs(string("SearchSum.") + 
                    to_string(securitybit[sb]) + "." + 
                    to_string(polyNum[pn]) + ".txt"
                );
                if(!ofs)
                {
                    cout << "<main> open '" << string("SearchSum.") + 
                    to_string(securitybit[sb]) + "." + 
                    to_string(polyNum[pn]) + ".txt"
                    << "' error" << endl;
                    abort();
                }
                for(int i = 0; i < MaxIdNum[pn] * 2 + 1; i++)
                {
                    ofs << result[i].A << "," << result[i].B << endl;
                }
                ofs.close();

                // cout << "Decrypting..." << endl;
                ZZZ a;
                read_key_from_file(a, (string("Private_Key.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) +  ".txt").c_str());
                ZZZ* out = new ZZZ[MaxIdNum[pn] * 2 + 1];
                Decrypt(out, result, a, N, MaxIdNum[pn] * 2 + 1);
                // cout << "Decrypted" << endl;
                delete[] result;
                delete[] out;
            }
        }
    }

    return 0;
}