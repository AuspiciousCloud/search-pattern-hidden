#ifndef PAILLIERTYPE_DECRYPT_H
#define PAILLIERTYPE_DECRYPT_H

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

void Decrypt(ZZZ* out, ZZZ* in, pcs_private_key *vk, int PolyLen);

#endif //PAILLIERTYPE_DECRYPT_H
