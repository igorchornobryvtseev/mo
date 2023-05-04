//***************************************************************
// File: Dictionary.h
// Class/Module: Dictionary
//
// Purpose:
// Description:
//
// Modification history:
//           12/2/01   [LEONID]  Created
//
// Class Invariant:
// Copy Semantics:
//***************************************************************
#ifndef IPRAD_DICTIONARY_H
#define IPRAD_DICTIONARY_H

// #include "gl_types.h"
#include "MyString.h"
// #include "PlaceBuffer.h"

// ***************************************************************

class Dictionary
{
public:
    struct Entry {
        Entry( ) = default;

        Entry(uint32_t value, const char* nameP) : _value(value), _nameP(nameP) { }

        bool IsValid ( ) const { return _value != (uint32_t)-1; }

        uint32_t    _value {(uint32_t)-1};
        const char* _nameP {"unknonw"};
    };

    bool HasValue (UINT32 value)
    {
        UINT32 context;
        for ( ___FirstEntry(&context); ___GetEntry(context).IsValid( ); ___NextEntry(&context) ) {
            if ( ___GetEntry(context)._value == value )
                return true;
        }
        return false;
    }

    virtual ~Dictionary( ) = default;

    virtual Entry ___GetEntry(UINT32 context) = 0;

    virtual void ___FirstEntry (UINT32* contextP) { *contextP = 0; }

    virtual void ___NextEntry (UINT32* contextP) { (*contextP)++; }
};

template<int MAX>
class DictionaryT : public Dictionary
{
public:
    Entry ___GetEntry (UINT32 context) override { return (context < (UINT32)_numOf) ? _table[context] : Entry( ); }

    bool IsEmpty ( ) { return (_numOf == 0); }

    DictionaryT<MAX>& Add (Entry entry)
    {
        if ( _numOf < MAX )
            _table[_numOf++] = entry;
        return *this;
    }

    template<typename ValueType>
    DictionaryT<MAX>& Add (ValueType val, const char* strP)
    {
        Add(Dictionary::Entry(static_cast<uint32_t>(val), strP));
        return *this;
    }

    DictionaryT<MAX>& Add (Dictionary* srcP)
    {
        UINT32 context;
        for ( srcP->___FirstEntry(&context); srcP->___GetEntry(context).IsValid( ); srcP->___NextEntry(&context) )
            Add(srcP->___GetEntry(context));
        return *this;
    }

    void Del (UINT32 val)
    {
        int i;
        for ( i = 0; (i < _numOf) && (_table[i]._value != val); i++ )
            ;
        if ( i >= _numOf )
            return;
        for ( ; i < (_numOf - 1); i++ )
            _table[i] = _table[i + 1];
        _table[_numOf - 1] = Entry( );
        _numOf--;
    }

    int   _numOf {0};
    Entry _table[MAX + 1];

    void Clear ( )
    {
        for ( int i = 0; i < _numOf; i++ )
            _table[i] = Entry( );
        _numOf = 0;
    }
};

#define DICTIONARY_1(name, v1, s1) \
    struct name : DictionaryT<1> { \
        name( )                    \
        {                          \
            Add(v1, s1);           \
        }                          \
    }

#define DICTIONARY_2(name, v1, s1, v2, s2) \
    struct name : DictionaryT<2> {         \
        name( )                            \
        {                                  \
            Add(v1, s1);                   \
            Add(v2, s2);                   \
        }                                  \
    }

#define DICTIONARY_3(name, v1, s1, v2, s2, v3, s3) \
    struct name : DictionaryT<3> {                 \
        name( )                                    \
        {                                          \
            Add(v1, s1);                           \
            Add(v2, s2);                           \
            Add(v3, s3);                           \
        }                                          \
    }

#define DICTIONARY_4(name, v1, s1, v2, s2, v3, s3, v4, s4) \
    struct name : DictionaryT<4> {                         \
        name( )                                            \
        {                                                  \
            Add(v1, s1);                                   \
            Add(v2, s2);                                   \
            Add(v3, s3);                                   \
            Add(v4, s4);                                   \
        }                                                  \
    }

