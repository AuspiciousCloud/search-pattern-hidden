
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

std::string TGen(const unsigned char* k2, const char* w)
{
    CryptoPP::SecByteBlock key(k2, 16);

    // token
    std::string token;
    try
    {
        CryptoPP::HMAC< CryptoPP::SHA256 > f1(key, key.size());

        CryptoPP::StringSource ss(w, true, 
            new CryptoPP::HashFilter(f1, 
                new CryptoPP::StringSink(token)
            ) // HashFilter      
        ); // StringSource
    }
    catch(const CryptoPP::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return token;
}
