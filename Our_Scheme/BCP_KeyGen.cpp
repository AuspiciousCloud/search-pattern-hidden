#include "BCP.h"
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

double BCP_KeyGen(ZZZ N, ZZZ g, int no, int securitybit)
{
    timeval start, end;
    gettimeofday(&start, 0);
    ZZZ N2;
    N2 = N*N;
    ZZZ a, h;
    gmp_randclass rr(gmp_randinit_default);
    rr.seed(time(NULL));
    a = rr.get_z_range(N2 - 1); // generate a in [0, N^2 - 2]
    a = a + 1;  // a in [1, N^2 - 1]

    mpz_powm(h.get_mpz_t(),g.get_mpz_t(),a.get_mpz_t(),N2.get_mpz_t());
    gettimeofday(&end, 0);
    double costtime = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;

    write_to_file(string("Private_Key.") + to_string(securitybit) + "." + to_string(no) + ".txt", a.get_str());
    write_to_file(string("Public_Key.") + to_string(securitybit) + "." + to_string(no) + ".txt", h.get_str());
    return costtime;
}