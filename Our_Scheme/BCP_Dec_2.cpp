#include "BCP.h"
#include <stdlib.h>

using namespace std;

ZZZ BCP_Dec_2(struct E C, ZZZ p_prime, ZZZ q_prime, ZZZ g, ZZZ k, ZZZ h, ZZZ N)
{
    
    ZZZ N2 = N*N;
    ZZZ N_prime;
    N_prime = p_prime * q_prime;

    ZZZ k_invert;
    mpz_invert(k_invert.get_mpz_t(),k.get_mpz_t(),N.get_mpz_t());

    ZZZ h_prime, a_prime, A_prime, r_prime; 
    mpz_powm(h_prime.get_mpz_t(),h.get_mpz_t(),N_prime.get_mpz_t(),N2.get_mpz_t());
    a_prime = (((h_prime -1)/N)*k_invert) % N; 
    mpz_powm(A_prime.get_mpz_t(),C.A.get_mpz_t(),N_prime.get_mpz_t(),N2.get_mpz_t());
    r_prime = (((A_prime -1)/N)*k_invert) % N; 
    ZZZ gamma, pi, g_gamma, g_gamma_invert, B_devide_g_gamma, m, B_devide_g_gamma_prime;

    gamma = (a_prime * r_prime) % N;
    mpz_invert(pi.get_mpz_t(),N_prime.get_mpz_t(),N.get_mpz_t());
    mpz_powm(g_gamma.get_mpz_t(),g.get_mpz_t(),gamma.get_mpz_t(),N2.get_mpz_t());
    mpz_invert(g_gamma_invert.get_mpz_t(),g_gamma.get_mpz_t(),N2.get_mpz_t());
    B_devide_g_gamma = C.B*g_gamma_invert;
    mpz_powm(B_devide_g_gamma_prime.get_mpz_t(),B_devide_g_gamma.get_mpz_t(),N_prime.get_mpz_t(),N2.get_mpz_t());
    m = ((((B_devide_g_gamma_prime -1) % N2 ) / N) * pi ) % N; 

    return m;
}