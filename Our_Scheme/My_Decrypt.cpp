#include "BCP.h"
#include "PolynomialMatrix.h"

#include <sys/time.h>

/* *
 * Decrypt Algorithm in our scheme
 * decrypt search results
 * */
double Decrypt(ZZZ* out, E* in, ZZZ a, ZZZ N, int PolyLen)
{
    double time_msec = 0.0;
    timeval start, end;
    gettimeofday(&start, 0);
    for(int i = 0; i < PolyLen; i++)
    {
        out[i] = BCP_Dec_1(a, N, in[i]);
    }
    gettimeofday(&end, 0);
    time_msec = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;
    return time_msec;
}