#define DICTIONARY_5(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5) \
    struct name : DictionaryT<5> {                                 \
        name( )                                                    \
        {                                                          \
            Add(v1, s1);                                           \
            Add(v2, s2);                                           \
            Add(v3, s3);                                           \
            Add(v4, s4);                                           \
            Add(v5, s5);                                           \
        }                                                          \
    }

#define DICTIONARY_6(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5, v6, s6) \
    struct name : DictionaryT<6> {                                         \
        name( )                                                            \
        {                                                                  \
            Add(v1, s1);                                                   \
            Add(v2, s2);                                                   \
            Add(v3, s3);                                                   \
            Add(v4, s4);                                                   \
            Add(v5, s5);                                                   \
            Add(v6, s6);                                                   \
        }                                                                  \
    }

#define DICTIONARY_7(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5, v6, s6, v7, s7) \
    struct name : DictionaryT<7> {                                                 \
        name( )                                                                    \
        {                                                                          \
            Add(v1, s1);                                                           \
            Add(v2, s2);                                                           \
            Add(v3, s3);                                                           \
            Add(v4, s4);                                                           \
            Add(v5, s5);                                                           \
            Add(v6, s6);                                                           \
            Add(v7, s7);                                                           \
        }                                                                          \
    }

#define DICTIONARY_8(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5, v6, s6, v7, s7, v8, s8) \
    struct name : DictionaryT<8> {                                                         \
        name( )                                                                            \
        {                                                                                  \
            Add(v1, s1);                                                                   \
            Add(v2, s2);                                                                   \
            Add(v3, s3);                                                                   \
            Add(v4, s4);                                                                   \
            Add(v5, s5);                                                                   \
            Add(v6, s6);                                                                   \
            Add(v7, s7);                                                                   \
            Add(v8, s8);                                                                   \
        }                                                                                  \
    }

#define DICTIONARY_9(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5, v6, s6, v7, s7, v8, s8, v9, s9) \
    struct name : DictionaryT<9> {                                                                 \
        name( )                                                                                    \
        {                                                                                          \
            Add(v1, s1);                                                                           \
            Add(v2, s2);                                                                           \
            Add(v3, s3);                                                                           \
            Add(v4, s4);                                                                           \
            Add(v5, s5);                                                                           \
            Add(v6, s6);                                                                           \
            Add(v7, s7);                                                                           \
            Add(v8, s8);                                                                           \
            Add(v9, s9);                                                                           \
        }                                                                                          \
    }

#define DICTIONARY_10(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5, v6, s6, v7, s7, v8, s8, v9, s9, v10, s10) \
    struct name : DictionaryT<10> {                                                                           \
        name( )                                                                                               \
        {                                                                                                     \
            Add(v1, s1);                                                                                      \
            Add(v2, s2);                                                                                      \
            Add(v3, s3);                                                                                      \
            Add(v4, s4);                                                                                      \
            Add(v5, s5);                                                                                      \
            Add(v6, s6);                                                                                      \
            Add(v7, s7);                                                                                      \
            Add(v8, s8);                                                                                      \
            Add(v9, s9);                                                                                      \
            Add(v10, s10);                                                                                    \
        }                                                                                                     \
    }

#define DICTIONARY_11(name, v1, s1, v2, s2, v3, s3, v4, s4, v5, s5, v6, s6, v7, s7, v8, s8, v9, s9, v10, s10, v11, s11) \
    struct name : DictionaryT<11> {                                                                                     \
        name( )                                                                                                         \
        {                                                                                                               \
            Add(v1, s1);                                                                                                \
            Add(v2, s2);                                                                                                \
            Add(v3, s3);                                                                                                \
            Add(v4, s4);                                                                                                \
            Add(v5, s5);                                                                                                \
            Add(v6, s6);                                                                                                \
            Add(v7, s7);                                                                                                \
            Add(v8, s8);                                                                                                \
            Add(v9, s9);                                                                                                \
            Add(v10, s10);                                                                                              \
            Add(v11, s11);                                                                                              \
        }                                                                                                               \
    }

// ***************************************************************

using DictAutoCompletion = DictionaryT<100>;
#endif  // IPRAD_DICTIONARY_H
