#ifndef KAMARA_H
#define KAMARA_H

#include <string>
#include <vector>

typedef struct
{
    std::string l;
    std::string e;
}
wbItem;

/* ******************** *\
 * processing functions *
\* ******************** */
void split(std::vector<std::string>& out, const std::string& instr, const std::string& seperator);
void ReadWords(const char* wFile, std::vector<std::vector<std::string>>& wkp, int maxWordNum);
const wbItem* BiSearch(const wbItem* start, const wbItem* end, std::string val);
void quicksort(std::vector<wbItem>& a, int left, int right);
/* ******************** *\
 * processing functions *
\* ******************** */

/* ***************** *\
 * Kamara Algorithms *
\* ***************** */
void Setup(const std::vector<std::vector<std::string>>& wkp, std::vector<wbItem>& WordBlock, 
            std::vector<wbItem>& DummyBlock, 
            unsigned char* key1, unsigned char* key2, unsigned char* ivstr);

std::string TGen(const unsigned char* k2, const char* w);

void Rebuild(std::vector<wbItem>& WordBlock, std::vector<wbItem>& Shelter, 
                std::vector<wbItem>& DummyBlock, 
                unsigned char* k1, unsigned char* k2, unsigned char* ivstr);

void Search(const std::string& token, std::vector<wbItem>& R, 
                std::vector<wbItem>& WordBlock, std::vector<wbItem>& Shelter, 
                std::vector<wbItem>& DummyBlock, 
                unsigned char* k1, unsigned char* k2, unsigned char* ivstr);

void Decrypt(std::vector<std::string>& ids, const std::vector<wbItem>& R, 
    unsigned char* k1, unsigned char* k2, unsigned char* ivstr);
/* ***************** *\
 * Kamara Algorithms *
\* ***************** */

#endif