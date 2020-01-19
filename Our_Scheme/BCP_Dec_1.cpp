#include "BCP.h"
#include <stdlib.h>

using namespace std;

ZZZ BCP_Dec_1(ZZZ a, ZZZ N, struct E C)
{
    ZZZ A_a, m, A_a_invert, N2; 
    N2 = N*N;
    mpz_powm(A_a.get_mpz_t(),C.A.get_mpz_t(),a.get_mpz_t(),N2.get_mpz_t());
    mpz_invert(A_a_invert.get_mpz_t(),A_a.get_mpz_t(),N2.get_mpz_t());
    m = ((C.B*A_a_invert -1)%N2)/N;
    return m;
}