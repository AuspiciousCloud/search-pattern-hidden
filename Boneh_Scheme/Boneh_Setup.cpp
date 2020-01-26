/* *
 * Setup Algorithm in our scheme
 * */

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


/**
 * return Array(Index[0] of Every polynomial)
 * @param outFileName
 * @param FileName
 * @param KeysTable
 * @param KeysHashTable
 * @param hashKey
 * @param PolyNum
 * @param MaxIdNum
 * @param N
 * @param pk
 * @param vk
 * @param securityParam
 * @return
 */
vector<int> Setup(const char *outFileName,
                  const char *FileName,
                  std::vector<std::string> &KeysTable,
                  std::vector<std::string> &KeysHashTable,
                  const unsigned char *hashSeed,
                  int PolyNum, int MaxIdNum,
                  ZZZ N,
                  pcs_public_key *&pk,
                  pcs_private_key *&vk,
                  hcs_random *securityParam) {
    // PCS Setup | initialize data structures
    pk = pcs_init_public_key();
    vk = pcs_init_private_key();
    // Generate a key pair with modulus of size 256 bits(IV)
    pcs_generate_key_pair(pk, vk, securityParam, 256);

//    cout << pk->n << endl;
    //  N assign
    string tmpStr = mpz_get_str(NULL, 10, pk->n);
    N = tmpStr;
//    cout << "START" << endl;
//    cout << N.get_mpz_t() << endl;
//    cout << "END" << endl;

    vector<int> indiceL;
    int col = MaxIdNum + 1;
    // create E matrix; indiceL added
    //  TIPS: origin E<ZZZ a, ZZZ b>
    ZZZ **ematrix = new ZZZ *[PolyNum];
    ZZZ *edata = new ZZZ[PolyNum * col];
    for (int i = 0; i < PolyNum; i++) {
        indiceL.push_back(i * col);
        ematrix[i] = &edata[i * col];
    }

    std::ifstream ifs(FileName);
    if (!ifs) {
        std::cout << "<Setup> open file '" << FileName << "' error." << std::endl;
        abort();
    }

    CryptoPP::HMAC<CryptoPP::SHA256> f1(hashSeed, 16);

    double total = 0.0;
    for (int i = 0; i < PolyNum; i++) {
        string hash = "";
        std::vector<std::string> idlist;
        std::string idstr;
        std::getline(ifs, idstr);
        split(idlist, idstr, std::string(" "));
        // insert key into table
        KeysTable.push_back(idlist.front());
        //  insert hashed key
        try {
//            cout << "Setup Kw: " << idlist.front() ;
            CryptoPP::StringSource ss(
                    idlist.front(), true,
                    new CryptoPP::HashFilter(
                            f1, new CryptoPP::StringSink(hash)
                    ) // HashFilter
            ); // StringSource
//            cout << " Hash: " << hash << endl;
            KeysHashTable.push_back(hash);
        } catch (const CryptoPP::Exception &e) {
            cerr << e.what() << endl;
            exit(1);
        }


        // remove key
        idlist.erase(idlist.begin());
        int polyc = 0;
        //  create file ids
        ZZZ *ids = new ZZZ[MaxIdNum];
        for (auto it = idlist.cbegin(); it != idlist.cend(); polyc++, it++) {
            ids[polyc] = (*it).c_str();
        }
        //  ATTENTION 填充0
        while (polyc < MaxIdNum) {
            ids[polyc++] = 0;
        }

        // compute
//        cout << "Start" << endl;
        polynomial result = One(N);
//        cout << "End" << endl;
        ZZZ icoef[2] = {0, 1};
        for (int j = 0; j < MaxIdNum; j++) {
            icoef[0] = ids[j];
            result = result * polynomial(icoef, 2, N);
        }
        timeval start, end;
        gettimeofday(&start, 0);
        for (int j = 0; j < col; j++) {
            //  ATTENTION BCP \to Paillier
            pcs_encrypt(pk, securityParam, ematrix[i][j].get_mpz_t(), result.coef(MaxIdNum - j).get_mpz_t());
        }
        gettimeofday(&end, 0);
        total += 1000 * 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    }
    ifs.close();

    //  return hash Gen Key
//    for (auto &&byte : k) {
//        hashSeed[i++] = byte;
//    }
//    cout << "Start HASHGenKey ";
//    cout << hashSeed << endl;
//    cout << "END" << endl;

    std::ofstream ofs(outFileName);
    if (!ofs) {
        std::cout << "<Setup> open '" << outFileName << "' error." << std::endl;
        abort();
    }
    for (int i = 0; i < PolyNum; i++) {
//        cout << "Poly # " << i << endl;
        for (int j = 0; j < col; j++) {
            ofs << (j ? " " : "") << ematrix[i][j];
        }
        ofs << std::endl;
    }
    ofs.close();
    delete[] ematrix;
    delete[] edata;
    cout << "Time used ";
    cout << total / 1000 << "ms" << endl;

    return indiceL;
}
