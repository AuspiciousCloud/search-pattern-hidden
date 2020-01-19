//
// Created by Quinn Sheng on 2020/1/18.
//

#ifndef PAILLIERTYPE_TOKENGEN_H
#define PAILLIERTYPE_TOKENGEN_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "PrGlib.h"
#include <ctime>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hmac.h>
#include <cryptopp/filters.h>

vector<string> TokenGen(
        vector<string> searchKws,
        const unsigned char *hashSeed);

#endif //PAILLIERTYPE_TOKENGEN_H
