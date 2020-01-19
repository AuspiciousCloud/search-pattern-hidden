#include "BCP.h"

/* *
 * Multiplicative homomorphic algorithm
 * */

ZZZ modular(ZZZ m, ZZZ n){

    ZZZ mod;

    if(m>=0)
        mod = m%n;
    else
    {
        int q =1;
        do{
            mod = m+n*q;
            q++;

        }while(mod<0);
    }

    return mod;

}

E Mul_Homo(struct E e1, struct E e2, ZZZ g, ZZZ h, ZZZ N, ZZZ p_prime, ZZZ q_prime, ZZZ k){

        /* cloud server */
        struct E e_mul;
        ZZZ N_prime;
        N_prime = p_prime * q_prime;
        ZZZ k_invert;
        mpz_invert(k_invert.get_mpz_t(),k.get_mpz_t(),N.get_mpz_t());
        ZZZ N2;
        N2 = N*N;

        ZZZ tao, tao_prime, tao_invert, tao_prime_invert;

        gmp_randclass rr(gmp_randinit_default);
        rr.seed(time(NULL));
        tao = rr.get_z_bits(5);

        rr.seed(time(NULL));
        tao_prime = rr.get_z_bits(5);

        tao_invert = modular (0-tao, N);
        tao_prime_invert = modular (0 - tao_prime, N);

        struct E e_tao_invert, e_tao_prime_invert, Z1, Z2;

        BCP_Enc(e_tao_invert, N, g, h, tao_invert);
        BCP_Enc(e_tao_prime_invert, N, g, h, tao_prime_invert);

        Z1 = Add_Homo_1(e1, e_tao_invert, N);
        Z2 = Add_Homo_1(e2, e_tao_prime_invert, N);

        /* auxiliary server */
        ZZZ m1_prime, m2_prime, m_prime;
        struct E C_prime;
        m1_prime = BCP_Dec_2(Z1, p_prime, q_prime, g, k, h, N);
        m2_prime = BCP_Dec_2(Z2, p_prime, q_prime, g, k, h, N);
        m_prime = (m1_prime * m2_prime)%N;
        BCP_Enc(C_prime, N, g, h, m_prime);

        /* cloud server */
        ZZZ tao_mul;
        tao_mul = modular (0 -tao*tao_prime, N);

        struct E T;
        BCP_Enc(T, N, g, h, tao_mul);
        ZZZ A1_exp, A2_exp, B1_exp, B2_exp;
        mpz_powm(A1_exp.get_mpz_t(),e1.A.get_mpz_t(),tao_prime.get_mpz_t(),N2.get_mpz_t());
        mpz_powm(A2_exp.get_mpz_t(),e2.A.get_mpz_t(),tao.get_mpz_t(),N2.get_mpz_t());
        mpz_powm(B1_exp.get_mpz_t(),e1.B.get_mpz_t(),tao_prime.get_mpz_t(),N2.get_mpz_t());
        mpz_powm(B2_exp.get_mpz_t(),e2.B.get_mpz_t(),tao.get_mpz_t(),N2.get_mpz_t());
        e_mul.A = (C_prime.A * A1_exp * A2_exp * T.A) % N2;
        e_mul.B = (C_prime.B * B1_exp * B2_exp * T.B) % N2;

        return e_mul;

}