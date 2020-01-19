
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hmac.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "Kamara.h"

void Rebuild(std::vector<wbItem>& WordBlock, std::vector<wbItem>& Shelter, 
                std::vector<wbItem>& DummyBlock, 
                unsigned char* k1, unsigned char* k2, unsigned char* ivstr)
{
    Shelter.clear();
    std::string prew = "";
    int wi = -1, idi = 0;
    std::vector<std::vector<std::string>> wkp;
    std::vector<wbItem> temp;
    for (auto &&wbi : WordBlock)
    {
        // download all (l, e) in the Word Block
        // decrypt e to obtain w and id.
        //////////////////////////////////////////////////////////////////////////
        // Decrypt

        CryptoPP::SecByteBlock key(k1, 16);
        CryptoPP::SecByteBlock iv(ivstr, CryptoPP::AES::BLOCKSIZE);
        CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(key, key.size(), iv);
        CryptoPP::SecByteBlock plainData(wbi.e.length());
        cfbDecryption.ProcessData(plainData, (unsigned char*)wbi.e.c_str(), wbi.e.length());
        // split w and id
        std::vector<std::string> pl;
        std::string plainText((char*)plainData.begin(), plainData.size());
        split(pl, plainText, std::string("||"));
        
        // store w and id
        wbItem item;
        item.l = pl[0];
        item.e = pl[1];
        temp.push_back(item);
    }
    // re-organize
    // quick sort
    quicksort(temp, 0, temp.size() - 1);
    std::string pre("");
    std::vector<std::string> linedata;
    wkp.clear();
    for (auto &&item : temp)
    {        
        if (item.l != pre)
        {
            if (pre != "")
            {
                wkp.push_back(linedata);
                pre = item.l;
            }
            linedata.clear();
            linedata.push_back(item.l);
            linedata.push_back(item.e);
        }
        else
        {
            linedata.push_back(item.e);
            if (item.l == temp.end()->l && item.e == temp.end()->e)
            {
                wkp.push_back(linedata);
            }
            
        }
    }
    // re-setup
    Setup(wkp, WordBlock, DummyBlock, k1, k2, ivstr);
}