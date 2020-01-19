//
// Created by Quinn Sheng on 2020/1/18.
//

#ifndef PAILLIERTYPE_SEARCH_H
#define PAILLIERTYPE_SEARCH_H


#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/time.h>

#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hmac.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

#include "PrGlib.h"
#include "libhcs.h"
#include "PolynomialMatrix.h"

void HomoMulit(
        pcs_public_key *pk,
        hcs_random *securityParam,
        int times,
        ZZZ rop);

ZZZ Add_Homo_1(ZZZ a, ZZZ b, pcs_public_key *pk);

ZZZ Add_Homo_2(mpz_t e, mpz_t l, pcs_public_key *pk);

void Search(const char *outName,
            const char *epmName,
            pcs_public_key *pk,
            vector<string> KeysHashTable,
            vector<string> searchToken,
            int PolyNum, int MaxIdNum,
            hcs_random *securityParam);

#endif //PAILLIERTYPE_SEARCH_H
