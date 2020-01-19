//
// Created by Quinn Sheng on 2020/1/18.
//

#ifndef PAILLIER_H
#define PAILLIER_H

#include "PrGlib.h"
#include <iostream>
#include <string>
#include "PolynomialMatrix.h"
#include "libhcs.h"

#define SIZE 256           /*多项式的长度*/
#define SIZE_prime 511

vector<int> Setup(
        const char *outFileName, const char *FileName,
        std::vector<std::string> &KeysTable,
        std::vector<std::string> &KeysHashTable,
        const unsigned char *hashSeed,
        int PolyNum, int MaxIdNum,
        ZZZ N,
        pcs_public_key *&pk,
        pcs_private_key *&vk,
        hcs_random *securityParam
);


#endif //PAILLIER_H
