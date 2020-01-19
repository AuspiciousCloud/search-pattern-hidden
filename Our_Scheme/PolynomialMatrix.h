#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <vector>
#include <string>

#include "BCP.h"

#define POLY_SIZE 2048   // max size of ploynomial

/************ polynomial class *************\
 * Implementation of polynomial operations *
\*******************************************/ 
class polynomial
{
private:
    ZZZ* coefficients;
    unsigned int size;
    ZZZ modulo;
public:
    polynomial();
    polynomial(ZZZ* coef, unsigned int csize, ZZZ mod);
    ~polynomial() { delete[] coefficients; }
    polynomial operator*(const polynomial& rhs);
    polynomial& operator=(const polynomial& rhs);
    friend std::ostream& operator<<(std::ostream& os, const polynomial& poly);
    ZZZ coef(unsigned int power) const { return coefficients[power]; }
};

polynomial One(ZZZ n);

void split(std::vector<std::string>& out, const std::string& instr, const std::string& seperator);

#endif