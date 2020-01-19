
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hmac.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "Kamara.h"

void Decrypt(std::vector<std::string>& ids, const std::vector<wbItem>& R, 
    unsigned char* k1, unsigned char* k2, unsigned char* ivstr)
{
    for (auto &&wbi : R)
    {
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
        // push id to search result container
        ids.push_back(pl[1]);
    }
}