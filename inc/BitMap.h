#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

//#include <memory.h>

//#include "gl_types.h"

class MyOutStream;

class BitMap
{
public:
    virtual int     MinNum( ) const   = 0;
    virtual int     MaxNum( ) const   = 0;
    virtual UINT32* WordP(int i)      = 0;
    virtual UINT32  Word(int i) const = 0;

    typedef UINT8 VeryBigBuffer[10000];
    BitMap( )          = default;
    virtual ~BitMap( ) = default;

    bool IsEmpty( );
    bool InRange(int number);
    bool Set(int number, bool value);
    bool SetRange(int minNumber, int maxNumber, bool value);

    void SetAll (bool value) { SetRange(MinNum( ), MaxNum( ), value); }

    bool    Get(int number);
    BitMap* Clean( );

    struct Iterator {
        Iterator(BitMap* ownerP) : _ownerP(ownerP)
        {
            int i;
            for ( i = 0; (i < _ownerP->NumWords( )) && (*(_ownerP->WordP(i)) == 0); i++ )
                ;
            for ( _curNumber = _ownerP->MinNum( ) + i * 32; (_curNumber <= _ownerP->MaxNum( )) && (!_ownerP->Get(_curNumber)); _curNumber++ )
                ;
        }

        bool IsValid ( ) { return _ownerP->InRange(_curNumber); }

        Iterator& operator++ (int)
        {
            for ( _curNumber++; _curNumber <= _ownerP->MaxNum( ); _curNumber++ ) {
                if ( _ownerP->Get(_curNumber) )
                    break;
            }
            return *this;
        }

        int operator* ( ) const { return _curNumber; }

        //		ENTRY * operator ->() const { return & _ownerP->_table[_index]; }

        BitMap* _ownerP;

        int _curNumber;
    };

    Iterator Begin ( ) { return Iterator(this); }

    bool IsEqual (BitMap* secondP)
    {
        if ( (MinNum( ) != secondP->MinNum( )) || (MaxNum( ) != secondP->MaxNum( )) )
            return false;
        return (memcmp(WordP(0), secondP->WordP(0), NumWords( ) * sizeof(UINT32)) == 0);
    }

    void PrintAsRange(MyOutStream* outP);

    UINT32 ToUint32(int from, int to);

    bool operator&& (const BitMap& otherR) const
    {
        int numWordsToCheck = (NumWords( ) <= otherR.NumWords( )) ? NumWords( ) : otherR.NumWords( );
        for ( int i = 0; i < numWordsToCheck; i++ ) {
            if ( Word(i) & otherR.Word(i) )
                return true;
        }
        return false;
    }

    int GetBytes (UINT8* dstP, int maxDstBytes)
    {
        int numBytes = (NumBytes( ) > maxDstBytes) ? maxDstBytes : NumBytes( );

        bzero(dstP, numBytes);
        for ( int i = MinNum( ); i <= MaxNum( ); i++ ) {
            UINT8 mask      = 0x1 << ((i - MinNum( )) % 8);
            int   byteIndex = (i - MinNum( )) / 8;

            if ( byteIndex >= numBytes )
                break;
            if ( Get(i) )
                dstP[byteIndex] |= mask;
        }
        return numBytes;
    }

    void SetBytes (UINT8* srcP, int srcBytes)
    {
        Clean( );
        for ( int i = 0; i < srcBytes * 8; i++ ) {
            UINT8 mask = 0x1 << (i % 8);
            if ( srcP[i / 8] & mask )
                Set(MinNum( ) + i, true);
        }
    }

    void Swap (BitMap* secondP)
    {
        if ( (MinNum( ) != secondP->MinNum( )) || (MaxNum( ) != secondP->MaxNum( )) )
            return;
        BitMap::VeryBigBuffer tmp;
        memcpy(tmp, WordP(0), NumWords( ) * sizeof(UINT32));
        memcpy(WordP(0), secondP->WordP(0), NumWords( ) * sizeof(UINT32));
        memcpy(secondP->WordP(0), tmp, NumWords( ) * sizeof(UINT32));
    }

    int NumBits ( ) const { return MaxNum( ) - MinNum( ) + 1; }

    int NumWords ( ) const { return NumBits( ) / 32 + ((NumBits( ) % 32) ? 1 : 0); }

    int NumBytes ( ) const { return NumBits( ) / 8 + ((NumBits( ) % 8) ? 1 : 0); }

    void CopyFrom (BitMap* srcP)
    {
        if ( (MinNum( ) != srcP->MinNum( )) || (MaxNum( ) != srcP->MaxNum( )) )
            return;
        memcpy(WordP(0), srcP->WordP(0), NumWords( ) * sizeof(UINT32));
    }

protected:
    UINT32  GetMask(int number);
    UINT32* Bit2WordP(int number);

private:
    BitMap& operator= (const BitMap& secondR);
    BitMap(const BitMap& secondR);
};

template<int MIN_NUM, int MAX_NUM>
class BitMapT : public BitMap
{
public:
    int MinNum ( ) const override { return MIN_NUM; }

    int MaxNum ( ) const override { return MAX_NUM; }

    UINT32* WordP (int i) override { return &_array[i]; }

    UINT32 Word (int i) const override { return _array[i]; }

    typedef BitMapT<MIN_NUM, MAX_NUM> MyType;

    BitMapT( ) { Clean( ); }

    BitMapT(const MyType& secondR) { memcpy(_array, secondR._array, sizeof(_array)); }

    MyType& operator= (const MyType& secondR)
    {
        memcpy(_array, secondR._array, sizeof(_array));
        return *this;
    }

    MyType& operator|= (const MyType& secondR)
    {
        int numWords = NumWords( );
        for ( int i = 0; i < numWords; i++ )
            *WordP(i) |= *secondR.WordP(i);
        return *this;
    }

    MyType operator| (const MyType& secondR)
    {
        MyType result = secondR;
        result |= secondR;
        return result;
    }

    MyType& operator&= (const MyType& secondR)
    {
        int numWords = NumWords( );
        for ( int i = 0; i < numWords; i++ )
            *WordP(i) &= *secondR.WordP(i);
        return *this;
    }

    MyType operator& (const MyType& secondR)
    {
        MyType result = secondR;
        result &= secondR;
        return result;
    }

    MyType& operator^= (const MyType& secondR)  // XOR
    {
        int numWords = NumWords( );
        for ( int i = 0; i < numWords; i++ )
            *WordP(i) ^= *secondR.WordP(i);
        return *this;
    }

    MyType operator^ (const MyType& secondR)
    {
        MyType result = secondR;
        result ^= secondR;
        return result;
    }

private:
    enum { NUM_BITS = MAX_NUM - MIN_NUM + 1 };

    UINT32 _array[NUM_BITS / 32 + ((NUM_BITS % 32) ? 1 : 0)];
};

class BitMapDynamic : public BitMap
{
public:
    int MinNum ( ) const override { return _minNum; }

    int MaxNum ( ) const override { return _maxNum; }

    UINT32* WordP (int i) override { return &_arrayP[i]; }

    UINT32 Word (int i) const override { return _arrayP[i]; }

    void Init (int minNum, int maxNum)
    {
        _minNum = minNum;
        _maxNum = maxNum;

        delete[] _arrayP;

        _arrayP = new UINT32[NumWords( )];
        Clean( );
    }

    ~BitMapDynamic( ) override { delete[] _arrayP; }

private:
    UINT16  _minNum {0};
    UINT16  _maxNum {0};
    UINT32* _arrayP {nullptr};
};

#endif  // BITMAP_H_INCLUDED
