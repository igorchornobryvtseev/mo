
#ifndef CLIMO_H_INCLUDED
#define CLIMO_H_INCLUDED

#include "Mo.h"
#include "MoType.h"
#include "Value.h"

const unsigned int CliNameColumnLen = 24;

extern CliContext* cliP;

//////////////////////////////////////////////////////////

class WIDTH
{
public:
    WIDTH(Value* valueP, UINT32 width) : _valueP(valueP), _width(width) { }

    // private:
    Value* _valueP;
    UINT32 _width;
};

MyOutStream& operator<< (MyOutStream& dstR, WIDTH width);

//////////////////////////////////////////////////////////

class AttrArray
{
public:
    AttrArray(T_MoType moType) : _moType(moType) { }

    void Remove(AttrId attrId);

    AttrId GetAttr (int i) { return _array[i]._attrId; }

    bool IsUsed (int i) { return _array[i]._isUsed; }

    void SetUsed (int i) { _array[i]._isUsed = true; }

    bool IsMandat (int i) { return _array[i]._isMandat; }

    bool IsAttrUsed (AttrId attrId)
    {
        for ( UINT32 i = 0; i < _numOf; i++ ) {
            if ( (GetAttr(i) == attrId) && IsUsed(i) )
                return true;
        }
        return false;
    }

    void BeMandat (AttrId attrId)
    {
        for ( UINT32 i = 0; i < _numOf; i++ ) {
            if ( GetAttr(i) == attrId )
                _array[i]._isMandat = true;
        }
    }

    AttrArray& Add (AttrId attrId, int length = 0)
    {
        _array[_numOf]._attrId   = attrId;
        _array[_numOf]._isUsed   = false;
        _array[_numOf]._isMandat = false;
        _array[_numOf]._length   = length;

        _numOf++;
        return *this;
    }

    AttrArray& AddMandat (AttrId attrId, int length = 0)
    {
        _array[_numOf]._attrId   = attrId;
        _array[_numOf]._isUsed   = false;
        _array[_numOf]._isMandat = true;
        _array[_numOf]._length   = length;

        _numOf++;
        return *this;
    }

    UINT32 _numOf {0};

    struct Descr {
        AttrId _attrId;
        bool   _isMandat;
        bool   _isUsed;
        int    _length;
    };

    Descr    _array[100];
    T_MoType _moType;
};

///////////////////////////////////////////////////

class AttrListSet : public AttrArray
{
public:
    AttrListSet(T_MoType moType) : AttrArray(moType) { }

    void AddSettable ( )
    {
        for ( Mo::IteratorAttr it = SampleMo(_moType)->BeginAttr( ); it.IsValid( ); it++ ) {
            if ( it->IsSettable( ) )
                Add(it->__Id( ));
        }
    }
};

//////////////////////////////////////////////////////////

struct CheckerLearned {
    virtual ~CheckerLearned( ) = default;

    virtual bool IsLearned(Mo* moP) = 0;
};

class CliReaderIdBase : public ValueUnion
{
public:
    CliReaderIdBase(Value* valueBaseP, char* descrP) : _valueBaseP(valueBaseP), _descrP(descrP)
    {
        if ( (Mo::_cmdType == CMD_SHOW) || (Mo::_cmdType == CMD_CLEAR) )
            _valueReserved._dictionary.Add(WORD_ALL, "all");
        Mo::_readersId[Mo::_numOfReadersId++] = this;
    }

    enum { WORD_ALL = 100000, WORD_LEARNED = 100003 };

    void UseLearned ( ) { _valueReserved._dictionary.Add(WORD_LEARNED, "learned"); }

    bool IsLearned ( ) { return (_valueIndex == 0) ? (_valueReserved == WORD_LEARNED) : false; }

    void NotUseAll ( ) { _valueReserved._dictionary.Del(WORD_ALL); }

    bool IsAll ( ) { return (_valueIndex == 0) ? (_valueReserved == WORD_ALL) : false; }

    Value* GetCurrentValueP (UINT8 valueIndex) override
    {
        switch ( valueIndex ) {
            case 0: return &_valueReserved;
            case 1: return _valueBaseP;
            default: return nullptr;
        };
    }

    void DoGetCliHelp ( ) override
    {
        _valueReserved.DoGetCliHelp( );
        if ( _descrP != nullptr )
            ocli << Separator( ) << _descrP;
        else
            _valueBaseP->DoGetCliHelp( );
    }

