
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/hmac.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "Kamara.h"

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

void ReadWords(const char* wFile, std::vector<std::vector<std::string>>& wkp, int maxWordNum)
{
    // read word from file and store words to Word Block
    std::ifstream ifs(wFile);
    if(!ifs)
    {
        std::cout << "<Setup> open '" << wFile << "' error." << std::endl;
        abort();
    }
    for(int i = 0; i < maxWordNum; i++)
    {
        // read a string line from the file
        std::vector<std::string> idlist;
        std::string idstr;
        std::getline(ifs, idstr);
        split(idlist, idstr, std::string(" "));
        wkp.push_back(idlist);
    }
}

const wbItem* BiSearch(const wbItem* start, const wbItem* end, std::string val)
{
    if(start > end)
    {
        return nullptr;
    }
    const wbItem* mid = start + (end-start)/2;
    if(val < mid->l)
    {
        // val in (start, mid)
        return(BiSearch(start, mid - 1, val));
    }
    else if(val > mid->l)
    {
        // val in (mid, end)
        return(BiSearch(mid + 1, end, val));
    }
    else
    {
        // val == mid->l
        return mid;
    }
}

void swap(wbItem& a1, wbItem& a2)
{
    std::string temp(a1.e);
    a1.e = a2.e;
    a2.e = a1.e;
    temp = a1.l;
    a1.l = a2.l;
    a2.l = temp;
}

int partition(std::vector<wbItem>& a, int left, int right)
{
    std::string pivotValue = a[right].l;
    int storeIndex = left;
    for (size_t i = left; i < right; i++)
    {
        if (a[i].l <= pivotValue)
        {
            swap(a[storeIndex], a[i]);
            storeIndex++;
        }
    }
    swap(a[right], a[storeIndex]);
    return storeIndex;
}

void quicksort(std::vector<wbItem>& a, int left, int right)
{
    if (right > left)
    {
        int pivotNewIndex = partition(a, left, right);
        quicksort(a, left, pivotNewIndex - 1);
        quicksort(a, pivotNewIndex + 1, right);
    }
    
}