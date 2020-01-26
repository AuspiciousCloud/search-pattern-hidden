//
// Created by Quinn Sheng on 2020/1/18.
//

#include "PrGlib.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include "Boneh_Setup.h"
#include "Boneh_TGen.h"
#include "Boneh_Search.h"
#include "Boneh_Decrypt.h"

using namespace std;

int main(int argc, char *argv[]) {
    const int securitybit[1] = {128};

    const int polyNum[5] = {420, 700, 874, 1031, 1156};
    const int MaxIdNum[5] = {50, 59, 72, 80, 88};
    const char *inFile[5] = {"newtext.001.txt", "newtext.002.txt", "newtext.003.txt", "newtext.004.txt",
                             "newtext.005.txt"};
    const char *SearchKey[5] = {"account", "account", "account", "account", "account"};
    const int repeatTime = 10;
    double timeused = 0;

    //  Mod
    pcs_public_key *pk;
    pcs_private_key *vk;
    hcs_random *securityParam = hcs_init_random();

    for (int sb = 0; sb < 1; sb++) {
        for (int pn = 0; pn < 5; pn++) {
            for (size_t i = 0; i < repeatTime; i++) {
                vector<string> KeysTable;
                vector<string> KeysHashTable;
                vector<int> indiceL;
                char hashSeed[17] = "191A47EC4465DD95";
                int WordNum = polyNum[pn];
                cout << "set up" << endl;
                string setupName = (string("Setup.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) +
                                    ".txt");
                Setup(setupName.c_str(),
                      inFile[pn],
                      KeysTable,
                      KeysHashTable,
                      reinterpret_cast<const unsigned char *>(hashSeed),
                      WordNum,
                      MaxIdNum[pn],
                      N, pk, vk, securityParam);

//                cout << "setted up" << endl;
                vector<string> SearchKeys;
                SearchKeys.push_back(SearchKey[pn]);
                cout << "token generating" << endl;
                vector<string> tokens;
                tokens = TokenGen(SearchKeys, reinterpret_cast<const unsigned char *>(hashSeed));

//                cout << "token generated" << endl;

                ZZZ* result = new ZZZ[MaxIdNum[pn] * 2 + 1];
                cout << "searching" << endl;
                Search((string("Search.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) +
                        ".txt").c_str(),
                       (string("Setup.out.") + to_string(securitybit[sb]) + "." + to_string(polyNum[pn]) +
                        ".txt").c_str(),
                       pk,
                       KeysHashTable,
                       tokens,
                       WordNum, MaxIdNum[pn],
                       securityParam
                       result);
                    //    securityParam);
                cout << "Searching END" << endl;
                cout << "Decrypt" << endl;
                ZZZ* out = new ZZZ[MaxIdNum[pn] * 2 + 1];
                Decrypt(result, out, vk, MaxIdNum[pn] * 2 + 1);
            }
        }
    }

    return 0;
}
