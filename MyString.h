#pragma once

//#include <fmt/format.h>

#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <filesystem>
#include <string>

// #include "gl_types.h"
// #include "MacAddress.h"

//namespace fs = std::filesystem;
class Mo;

class MyOutStream
{
// #if __cplusplus > 202000L
//     #define MY_STRING_CONSTEXPR constexpr
// #else
    #define MY_STRING_CONSTEXPR inline
// #endif
public:
    void Print(const char* formatP, ...);
    void Print(UINT32 len, const char* strP);

    virtual ~MyOutStream( ) = default;

    virtual void DoPrint(const char* formatP, va_list* listP) = 0;

    void Clean ( ) { DoClean( ); }

    void SetMoFocus (Mo* moP) { _moFocusP = moP; }

    Mo* GetMoFocus ( ) { return _moFocusP; }

    void SetNameValSeparator (char* separatorP) { _nameValSeparatorP = separatorP; }

    char* GetNameValSeparator ( ) { return _nameValSeparatorP; }

    MY_STRING_CONSTEXPR MyOutStream& assign(std::string_view sv);
    MY_STRING_CONSTEXPR MyOutStream& append(std::string_view sv);
    // template<typename... T>
    // MY_STRING_CONSTEXPR MyOutStream& formatAppend(fmt::format_string<T...> fmt, T&&... args);

protected:
    virtual void DoClean( ) = 0;

    Mo*   _moFocusP {nullptr};
    char* _nameValSeparatorP /*{": "}*/;
};

class MyString : public MyOutStream
{
public:
    MyString& operator= (const char* srcP);

    UINT32 GetLen ( ) const { return strlen(AsStr( )); }

    bool IsEmpty ( ) { return AsStr( )[0] == 0; }

    UINT32 GetMaxLen ( ) const { return const_cast<MyString*>(this)->DoMaxLen( ); }

    char* AsStr ( ) { return DoStartP( ); }

    const char* AsStr ( ) const { return const_cast<MyString*>(this)->DoStartP( ); }

    operator std::string_view ( ) const noexcept { return view( ); }

    std::string_view view ( ) const { return {AsStr( ), GetLen( )}; }

    bool AsInt(int* intP);
    int  AsInt( );

    char Get (UINT32 pos) { return DoStartP( )[pos]; }

    char GetLast ( )
    {
        UINT32 len = GetLen( );
        return (len > 0) ? Get(len - 1) : 0;
    }

    bool StartsWith(const char* secondP);
    bool StartsWith(const MyString& secondR);

    int FindWord(int startPos);
    int SkipWord(int startPos);

    int MaxCommonPrefix(const char* secondP, MyString* CommonSubStr);
    int MaxCommonPrefix(const MyString& secondR, MyString* CommonSubStr);

    int  FindSubstr(int startPos, char* pattern);
    void RemoveSubstr(int startPos, int len);
    void RemoveWord(int startPos);
    void InsertSubstr(int startPos, char* srcP);
    void StripQuotes( );

    MyString& StripStart(char* patternP);
    MyString& StripEnd(char* patternP);

    bool SubstituteSubstr(char* pattern, char* newSubstr);

    void ToUpper(int index);

    bool LoadFile(char* pathP);

    int Compare(const MyString& secondR) const;

    bool operator== (const MyString& secondR) const;
    bool operator== (const char* secondP) const;
    bool operator!= (const MyString& secondR) const;
    bool operator!= (const char* secondP) const;

    void DoPrint(const char* formatP, va_list* listP) override;

protected:
    virtual char* DoStartP( ) = 0;

    const char* DoStartP ( ) const { return const_cast<MyString*>(this)->DoStartP( ); }

    virtual size_t DoMaxLen( ) = 0;

    void DoClean ( ) override
    {
        if ( AsStr( ) != nullptr )
            AsStr( )[0] = 0;
    }
};

template<int MAX_LEN>
class MyStringT : public MyString
{
public:
    MyStringT( ) { DoClean( ); }

    explicit MyStringT(const char* formatP, ...)
    {
        DoClean( );

        va_list vaList;
        va_start(vaList, formatP);
        DoPrint(formatP, &vaList);
        va_end(vaList);
    }

    MyStringT& Set (std::string_view text)
    {
        const size_t len  = std::min(text.length( ), DoMaxLen( ));
        auto         iter = std::copy_n(text.begin( ), len, std::begin(_buffer));
        *iter             = '\0';
        return *this;
    }

protected:
    char* DoStartP ( ) override { return _buffer; }

    size_t DoMaxLen ( ) override { return MAX_LEN; }

private:
    char _buffer[MAX_LEN + 1];
};

class PreallocatedString : public MyString
{
public:
    PreallocatedString(char* cstr, size_t size) : bufferSize(size), buffer(cstr) { }

#if 0
    void newLocation (char* cstr, size_t size = 0)
    {
        buffer     = cstr;
        bufferSize = size ? size : ::strlen(cstr) + 1;
    }
#endif

    char* DoStartP ( ) override { return buffer; }

    size_t DoMaxLen ( ) override { return bufferSize - 1; }

private:
    size_t bufferSize;
    char*  buffer;
};

MyOutStream& operator<< (MyOutStream& dstR, const char* str);
MyOutStream& operator<< (MyOutStream& dstR, const MyString& str);
// MyOutStream& operator<< (MyOutStream& dstR, std::string_view str);
MyOutStream& operator<< (MyOutStream& dstR, const std::string& str);
//MyOutStream& operator<< (MyOutStream& dstR, const fs::path& path);
MyOutStream& operator<< (MyOutStream& dstR, UINT64 src);
//MyOutStream& operator<< (MyOutStream& dstR, const Mac& mac);
bool         operator== (const std::string& str1, const MyString& str2);

#if __cplusplus < 202002L
inline bool operator!= (const std::string& str1, const MyString& str2)
{
    return !(str1 == str2);
}

inline bool operator== (const MyString& str1, const std::string& str2)
{
    return str2 == str1;
}

inline bool operator!= (const MyString& str1, const std::string& str2)
{
    return !(str2 == str1);
}
#endif

struct MySystem : public MyStringT<2048> {
    MySystem( ) : MyStringT<2048>( ) { }

    explicit MySystem(const char* formatP, ...)
    {
        DoClean( );

        va_list vaList;
        va_start(vaList, formatP);
        DoPrint(formatP, &vaList);
        va_end(vaList);
    }

    int Exe( );
};

MY_STRING_CONSTEXPR MyOutStream& MyOutStream::assign(std::string_view sv)
{
    DoClean( );
    return append(sv);
}

MY_STRING_CONSTEXPR MyOutStream& MyOutStream::append(std::string_view sv)
{
    Print(sv.length( ), sv.data( ));
    return *this;
}

// template<typename... T>
// MY_STRING_CONSTEXPR MyOutStream& MyOutStream::formatAppend(fmt::format_string<T...> fmt, T&&... args)
// {
//     return append(fmt::format(fmt, std::forward<T>(args)...));
// }
