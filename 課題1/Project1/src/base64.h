#if !defined BASE64_H_INC_
#define BASE64_H_INC_

#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

class base64
{
    int iLen;
    char* lpszOutputString;
    static const char* szB64;

    static int FindIndexInB64(char c);

public:
    base64();
    ~base64();
    const char* Encode(const char* szStr, int iLens = -1);
    const char* Decode(const char* szStr);
    int GetLenght() const { return iLen; }
    const char* Get() const { return lpszOutputString; }
    static void Encode(istream& istr, ostream& ostr, int iRet = 76);
    static void Decode(istream& istr, ostream& ostr);

    static void Encode(const char* lpszImputFileName, const char* lpszOutputFileName, int iRet = 76)
    {
        ifstream f(lpszImputFileName, ios::in | ios::binary);
        ofstream of(lpszOutputFileName);
        Encode(f, of);
    }
    static void Decode(const char* lpszImputFileName, const char* lpszOutputFileName)
    {
        ifstream f(lpszImputFileName);
        ofstream of(lpszOutputFileName, ios::out | ios::trunc | ios::binary);
        Decode(f, of);
    }
};

#endif // #if !defined BASE64_H_INC_