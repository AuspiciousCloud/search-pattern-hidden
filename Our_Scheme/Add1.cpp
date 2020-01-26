#include "BCP.h"

E Add_Homo_1(struct E e1, struct E e2, ZZZ N){

        struct E e;
        ZZZ N2;

        N2 = N*N;

        e.A = (e1.A * e2.A) % N2;
        e.B = (e1.B * e2.B) % N2;

        return e;

}
