// MyString.cpp: implementation of the MyString class.
//
//////////////////////////////////////////////////////////////////////
#include "igorc_common.h"

#include "MyString.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MyOutStream::Print(const char* formatP, ...)
{
    va_list vaList;
    va_start(vaList, formatP);
    DoPrint(formatP, &vaList);
    va_end(vaList);
}

void MyOutStream::Print(UINT32 len, const char* strP)
{
    for ( UINT32 i = 0; i < len; i++ )
        Print("%c", strP[i]);
}

MyString& MyString::operator= (const char* srcP)
{
    DoClean( );
    Print(srcP);
    return *this;
}

bool MyString::AsInt(int* intP)
{
    int numOf = sscanf(DoStartP( ), "%d", intP);
    return (numOf == 1);
}

int MyString::AsInt( )
{
    int  value;
    bool isOk = AsInt(&value);
    return isOk ? value : -1;
}

int MyString::FindSubstr(int startPos, const char* pattern)
{
    char* startP = strstr(AsStr( ) + startPos, pattern);
    if ( startP == nullptr )
        return -1;
    int pos = startP - AsStr( );
    return pos;
}

bool MyString::SubstituteSubstr(const char* pattern, const char* newSubstr)
{
    int pos = FindSubstr(0, pattern);
    if ( pos < 0 )
        return false;
    RemoveSubstr(pos, strlen(pattern));
    InsertSubstr(pos, newSubstr);
    return true;
}

void MyString::RemoveSubstr(int startPos, int len)
{
    int totalLen = strlen(AsStr( ));
    int tailPos  = startPos + len;

    if ( tailPos > totalLen )
        return;
    memmove(AsStr( ) + startPos, AsStr( ) + tailPos, totalLen - tailPos + 1);
}

int MyString::FindWord(int startPos)
{
    while ( (Get(startPos) == ' ') || (Get(startPos) == '\t') )
        startPos++;
    return startPos;
}

int MyString::SkipWord(int startPos)
{
    while ( (Get(startPos) != ' ') && (Get(startPos) != '\t') && (Get(startPos) != 0) )
        startPos++;
    return startPos;
}

void MyString::RemoveWord(int startPos)
{
    int len = 0;
    while ( (Get(startPos + len) != 0) && (Get(startPos + len) != ' ') )
        len++;
    RemoveSubstr(startPos, len);
}

void MyString::ToUpper(int index)
{
    AsStr( )[index] = toupper(AsStr( )[index]);
}

void MyString::StripQuotes( )
{
    if ( Get(0) == '\"' )
        RemoveSubstr(0, 1);

    int len = GetLen( );
    if ( (len > 0) && (Get(len - 1) == '\"') )
        AsStr( )[len - 1] = 0;
}

static bool CheckChar (char orig, char* patternP)
{
    UINT32 len = strlen(patternP);
    for ( UINT32 i = 0; i < len; i++ ) {
        if ( orig == patternP[i] )
            return true;
    }
    return false;
}

MyString& MyString::StripStart(char* patternP)
{
    while ( GetLen( ) && CheckChar(Get(0), patternP) )
        RemoveSubstr(0, 1);
    return *this;
}

MyString& MyString::StripEnd(char* patternP)
{
    while ( GetLen( ) && CheckChar(GetLast( ), patternP) )
        AsStr( )[GetLen( ) - 1] = 0;
    return *this;
}

bool MyString::LoadFile(char* pathP)
{
    FILE* fileP = fopen(pathP, "rb");
    if ( fileP == nullptr )
        return false;

    fseek(fileP, 0, SEEK_END);
    unsigned long fsize = ftell(fileP);
    fseek(fileP, 0, SEEK_SET);

    if ( fsize >= (GetMaxLen( ) - 1) )
        fsize = GetMaxLen( ) - 1;

    fread(AsStr( ), fsize, 1, fileP);
    fclose(fileP);
    AsStr( )[fsize] = 0;

    return true;
}

