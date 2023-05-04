//***************************************************************
// File:	 PlaceBuffer.h
// Copyright: Siklu Communications , 2010
// SVN: $Revision$
// SVN: $Author$
// SVN: $Date$
// ***************************************************************

#ifndef __sk_PlaceBuffer_h__
#define __sk_PlaceBuffer_h__

#include <cstdlib>

/////////////////////////////////////////////////////

class PlaceBuffer
{
public:
    PlaceBuffer(const char* textP) : _textP((char*)textP) { }

    virtual ~PlaceBuffer( ) = default;

    virtual void* Alloc(size_t objSize) = 0;

    char* _textP;
};

// __FILE__ , __func__, _PRETTY_FUNCTION__, __LINE__)

void* operator new (size_t objSize, PlaceBuffer* placeP);
void* operator new (size_t objSize, PlaceBuffer& placeR);
void* operator new[] (size_t objSize, PlaceBuffer& placeR);

/////////////////////////////////////////////////////

template<int SIZE>
class PlaceBufferT : public PlaceBuffer
{
public:
    PlaceBufferT(const char* textP = "PlaceBufferT<>") : PlaceBuffer(textP) { }

    void* Alloc (size_t objSize) override { return (objSize <= SIZE) ? _buffer : nullptr; }

private:
    unsigned char _buffer[SIZE];
};

struct PlaceEvent : public PlaceBufferT<1000> {
    PlaceEvent( ) : PlaceBufferT<1000>("PlaceEvent") { }
};

struct PlaceIter : public PlaceBufferT<200> {
    PlaceIter( ) : PlaceBufferT<200>("PlaceIter") { }
};

struct PlaceEnumerator : public PlaceBufferT<200> {
    PlaceEnumerator( ) : PlaceBufferT<200>("PlaceEnumerator") { }
};

/////////////////////////////////////////////////////

struct PlaceGlobal : public PlaceBuffer {
    PlaceGlobal(const char* textP = "PlaceGlobal") : PlaceBuffer(textP) { }

    void* Alloc (size_t objSize) override { return new char[objSize]; }
};

struct PlaceSnmpColumns : public PlaceGlobal {
    PlaceSnmpColumns(const char* textP) : PlaceGlobal(textP) { }
};

/////////////////////////////////////////////////////

#endif /* __sk_PlaceBuffer_h__ */
