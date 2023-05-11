
#ifndef CASTREAM_H_INCLUDED
#define CASTREAM_H_INCLUDED

#include <stdio.h>

// #include "gl_types.h"
#include "MoType.h"
#include "MyString.h"

///////////////////////////////////////////

class CliOutStream : public MyOutStream
{
public:
    CliOutStream( ) { AttachBuffer(_internalBuffer, sizeof(_internalBuffer)); }

    void AttachBuffer (char* bufferP, UINT32 bufferLen)
    {
        _bufferP = bufferP;
        _maxLen  = bufferLen - 1;
        Clean( );
    }

    void DetachBuffer ( ) { AttachBuffer(_internalBuffer, sizeof(_internalBuffer)); }

    char* AsStr ( ) { return _bufferP; }

    UINT32 GetLen ( ) { return _len; }

    void DoPrint (const char* formatP, va_list* listP) override
    {
        int remainedLen = _maxLen - _len - 1;
        if ( remainedLen <= 0 )
            return;
        int addLen = vsnprintf(_bufferP + _len, remainedLen, formatP, *listP);
        LOG("_bufferP[%d]='%s'", addLen, _bufferP);
        if ( addLen >= remainedLen )
            addLen = remainedLen - 1;
        _len += addLen;
    }

    void DoClean ( ) override
    {
        _bufferP[0] = 0;
        _len        = 0;
    }

//private:
    char*  _bufferP;
    UINT32 _maxLen;
    UINT32 _len;

    char _internalBuffer[16000];
};

extern CliOutStream ocli;

//////////////////////////////////////////////////////

class Value;
class Mo;

enum XmlActionType { XML_SHOW_INFO, XML_SHOW_SHORT, XML_SHOW_STATS_HISTORY, XML_SHOW_HELP_MO };

class Xml
{
public:
    Xml(MyOutStream* outP) : _outP(outP) { }

    static void TagEndCode(MyOutStream* outP, const char* endCodeP);
    static void TagText(MyOutStream* outP, const char* textP);

    static void EncodeText(char* dstP, size_t maxDstLen, const char* textP);

    Xml& TagEndCode(const char* endCodeP);
    Xml& TagText(const char* textP);

    void SetShowActionType (XmlActionType action) { _action = action; }

    void TagMo(Mo* moP);
    void TagMo(char* typeP, char* nameP);
    void TagMoEnd( );

    void TagAttr(const char* nameP, const char* valueP);
    void TagAttr(const char* nameP, int value);
    void TagAttr(const char* nameP, Value* valueP);
    void TagAttr(Mo* moP, T_AttrType attrType);

    Xml* Attr(const char* leftP, const char* rightP);  // left="right"

    Xml* Print(const char* format, ...);

    MyOutStream*  _outP;
    XmlActionType _action {XmlActionType::XML_SHOW_SHORT};
};

//////////////////////////////////////////////////////

void FatalError(const char* fromP, const char* errP);

//////////////////////////////////////////////////////

#endif  // CASTREAM_H_INCLUDED