void MyString::InsertSubstr(int startPos, const char* srcP)
{
    UINT32 insertLen = strlen(srcP);
    if ( (startPos + insertLen) > GetMaxLen( ) )
        insertLen = GetMaxLen( ) - startPos;

    UINT32 tailLen = GetLen( ) - startPos;
    if ( (startPos + insertLen + tailLen) > GetMaxLen( ) )
        tailLen = GetMaxLen( ) - startPos - insertLen;

    memmove(AsStr( ) + startPos + insertLen, AsStr( ) + startPos, tailLen);
    *(AsStr( ) + startPos + insertLen + tailLen) = 0;

    memcpy(AsStr( ) + startPos, srcP, insertLen);
}

void MyString::DoPrint(const char* formatP, va_list* listP)
{
    int len         = GetLen( );
    int remainedLen = DoMaxLen( ) - len + 1;
    if ( remainedLen <= 0 )
        return;
    vsnprintf(DoStartP( ) + len, remainedLen, formatP, *listP);
}

bool MyString::StartsWith(const char* secondP)
{
    UINT32 secondLen = strlen(secondP);
    if ( secondLen > GetLen( ) )
        return false;

    bool isSubstring = (strncasecmp(AsStr( ), secondP, secondLen) == 0);
    return isSubstring;
}

bool MyString::StartsWith(const MyString& secondR)
{
    return StartsWith(((MyString&)secondR).AsStr( ));
}

int MyString::MaxCommonPrefix(const char* secondP, MyString* CommonSubStr)
{
    UINT32 minLen = 0;
    if ( (strlen(secondP) == 0) || (GetLen( ) == 0) )
        return -1;

    if ( strlen(secondP) > GetLen( ) )
        minLen = GetLen( );
    else
        minLen = strlen(secondP);

    UINT32 i = 0;

    for ( ; i < minLen; i++ ) {
        if ( Get(i) != secondP[i] )
            break;
    }

    if ( i > 0 )
        strncpy(CommonSubStr->AsStr( ), AsStr( ), i * sizeof(char));
    *(CommonSubStr->AsStr( ) + i * sizeof(char)) = 0;

    return i;
}

int MyString::MaxCommonPrefix(const MyString& secondR, MyString* CommonSubStr)
{
    return MaxCommonPrefix(((MyString&)secondR).AsStr( ), CommonSubStr);
}

int MyString::Compare(const MyString& secondR) const
{
    return strcmp(AsStr( ), ((MyString&)secondR).AsStr( ));
}

bool MyString::operator== (const MyString& secondR) const
{
    return (strcasecmp(DoStartP( ), ((MyString&)secondR).DoStartP( )) == 0);
}

bool MyString::operator== (const char* secondP) const
{
    return (strcasecmp(DoStartP( ), secondP) == 0);
}

bool MyString::operator!= (const MyString& secondR) const
{
    return (strcasecmp(DoStartP( ), ((MyString&)secondR).DoStartP( )) != 0);
}

bool MyString::operator!= (const char* secondP) const
{
    return (strcasecmp(DoStartP( ), secondP) != 0);
}

MyOutStream& operator<< (MyOutStream& dstR, const MyString& str)
{
    dstR.Print(str.GetLen( ), str.AsStr( ));
    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, const char* str)
{
    dstR.Print(str);
    return dstR;
}

// MyOutStream& operator<< (MyOutStream& dstR, std::string_view str)
// {
//     dstR.Print(str.length( ), str.data( ));
//     return dstR;
// }

MyOutStream& operator<< (MyOutStream& dstR, const std::string& str)
{
    dstR.Print(str.length( ), str.c_str( ));
    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, UINT64 src)
{
    dstR.Print("%lld", src);
    return dstR;
}

// MyOutStream& operator<< (MyOutStream& dstR, const Mac& mac)
// {
//     dstR.Print("%s", mac.AsString( ).c_str( ));
//     return dstR;
// }

// MyOutStream& operator<< (MyOutStream& dstR, const std::filesystem::path& path)
// {
//     dstR << path.string( );
//     return dstR;
// }

bool operator== (const std::string& str1, const MyString& str2)
{
    return str1 == str2.AsStr( );
}

int MySystem::Exe( )
{
    int result = system(AsStr( ));
    return result;
}
