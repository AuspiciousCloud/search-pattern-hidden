#include "Kamara.h"

#include <iostream>
#include <string>
#include <vector>
#include <sys/time.h>

using namespace std;

/*
compilation command:
g++ Process.cpp Kamara_Setup.cpp Kamara_TGen.cpp Kamara_Rebuild.cpp Kamara_Search.cpp Kamara_Decrypt.cpp Kamara_main.cpp -o Kamara_main.exe -lcryptopp -lpthread

run:
./Kamara_main.exe $word_id_pair_file_name$ $max_id_number$
*/

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "too few argument" << endl;
        exit(1);
    }
    unsigned char* k1 = new unsigned char[16];
    unsigned char* k2 = new unsigned char[16];
    unsigned char* iv = new unsigned char[16];

    std::cout << "ReadWords" << std::endl;
    vector<vector<string>> wkp;
    ReadWords(argv[1], wkp, stoi(argv[2]));

    std::cout << "Setup" << std::endl;
    vector<wbItem> WordBlock, DummyBlock;
    timeval start, end;
    gettimeofday(&start, 0);
    Setup(wkp, WordBlock, DummyBlock, k1, k2, iv);
    gettimeofday(&end, 0);
    cout << "setup time: " << 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000 << " ms" << endl;

    std::cout << "TGen" << std::endl;
    string token = TGen(k2, "link");

    std::cout << "Search" << std::endl;
    vector<wbItem> Shelter, R;

    gettimeofday(&start, 0);
    Search(token, R, WordBlock, Shelter, DummyBlock, k1, k2, iv);
    gettimeofday(&end, 0);
    std::cout << "Word Num: " << argv[2] << ", Time: " 
        << 1000*1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) << " us" << std::endl;
    
    std::cout << "Decrypt" << std::endl;
    vector<string> ids;
    Decrypt(ids, R, k1, k2, iv);
    std::cout << ids.size() << " id found:";
    for (auto &&id : ids)
    {
        cout << " " << id;
    }
    cout << endl;
    
    std::cout << "Finished" << std::endl;
    return 0;
}