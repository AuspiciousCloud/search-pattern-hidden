//
// Created by Quinn Sheng on 2020/1/18.
//

#include "TokenGen.h"

vector<string> TokenGen(
        vector<string> searchKws,
        const unsigned char *hashSeed) {
    vector<string> hashTokens;

//    CryptoPP::SecByteBlock k(hashSeed, 16);
    CryptoPP::HMAC<CryptoPP::SHA256> f1(hashSeed, 16);
    string token;
    for (int i = 0; i < searchKws.size(); i++) {
//        cout << "Token #" << i << ": " << searchKws[i];
        try {
            CryptoPP::StringSource ss(
                    searchKws[i], true,
                    new CryptoPP::HashFilter(
                            f1, new CryptoPP::StringSink(token)
                    ) // HashFilter
            ); // StringSource
//            cout << " Hash " << token << endl;
            hashTokens.push_back(token);
        } catch (const CryptoPP::Exception &e) {
            cerr << e.what() << endl;
            exit(1);
        }
    }
    return hashTokens;
}