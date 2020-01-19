#include "BCP.h"

E Add_Homo_1(struct E e1, struct E e2, ZZZ N){

        struct E e;
        ZZZ N2;

        N2 = N*N;

        e.A = (e1.A * e2.A) % N2;
        e.B = (e1.B * e2.B) % N2;

        return e;

}

E Add_Homo_2(struct E e, ZZZ l, ZZZ N){

        struct E e1;
        ZZZ N2;

        N2 = N*N;

        mpz_powm(e1.A.get_mpz_t(),e.A.get_mpz_t(),l.get_mpz_t(),N2.get_mpz_t());
        mpz_powm(e1.B.get_mpz_t(),e.B.get_mpz_t(),l.get_mpz_t(),N2.get_mpz_t());

        return e1;

}