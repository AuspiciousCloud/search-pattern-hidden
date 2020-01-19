#include "BCP.h"

#include <sys/time.h>

void write_to_file(string path, string data)
{
    ofstream ofs;
    ofs.open(path, ofstream::out);
    if(!ofs)
    {
        cout << "open '" << path << "' error" << endl;
        abort();
    }
    ofs.write(data.c_str(), data.length());
    ofs.close();
}

void read_key_from_file(ZZZ& rop, string path)
{
    ifstream ifs;
    ifs.open(path, fstream::in);
    if(!ifs)
    {
        cout << "open '" << path << "' error" << endl;
        abort();
    }
    string str;
    getline(ifs, str);
    rop = str.c_str();
    ifs.close();
}

double BCP_Setup(int SecurityBit, int no)
{
    timeval start, end;
    gettimeofday(&start, 0);
    /*Generate strong prime*/
    PrGlib a;
    ZZZ p, q, g, k, p_prime, q_prime, N, N2, N_prime, order_g;
    
    p = a.PrG_generate_strong_prime(SecurityBit/2);

    q = a.PrG_generate_strong_prime(SecurityBit/2);

    p_prime = (p-1)/2;
    q_prime = (q-1)/2;

    N = p*q;
    N2 = N*N;
    N_prime = p_prime * q_prime; 
    order_g = N*N_prime;

    ZZZ test;

    do{
    
    gmp_randclass rr(gmp_randinit_default);
    rr.seed(time(NULL));
    g = rr.get_z_range(N2 - 1); // generate g in [0, N^2 - 2]
    g = g + 1;  // g in [1, N^2 - 1]
    mpz_powm(test.get_mpz_t(),g.get_mpz_t(),order_g.get_mpz_t(),N2.get_mpz_t());

    }while(test!=1); 

    ZZZ K;
    mpz_powm(K.get_mpz_t(),g.get_mpz_t(),N_prime.get_mpz_t(),N2.get_mpz_t());
    k = (K-1)/N;

    gettimeofday(&end, 0);
    double costtime = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;

    write_to_file(string("N.") + to_string(SecurityBit) + "." + to_string(no) + ".txt", N.get_str());
    write_to_file(string("p_prime.") + to_string(SecurityBit) + "." + to_string(no) + ".txt", p_prime.get_str());
    write_to_file(string("q_prime.") + to_string(SecurityBit) + "." + to_string(no) + ".txt", q_prime.get_str());
    write_to_file(string("g.") + to_string(SecurityBit) + "." + to_string(no) + ".txt", g.get_str());
    write_to_file(string("k.") + to_string(SecurityBit) + "." + to_string(no) + ".txt", k.get_str());
    return costtime;
}