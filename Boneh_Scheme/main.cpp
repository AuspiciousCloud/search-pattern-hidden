//
// Created by Quinn Sheng on 2020/1/17.
//

#include <gmp.h>    // gmp is included implicitly
#include <libhcs.h> // master header includes everything

int main(void)
{
    // initialize data structures
    pcs_public_key *pk = pcs_init_public_key();
    pcs_private_key *vk = pcs_init_private_key();
    hcs_random *hr = hcs_init_random();

    // Generate a key pair with modulus of size 2048 bits
    pcs_generate_key_pair(pk, vk, hr, 2048);

    // libhcs works directly with gmp mpz_t types, so initialize some
    mpz_t a, b, c;
    mpz_inits(a, b, c, NULL);

    mpz_set_ui(a, 50);
    mpz_set_ui(b, 76);

    gmp_printf("Before\na = %Zd\n\n", a); // can use all gmp functions still
    pcs_encrypt(pk, hr, a, a);  // Encrypt a (= 50) and store back into a
    pcs_encrypt(pk, hr, b, b);  // Encrypt b (= 76) and store back into b
    gmp_printf("a = %Zd\nb = %Zd\n", a, b); // can use all gmp functions still

    pcs_ee_add(pk, c, a, b);    // Add encrypted a and b values together into c
    pcs_decrypt(vk, c, c);      // Decrypt c back into c using private key
    gmp_printf("%Zd\n", c);     // output: c = 126

    // Cleanup all data
    mpz_clears(a, b, c, NULL);
    pcs_free_public_key(pk);
    pcs_free_private_key(vk);
    hcs_free_random(hr);

    return 0;
}