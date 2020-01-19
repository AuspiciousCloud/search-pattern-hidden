#include "PolynomialMatrix.h"
#include <string.h>
#include <iostream>


polynomial::polynomial() : size(0), modulo(1)
{
    coefficients = new ZZZ[POLY_SIZE];
    for(int i = 0; i < POLY_SIZE; i++)
    {
        coefficients[i] = "0";
    }
}

polynomial::polynomial(ZZZ* coef, unsigned int csize, ZZZ mod) : size(csize), modulo(mod)
{
    coefficients = new ZZZ[POLY_SIZE];
    for(int i = 0; i < POLY_SIZE; i++)
    {
        coefficients[i] = "0";
    }
    for(int i = 0; i < csize; i++)
    {
        mpz_mod(coefficients[i].get_mpz_t(), coef[i].get_mpz_t(), mod.get_mpz_t());
    }
}

polynomial polynomial::operator*(const polynomial& rhs)
{
    if(this->modulo != rhs.modulo)
    {
        std::cout << "polynomial multiple error" << std::endl;
        abort();
    }
    ZZZ* result_coef = new ZZZ[POLY_SIZE];
    for(int i = 0; i < POLY_SIZE; i++)
    {
        result_coef[i] = "0";
    }
    int rsize = this->size + rhs.size - 1;
    for(int i = 0; i < rsize; i++)
    {
        for(int j = 0; j <= i; j++)
        {
            if(j < this->size && (i - j) < rhs.size)
            {
                ZZZ temp;
                temp = this->coef(j) * rhs.coef(i - j);
                mpz_mod(temp.get_mpz_t(), temp.get_mpz_t(), (this->modulo).get_mpz_t());
                temp = result_coef[i] + temp;
                mpz_mod(result_coef[i].get_mpz_t(), temp.get_mpz_t(), this->modulo.get_mpz_t());
            }
        }
    }
    polynomial result(result_coef, rsize, this->modulo);
    delete[] result_coef;
    return result;
}

polynomial& polynomial::operator=(const polynomial& other)
{
    if(this != &other)
    {
        for(int i = 0; i < POLY_SIZE; i++)
        {
            this->coefficients[i] = other.coefficients[i];
        }
        this->size = other.size;
        this->modulo = other.modulo;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const polynomial& poly)
{
    for(int i = 0; i < poly.size; i++)
    {
        if(poly.coefficients[poly.size - i - 1] < 0)
        {
            os << " - " << 0 - poly.coefficients[poly.size - i - 1] << "x^" << poly.size - i - 1;
        }
        else
        {
            os << (i ? " + " : "") << poly.coefficients[poly.size - i - 1] << "x^" << poly.size - i - 1;
        }
            
    }
    return os;
}

polynomial One(ZZZ n)
{
    ZZZ one[1] = {1};
    return polynomial(one, 1, n);
}

void split(std::vector<std::string>& out, const std::string& instr, const std::string& seperator)
{
    out.clear();
    std::size_t beginpos = 0;
    std::size_t found;
    found = instr.find(seperator, beginpos);
    do
    {
        out.push_back(instr.substr(beginpos, found - beginpos));
        beginpos = found + seperator.length();
        found = instr.find(seperator, beginpos);
    } while (found != std::string::npos);
    out.push_back(instr.substr(beginpos, instr.length() - beginpos));
}
