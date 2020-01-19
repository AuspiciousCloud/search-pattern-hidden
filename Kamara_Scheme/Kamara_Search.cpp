
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

void Search(const std::string& token, std::vector<wbItem>& R, 
                std::vector<wbItem>& WordBlock, std::vector<wbItem>& Shelter, 
                std::vector<wbItem>& DummyBlock, 
                unsigned char* k1, unsigned char* k2, unsigned char* ivstr)
{
    R.clear();
    unsigned int c = 1;
    while(true)
    {
        CryptoPP::SecByteBlock key((unsigned char*)token.c_str(), token.size());
        std::string l;
        try
        {
            CryptoPP::HMAC< CryptoPP::SHA256 > f2(key, key.size());

            CryptoPP::StringSource ss(std::to_string(c), true, 
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
        std::string encode;
        CryptoPP::StringSource ss1(l, true,  
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(encode)
            )
        );
        l = encode;
        const wbItem* pos = nullptr;
        if(Shelter.size() != 0)
        {
            // pos = BiSearch(&(*Shelter.cbegin()), &(*Shelter.cend()), l);
            for (int i = 0; i < Shelter.size(); i++)
            {
                if (Shelter[i].l == l)
                {
                    pos = &Shelter[i];
                }
            }
        }
        if(pos != nullptr)
        {
            // save (l, e) in R
            R.push_back(*pos);
            // get an item from Dummy Block and abundant
            DummyBlock.pop_back();
        }
        else
        {
            // bisearch in wordblock
            const wbItem* wpos = nullptr;
            if(WordBlock.size() != 0)
            {
                wpos = BiSearch(&(*WordBlock.cbegin()), &(*WordBlock.cend()), l);
            }
            if(wpos != nullptr)
            {
                // l in word block
                R.push_back(*wpos);
                // append (l, e) to Shelter
                if(Shelter.size() >= (int)sqrt(WordBlock.size()))
                {
                    // Shelter size reach maximum value, rebuild
                    Rebuild(WordBlock, Shelter, DummyBlock, k1, k2, ivstr);
                }
                Shelter.push_back(*wpos);
            }
            else
            {
                // not found
                break;
            }
        }
        c++;
    }
}