#include "PrGlib.h"
#include "libhcs.h"
#include "PolynomialMatrix.h"

#include <sys/time.h>

/* *
 * Decrypt Algorithm in Boneh scheme
 * decrypt search results
 * */
void Decrypt(ZZZ* in, ZZZ* out, pcs_private_key *vk, int PolyLen)
{
    double time_msec = 0.0;
    timeval start, end;
    gettimeofday(&start, 0);
    for(int i = 0; i < PolyLen; i++)
    {
        pcs_decrypt(vk, in[i].get_mpz_t(), out[i].get_mpz_t());
    }
    gettimeofday(&end, 0);
    time_msec = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;
    cout << "Decrypt: ";
    cout << time_msec << endl;
}
