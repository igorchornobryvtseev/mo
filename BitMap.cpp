#include "igorc_common.h"

#include "BitMap.h"

#include "MyString.h"

UINT32 BitMap::ToUint32(int from, int to)
{
    UINT32 value = 0;
    UINT32 mask  = 0x1;

    if ( from < MinNum( ) )
        from = MinNum( );
    if ( to > MaxNum( ) )
        to = MaxNum( );

    for ( int i = from; i <= to; i++, mask <<= 1 ) {
        if ( Get(i) )
            value |= mask;
    }
    return value;
}

static void PrintSubrange (MyOutStream* outP, bool* isFirstP, int startNum, int endNum)
{
    if ( !*isFirstP )
        *outP << ",";
    *isFirstP = false;

    *outP << startNum;
    if ( startNum != endNum )
        *outP << "-" << endNum;
}

void BitMap::PrintAsRange(MyOutStream* outP)
{
    bool isFirstSubrange = true;
    int  startNum        = -1;
    int  endNum          = -1;
    for ( int i = MinNum( ); i <= MaxNum( ); i++ ) {
        if ( Get(i) ) {
            if ( startNum < 0 )
                startNum = endNum = i;  // start new sub-range
            else
                endNum++;  // continue current sub-range
        } else {
            if ( startNum >= 0 )
                PrintSubrange(outP, &isFirstSubrange, startNum, endNum);
            startNum = -1;
        }
    }
    if ( startNum >= 0 )
        PrintSubrange(outP, &isFirstSubrange, startNum, endNum);
}

bool BitMap::InRange(int number)
{
    if ( number < MinNum( ) )
        return false;
    if ( number > MaxNum( ) )
        return false;
    return true;
}

bool BitMap::IsEmpty( )
{
    for ( int i = 0; i < NumWords( ); i++ ) {
        if ( *WordP(i) != 0 )
            return false;
    }
    return true;
}

BitMap* BitMap::Clean( )
{
    for ( int i = 0; i < NumWords( ); i++ )
        *WordP(i) = 0;
    return this;
}

UINT32* BitMap::Bit2WordP(int number)
{
    int index = number - MinNum( );
    return WordP(index / 32);
}

UINT32 BitMap::GetMask(int number)
{
    int    index   = number - MinNum( );
    UINT32 bitMask = 0x1;
    if ( index % 32 != 0 )
        bitMask <<= index % 32;
    return bitMask;
}

bool BitMap::Get(int number)
{
    if ( !InRange(number) )
        return false;
    return (*Bit2WordP(number) & GetMask(number));
}

bool BitMap::Set(int number, bool value)
{
    if ( !InRange(number) )
        return false;
    if ( value )
        *Bit2WordP(number) |= GetMask(number);
    else
        *Bit2WordP(number) &= ~GetMask(number);
    return true;
}

bool BitMap::SetRange(int minNumber, int maxNumber, bool value)
{
    for ( int i = minNumber; i <= maxNumber; i++ ) {
        if ( !InRange(i) )
            return false;
        if ( value )
            *Bit2WordP(i) |= GetMask(i);
        else
            *Bit2WordP(i) &= ~GetMask(i);
    }
    return true;
}
