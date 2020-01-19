
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

#include <sys/time.h>

#include "Kamara.h"

void Setup(const std::vector<std::vector<std::string>>& wkp, std::vector<wbItem>& WordBlock, 
            std::vector<wbItem>& DummyBlock, 
            unsigned char* key1, unsigned char* key2, unsigned char* ivstr)
{
    WordBlock.clear();
    DummyBlock.clear();
    // generate k1, k2 randomly
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock k1(16), k2(16), iv(CryptoPP::AES::BLOCKSIZE);
    prng.GenerateBlock(k1, k1.size());
    prng.GenerateBlock(k2, k2.size());
    prng.GenerateBlock(iv, iv.size());
    
    for(int i = 0; i < wkp.size(); i++)
    {
        std::vector<std::string> idlist = wkp[i];
        // first substring is the word
        std::string w = idlist.front();
        // remove word, the remaining list is the id list
        idlist.erase(idlist.begin());
        // compute HMAC of word w
        std::string stag;
        try
        {
            CryptoPP::HMAC< CryptoPP::SHA256 > f1(k2, k2.size());

            CryptoPP::StringSource ss(w, true, 
                new CryptoPP::HashFilter(f1,
                    new CryptoPP::StringSink(stag)
                ) // HashFilter      
            ); // StringSource
        }
        catch(const CryptoPP::Exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }

        unsigned int count = 1;
        std::string e;
        for (auto &&id : idlist)
        {
            e = w + "||" + id;
            size_t messageLen = std::strlen(e.c_str()) + 1;

            //////////////////////////////////////////////////////////////////////////
            // Encrypt

            CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(k1, k1.size(), iv);
            CryptoPP::SecByteBlock eData(e.length());
            cfbEncryption.ProcessData(eData, (unsigned char*)e.c_str(), messageLen);

            std::string l;
            try
            {
                CryptoPP::HMAC< CryptoPP::SHA256 > f2((unsigned char*)stag.c_str(), stag.size());

                CryptoPP::StringSource ss2(std::to_string(count), true, 
                    new CryptoPP::HashFilter(f2,
                        new CryptoPP::StringSink(l)
                    ) // HashFilter      
                ); // StringSource
            }
            catch(const CryptoPP::Exception& e)
            {
                std::cerr << e.what() << std::endl;
                exit(1);
            }
            std::string encoded;
            CryptoPP::StringSource ss1(l, true,  
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(encoded)
                )
            );
            // push (l, e) into Word Block
            wbItem wbi;
            wbi.l = encoded;
            wbi.e = std::string((char *)eData.begin(), e.length());
            WordBlock.push_back(wbi);
            count++;
        }
    }
    // quick sort
    quicksort(WordBlock, 0, WordBlock.size() - 1);

    while (DummyBlock.size() < (int)sqrt(WordBlock.size()))
    {
        wbItem x;
        x.l = "12111";
        x.e = "12433355566";
        DummyBlock.push_back(x);
    }

    // store k1, k2 and iv
    int i = 0;
    for (auto &&byte : k1)
    {
        key1[i++] = byte;
    }
    i = 0;
    for (auto &&byte : k2)
    {
        key2[i++] = byte;
    }
    i = 0;
    for (auto &&byte : iv)
    {
        ivstr[i++] = byte;
    }
}