    void DoSetCli (WordReader* inP) override
    {
        if ( inP->IsEnter( ) ) {
            if ( Mo::_cmdType == CMD_SHOW )
                SetString("all");  // show allows default value 'all'; clear has 'all' but no default values
            else                   // CMD_SET,  CMD_CLEAR
                cliP->ReturnError(RetStatus {"Missing mandatory parameter. Operation failed."});
        } else
            ValueUnion::DoSetCli(inP);
    }

    // help

    ValueDictInternalT<5> _valueReserved;
    Value*                _valueBaseP;

    char* _descrP;

    virtual MyStringT<64> HelpName(T_MoType moType) = 0;
    virtual bool          IsMatch(Mo* moP)          = 0;
};

template<class VAL>
class CliReaderId : public CliReaderIdBase
{
public:
    CliReaderId(AttrId attrId, char* descrP = nullptr) : CliReaderIdBase(&_valueVal, descrP), _attrId(attrId) { }

    VAL    _valueVal;
    AttrId _attrId;

    MyStringT<64> HelpName (T_MoType moType) override
    {
        MyStringT<64> name;
        name << SampleMo(moType)->FindAttr(_attrId)->GetNameP( );
        return name;
    }

    bool IsMatch (Mo* moP) override
    {
        if ( IsAll( ) )
            return true;

        Value* idValueP = moP->AttrValue(_attrId);
        return (_valueVal == idValueP);
    }
};

template<class VAL>
class CliReaderIdList : public CliReaderIdBase
{
private:
    VAL    _valueVal;
    AttrId _attrId;

public:
    CliReaderIdList(AttrId attrId, char* descrP = nullptr) : CliReaderIdBase(&_valueVal, descrP), _attrId(attrId) { }

    bool IsContain (UINT32 value) { return _valueVal.GetBit(value); }

    IteratorValueListOf BeginList ( ) { return _valueVal.BeginList( ); }

    MyStringT<64> HelpName (T_MoType moType) override
    {
        MyStringT<64> name;
        name << SampleMo(moType)->FindAttr(_attrId)->GetNameP( ) << "-list";
        return name;
    }

    bool IsMatch (Mo* moP) override
    {
        if ( IsAll( ) )
            return true;

        return IsContain(moP->AttrUint32(_attrId));
    }
};

//////////////////////////////////////////////////////////

class CliModifierShow : public ValueDictInternalT<80>
{
public:
    AttrArray _info;

    enum { MODIF_INFO = ATTR_MAX + 1, MODIF_STATS = ATTR_MAX + 2, MODIF_STATS_SUMMARY = ATTR_MAX + 3 };

    CliModifierShow(T_MoType moType);

    void AssignHasHistory ( ) { _hasHistory = true; }

    void Remove (AttrId attrId)
    {
        _dictionary.Del(attrId._attrType);
        _info.Remove(attrId);
    }

    AttrId GetAttr ( ) { return AttrId((T_AttrType)GetUint32( )); }

    bool IsInfo ( ) { return GetUint32( ) == MODIF_INFO; }

    bool IsStats ( ) { return GetUint32( ) == MODIF_STATS; }

    bool IsStatsSummary ( ) { return GetUint32( ) == MODIF_STATS_SUMMARY; }

    void DoSetCli(WordReader* inP) override;

    bool   _hasHistory {false};
    bool   _historyGiven;  // did the user enter any history parameter
    UINT32 _minHistory;
    UINT32 _maxHistory;
};

class CliModifierClear : public ValueDictInternalT<2>
{
public:
    enum { MODIF_STATS = 10001 };

    CliModifierClear(T_MoType moType)
    {
        Mo* sampleP = SampleMo(moType);
        if ( sampleP->GetCountersLine( ) )
            _dictionary.Add(MODIF_STATS, "statistics");
        _isCreatable = sampleP->IsCreatable( );
    }

    bool IsStats ( ) { return !_isClearObject && (GetUint32( ) == MODIF_STATS); }

    void DoSetCli (WordReader* inP) override
    {
        if ( inP->IsEnter( ) ) {
            if ( !_isCreatable )
                cliP->ReturnError(RetStatus {"Object deletion is not supported. Operation failed."});
            else
                _isClearObject = true;
        } else
            ValueDictInternalT<2>::DoSetCli(inP);
    }

    bool _isClearObject {false};
    bool _isCreatable {false};
};

//////////////////////////////////////////////////////////

#endif  // CLIMO_H_INCLUDED
