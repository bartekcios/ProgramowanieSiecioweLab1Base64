// ProgramowanieSiecioweLab1Base64.cpp : Defines the entry point for the console application.
//

#include <string>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const char g_acTable[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
uint8_t getChar(const char a_cArg1)
{
    uint8_t uRetVal = 0u;

    for(unsigned int i=0;i<sizeof(g_acTable)/sizeof(g_acTable[0]);++i)
    {
        if(a_cArg1 == g_acTable[i])
        {
            uRetVal = i;
            break;
        }
    }

    return uRetVal;
}

string encodeBase64(string a_szIn)
{
    string szOut = "";

    const uint8_t uCondition = a_szIn.length()/3 + ((0 < a_szIn.length()%3)?1:0);

    for(unsigned int i=0;i<uCondition;++i)
    {
        const uint8_t uArraySize = 4u;
        char uCharacters[uArraySize] = {0u};
        int iArrayIndex = -1;

        //x' = Alfabet[x shr 2]
        uCharacters[0]  = (a_szIn.length() > i*3)   ?   (g_acTable[a_szIn[i*3]>>2]) :   0u;

        //y' = Alfabet[((x and 3) shl 4) or ((y and 240) shr 4)]
        iArrayIndex = -1;
        if(a_szIn.length() > i*3)
        {
            iArrayIndex = (a_szIn[i*3]&3u)<<4u;
        }
        if(a_szIn.length() > i*3+1)
        {
            iArrayIndex += (a_szIn[i*3+1]&240u)>>4u;
        }
        uCharacters[1] = (-1 < iArrayIndex) ? g_acTable[iArrayIndex]:0u;

        //z' = Alfabet[((y and 15) shl 2) or ((z and 192) shr 6)]
        iArrayIndex = -1;
        if(a_szIn.length() > i*3+1)
        {
            iArrayIndex = (a_szIn[i*3+1]&15u)<<2u;
        }
        if(a_szIn.length() > i*3+2)
        {
            iArrayIndex += (a_szIn[i*3+2]&192u)>>6u;
        }
        uCharacters[2] = (-1 < iArrayIndex) ? g_acTable[iArrayIndex]:0u;

        //w  = Alfabet[z and 63]
        uCharacters[3] = ((a_szIn.length() > i*3+2) ?   (g_acTable[(a_szIn[i*3+2]&63u)]) :   0u);

        for(int i=0;i<uArraySize;++i)
        {
            if(0u != uCharacters[i])
            {
                szOut += uCharacters[i];
            }
        }
    }

    if(2u == a_szIn.length()%3)
    {
        szOut += "=";
    }
    else if(1u == a_szIn.length()%3)
    {
        szOut += "==";
    }
    else
    {
        // nothing to do
    }

    cout << szOut << endl;

    return szOut;
}

string decodeBase64(string a_szEncoded)
{
    string szRetVal = "";
    for(unsigned int i=0;i<a_szEncoded.length()/4;++i)
    {
        //x' = (x shl 2) or (y shr 4)
        szRetVal += (getChar(a_szEncoded[i*4])<<2u) | ((getChar(a_szEncoded[i*4+1])>>4u));

        //y' = (y shl 4) or ((z shr 2) and 15)
        szRetVal += ((getChar(a_szEncoded[i*4+1]))<<4u) | (((getChar(a_szEncoded[i*4+2])>>2u)&15u));

        //z' = ((z shl 6) and 192) or (w and 63)
        szRetVal += (((getChar(a_szEncoded[i*4+2]))<<6u)&192u) | ((getChar(a_szEncoded[i*4+3])&63u));
    }

    cout << szRetVal << endl;

    return szRetVal;
}

int main(int argc, char *argv[])
{
    const string szText = "qwerty";
    const string szFileName = "test.txt";

    // open file to write encoded text
    std::ofstream ofFile;
    ofFile.open(szFileName);
    if(ofFile.is_open())
    {
        ofFile << encodeBase64(szText);
        ofFile.close();

        std::ifstream ifFile;
        ifFile.open(szFileName);

        if(ifFile.is_open())
        {
            string szLine = "";

            getline (ifFile, szLine, (char) ifFile.eof());
            decodeBase64(szLine);

            ifFile.close();
        }
    }

    if(1 < argc)
    {
        std::ifstream ifsFile;
        ifsFile.open (argv[1]);

        if(ifsFile.is_open())
        {
            string szLine = "";

            getline (ifsFile, szLine, (char) ifsFile.eof());

            decodeBase64(szLine);
        }
        else
        {
            cout << "Invalid file name" << endl;
        }
    }
    else
    {
        cout << "Missing file name" << endl;
    }

    //encodeBase64("Ala ma kota");

    return 0;
}
