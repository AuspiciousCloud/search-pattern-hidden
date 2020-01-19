#include "BCP.h"
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

void BCP_Enc(struct E& e, ZZZ N, ZZZ g, ZZZ h, ZZZ m)
{
    ZZZ r, h_r, N2;
    N2 = N*N;

    gmp_randclass rr(gmp_randinit_default);
    rr.seed(time(NULL));
    r = rr.get_z_range(N2 - 1); // generate r in [0, N^2 - 2]
    r = r + 1;  // r in [1, N^2 - 1]

    mpz_powm(e.A.get_mpz_t(),g.get_mpz_t(),r.get_mpz_t(),N2.get_mpz_t());
    mpz_powm(h_r.get_mpz_t(),h.get_mpz_t(),r.get_mpz_t(),N2.get_mpz_t());
    e.B = (h_r * (1+m*N)) % N2;
}
