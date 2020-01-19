#ifndef BCP_H
#define BCP_H

#include "PrGlib.h"
#include <iostream>
#include <string>

#define SIZE 256           /*多项式的长度*/
#define SIZE_prime 511

struct E {ZZZ A; ZZZ B;};

/*********** processing function *************/
void write_to_file(string path, string data);
void read_key_from_file(ZZZ& rop, string path);

/*********** BCP Algorithms ***************/
double BCP_Setup(int SecurityBit, int no);
double BCP_KeyGen(ZZZ N, ZZZ g, int no, int securitybit);
void BCP_Enc(struct E& e, ZZZ N, ZZZ g, ZZZ h, ZZZ m);
ZZZ BCP_Dec_1(ZZZ a, ZZZ N, struct E C);
ZZZ BCP_Dec_2(struct E C, ZZZ p_prime, ZZZ q_prime, ZZZ g, ZZZ k, ZZZ h, ZZZ N);
E Add_Homo_1(struct E e1, struct E e2, ZZZ N);
E Add_Homo_2(struct E e, ZZZ l, ZZZ N);
E Mul_Homo(struct E e1, struct E e2, ZZZ g, ZZZ h, ZZZ N, ZZZ p_prime, ZZZ q_prime, ZZZ k);

/************* Our Scheme Algorithms *****************/
double Setup(const char* outFileName, const char* FileName, std::vector<std::string>& KeysTable, int PolyNum, int MaxIdNum, int securitybit);
double TGen(const char* outName, int* SearchKeyIds, int SearchKeyNum, int MaxKeyNum, int PolyNum, int securitybit);
double TGen_API(const char* outName, const std::vector<std::string>& KeysTable, const std::vector<std::string>& SearchKeys, int MaxKeyNum, int PolyNum, int securitybit);
double Search(const char* outName, const char* epmName, const char* stName, int PolyNum, int MaxIdNum, int securitybit);
double SearchSum(E* out, const char* inName, int PolyNum, int PolyLen, int securitybit);
double Decrypt(ZZZ* out, E* in, ZZZ a, ZZZ N, int PolyLen);

#endif