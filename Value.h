#ifndef VALUE_H_INCLUDED
#define VALUE_H_INCLUDED

#include "MyString.h"

// #include <cctype>
// #include <cstdio>

#include "BitMap.h"
// #include "CaExternal.h"
// #include "CaSharedRegion.h"
#include "CaStream.h"
// #include "cfm.h"
#include "CliContext.h"
// #include "Dictionary.h"
// #include "gl_types.h"
// #include "iniparser.h"
#include "IpAddress.h"
// #include "IpV6Address.h"
// #include "MacAddress.h"
// #include "MoType.h"
#include "RetStatus.h"
// #include "SysApi.h"
long mygettime( );

// ***************************************************************

enum T_DataControl { DATA_KEEP, DATA_RESTORE, DATA_SWAP, IS_DATA_CHANGED };

char* Separator( );

class Xml;

// ***************************************************************

// #if __cplusplus >= 202002L
// template<class V>
// concept ValueType = std::derived_from<V, Value>;
// #else
    #define ValueType class
// #endif

class Value
{
public:
    Value( ) = default;

    void Print (MyOutStream* outP) { DoGetCli(outP); }

    void      PrintHelp( );
    RetStatus SetString(const char* stringP);
    RetStatus SetString(std::string_view view);
    void      SetCli(WordReader* contextP);  // doesn't print error

    bool operator== (const char* stringP);

    bool operator!= (const char* stringP) { return !operator== (stringP); }

    bool operator== (const Value* secondP);

    bool operator== (const Value& secondR) { return operator== (&secondR); }

    bool operator!= (const Value* secondP) { return !operator== (secondP); }

    bool operator!= (const Value& secondR) { return operator!= (&secondR); }

    bool operator== (UINT32 value)
    {
        UINT32 src;
        DoGetUint32(&src);
        return (src == value);
    }

    bool operator!= (UINT32 value)
    {
        UINT32 src;
        DoGetUint32(&src);
        return (src != value);
    }

    UINT32 GetUint32 ( ) const
    {
        UINT32 dst;
        const_cast<Value*>(this)->DoGetUint32(&dst);  // ToDo: we don't  want go deepter now, but need one day
        return dst;
    }

    template<class ENUM>
    ENUM GetEnum ( ) const
    {
        return static_cast<ENUM>(GetUint32( ));
    }

    UINT32 GetOldUint32( );

    bool IsDataChanged ( ) { return DoDataControl(IS_DATA_CHANGED) != 0; }

    // protected:

    virtual ~Value( ) = default;

    virtual bool DoGetUint32 (UINT32* dstP)
    {
        (void)dstP;
        return false;
    }

    virtual UINT32 DoDataControl (T_DataControl dataControl)
    {
        (void)dataControl;
        return 0;
    }

    virtual void DoGetCli(MyOutStream* outP) = 0;

    virtual void DoGetCliHelp ( ) { }

    virtual void DoGetCliAutoCompletion (DictAutoCompletion* dstP) { (void)dstP; }

    virtual void DoSetCli(WordReader* inP) = 0;

    // word is part of a list in Cli if it can be separated by a comma or a period
    virtual bool IsWordInCliPartOfList( ) = 0;

    // virtual void DoGetXmlHelp(Xml* xmlP);

    // virtual void DoGetSnmp (SnmpVariable* varP) { (void)varP; }

    // virtual bool DoSetSnmp (SnmpVariable* varP)
    // {
    //     (void)varP;
    //     return false;
    // }

private:
    Value& operator= (const Value& secondR);
    Value(const Value& secondR);
};

// ***************************************************************

struct ValueProxy : public Value {
public:
    virtual Value* GetOriginalValue( ) = 0;

    bool DoGetUint32 (UINT32* dstP) override { return GetOriginalValue( )->DoGetUint32(dstP); }

    UINT32 DoDataControl (T_DataControl dataControl) override { return GetOriginalValue( )->DoDataControl(dataControl); }

    void DoGetCli (MyOutStream* outP) override { GetOriginalValue( )->DoGetCli(outP); }

    void DoGetCliHelp ( ) override { GetOriginalValue( )->DoGetCliHelp( ); }

    void DoGetCliAutoCompletion (DictAutoCompletion* dstP) override { GetOriginalValue( )->DoGetCliAutoCompletion(dstP); }

    void DoSetCli (WordReader* inP) override { GetOriginalValue( )->DoSetCli(inP); }

    bool IsWordInCliPartOfList ( ) override { return false; }

    // void DoGetXmlHelp (Xml* xmlP) override { GetOriginalValue( )->DoGetXmlHelp(xmlP); }

    // void DoGetSnmp (SnmpVariable* varP) override { GetOriginalValue( )->DoGetSnmp(varP); }

    // bool DoSetSnmp (SnmpVariable* varP) override { return GetOriginalValue( )->DoSetSnmp(varP); }
};

// ***************************************************************

MyOutStream& operator<< (MyOutStream& dstR, const Value* srcP);
MyOutStream& operator<< (MyOutStream& dstR, const Value& srcR);

// ***************************************************************

class ValueDictionary : public Value
{
public:
    // doesn't work if Mo is in the process of being recreated
    bool SetUint32 (UINT32 value)
    {
        for ( Start( ); IsValid( ); Next( ) ) {
            if ( DoIsMatch( ) && (GetUint32( ) == value) )
                return true;
        }
        return false;
    }

    template<class ENUM>
// #if __cplusplus > 202000L
//     requires std::is_enum_v<ENUM>
// #endif
        bool SetEnum (ENUM e)
    {
// #if __cplusplus < 202000L
//         static_assert(std::is_enum_v<ENUM>);
// #endif
        return SetUint32(static_cast<uint32_t>(e));
    }

    // for recreation of Mo
    bool SetUint32ForReCreation (UINT32 value)
    {
        for ( GetDictionary( )->___FirstEntry(&_context); IsValid( ); GetDictionary( )->___NextEntry(&_context) ) {
            if ( Entry( )._value == value ) {
                return true;
            }
        }
        return false;
    }

    virtual Dictionary* GetDictionary( ) = 0;

    void DoGetCliAutoCompletion(DictAutoCompletion* dstP) override;

    bool DoGetUint32(UINT32* dstP) override;
    void DoGetCli(MyOutStream* outP) override;
    void DoGetCliHelp( ) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    UINT32 DoDataControl(T_DataControl dataControl) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << GetUint32( ); }

    // bool DoSetSnmp (SnmpVariable* varP) override { return SetUint32(varP->AsUint32( )); }

    virtual bool DoIsMatch ( ) { return true; }

    Dictionary::Entry Entry ( ) { return GetDictionary( )->___GetEntry(_context); }

    void Start ( )
    {
        for ( GetDictionary( )->___FirstEntry(&_context); IsValid( ) && !DoIsMatch( ); GetDictionary( )->___NextEntry(&_context) )
            ;
    }

    void Next ( )
    {
        if ( !IsValid( ) )
            return;
        for ( GetDictionary( )->___NextEntry(&_context); IsValid( ) && !DoIsMatch( ); GetDictionary( )->___NextEntry(&_context) )
            ;
    }

    bool IsValid ( ) { return Entry( ).IsValid( ); }

protected:
    UINT32 _context {0};
    UINT32 _keepContext {0};
};

template<class DICTIONARY, UINT32 INIT_VAL = (unsigned)-1>
class ValueDictT : public ValueDictionary
{
public:
    ValueDictT( )
    {
        if ( INIT_VAL != ((UINT32)-1) )
            SetUint32(INIT_VAL);
    }

    Dictionary* GetDictionary ( ) override { return &_dictionary; }

    static DICTIONARY _dictionary;
};

template<class DICTIONARY, UINT32 INIT_VAL>
DICTIONARY ValueDictT<DICTIONARY, INIT_VAL>::_dictionary;

template<int MAX>
class ValueDictInternalT : public ValueDictionary
{
public:
    Dictionary* GetDictionary ( ) override { return &_dictionary; }

    DictionaryT<MAX> _dictionary;
};

class ValueDictExternal : public ValueDictionary
{
public:
    ValueDictExternal(Dictionary* dictionaryP) : _dictionaryP(dictionaryP) { }

    Dictionary* GetDictionary ( ) override { return _dictionaryP; }

    Dictionary* _dictionaryP;
};

// ***************************************************************

class ValueUnion : public Value
{
public:
    void DoGetCliHelp( ) override;

    virtual Value* GetCurrentValueP(UINT8 valueIndex) = 0;

    bool DoGetUint32(UINT32* dstP) override;
    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    UINT32 DoDataControl(T_DataControl dataControl) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << GetUint32( ); }

    void DoGetCliAutoCompletion (DictAutoCompletion* dstP) override
    {
        for ( int i = 0; GetCurrentValueP(i) != nullptr; i++ )
            GetCurrentValueP(i)->DoGetCliAutoCompletion(dstP);
    }

    // bool DoSetSnmp(SnmpVariable* varP) override;

protected:
    UINT32 _valueIndex {0};
    UINT32 _keepValueIndex {0};
};

// ***************************************************************

class ValueUnionExternal : public ValueUnion
{
public:
    ValueUnionExternal(Value* val_1P, Value* val_2P) : _value_1(val_1P), _value_2(val_2P) { }

    Value* GetCurrentValueP (UINT8 valueIndex) override
    {
        switch ( valueIndex ) {
            case 0: return _value_1;
            case 1: return _value_2;
            default: return nullptr;
        }
    }

    Value* _value_1;
    Value* _value_2;
};

// ***************************************************************

class ValueLong : public Value
{
public:
    ValueLong(UINT32 value = 0) { _value = _keepValue = value; }

    bool SetUint32 (UINT32 value)
    {
        if ( !InRange(value) )
            return false;
        _value = value;
        return true;
    }

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << GetUint32( ); }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     UINT32 value  = varP->AsUint32( );
    //     bool   result = SetUint32(value);
    //     return result;
    // }

    virtual bool InRange (INT32 value)
    {
        (void)value;
        return true;
    }

    virtual Dictionary* GetUint32Mapper ( ) { return nullptr; }

    // protected:
    bool DoGetUint32 (UINT32* dstP) override
    {
        *dstP = ((UINT32)_value);
        return true;
    }

    void DoGetCli(MyOutStream* outP) override;
    void DoGetCliHelp( ) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;
    void DoGetCliAutoCompletion(DictAutoCompletion* dstP) override;

    UINT32 DoDataControl(T_DataControl dataControl) override;

protected:
    INT32 _value;
    INT32 _keepValue;
};

class ValueLongHex : public ValueLong
{
public:
    ValueLongHex(UINT32 value = 0) : ValueLong(value) { }

    void DoGetCli (MyOutStream* outP) override
    {
        if ( GetUint32Mapper( ) != nullptr ) {
            UINT32 context;
            for ( GetUint32Mapper( )->___FirstEntry(&context); GetUint32Mapper( )->___GetEntry(context).IsValid( ); GetUint32Mapper( )->___NextEntry(&context) ) {
                if ( GetUint32Mapper( )->___GetEntry(context)._value != (UINT32)_value )
                    continue;
                outP->Print("%s", GetUint32Mapper( )->___GetEntry(context)._nameP);
                return;
            }
        }
        outP->Print("0x%lX", _value);
    }
};

// ***************************************************************

template<int MIN_VAL, int MAX_VAL, int INIT_VAL = MIN_VAL>
class ValueLongRangeT : public ValueLong
{
public:
    ValueLongRangeT( ) : ValueLong(INIT_VAL) { }

    bool InRange (INT32 value) override { return value >= MIN_VAL && value <= MAX_VAL; }

    void DoGetCliHelp ( ) override { ocli.Print("%sinteger %d..%d", Separator( ), MIN_VAL, MAX_VAL); }

    // void DoGetXmlHelp (Xml* xmlP) override { xmlP->Print("<format type=\"integer\" min=\"%d\" max=\"%d\"/>", MIN_VAL, MAX_VAL); }
};

// ***************************************************************

class ValueFloat : public Value
{
public:
    ValueFloat( );

    void SetFloat(float value);

    float GetFloat ( ) const { return _value; }

protected:
    UINT32 DoDataControl(T_DataControl dataControl) override;

    float _value;
    float _keepValue;
};

class ValueFloatScientific : public ValueFloat
{
protected:
    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;
};

class ValueFloatNormal : public ValueFloat
{
protected:
    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;
};

// ***************************************************************

class ValueChain : public Value
{
public:
    virtual Value* GetCurrentChainValueP(UINT8 valueIndex) = 0;
    virtual Value* GetAllValueP(UINT8 valueIndex)          = 0;

    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    UINT32 DoDataControl(T_DataControl dataControl) override;
};

// ***************************************************************

struct IteratorValueListOf : public BitMap::Iterator {
    IteratorValueListOf(BitMap* ownerP) : BitMap::Iterator(ownerP) { }
};

class ValueListOf : public Value
{
private:
    BitMapDynamic _bitMap;
    BitMapDynamic _keep;

    bool _wasTouched {false};

protected:
    void _SetAllBits ( )
    {
        _wasTouched = true;
        _bitMap.SetAll(true);
    }

    void _CleanAllBits ( )
    {
        _wasTouched = true;
        _bitMap.Clean( );
    }

    bool SetBit (int number, bool value)
    {
        _wasTouched = true;
        return _bitMap.Set(number, value);
    }

    bool _SetBitRange (int minNum, int maxNum)
    {
        _wasTouched = true;
        return (minNum > maxNum) ? false : _bitMap.SetRange(minNum, maxNum, true);
    }

    virtual Value*    GetIoBaseP( )                       = 0;
    virtual RetStatus SetListElement(ListElement element) = 0;

public:
    ValueListOf(int minNum, int maxNum)
    {
        _bitMap.Init(minNum, maxNum);
        _keep.Init(minNum, maxNum);
    }

    bool GetBit (int number) { return _bitMap.Get(number); }

    bool GetBitOld (int number) { return _keep.Get(number); }

    bool IsEmpty ( ) { return _bitMap.IsEmpty( ); }

    bool operator&& (const ValueListOf& otherR) const { return (_bitMap && otherR._bitMap); }

    IteratorValueListOf BeginList ( ) { return IteratorValueListOf(&_bitMap); }

    IteratorValueListOf BeginOldList ( ) { return IteratorValueListOf(&_keep); }

    void DoGetCliAutoCompletion (DictAutoCompletion* dstP) override { GetIoBaseP( )->DoGetCliAutoCompletion(dstP); }

    void DoGetCliHelp ( ) override
    {
        ocli.Print("%slist: ", Separator( ));
        GetIoBaseP( )->DoGetCliHelp( );
    }

    void DoSetCli (WordReader* inP) override
    {
        _CleanAllBits( );

        DictAutoCompletion dict;
        DoGetCliAutoCompletion(&dict);

        for ( ;; ) {
            if ( inP->NowAutoComplete( ) )
                return inP->ReturnAutoComplete(&dict);

            ListElement element = inP->ReadListElement( );
            inP->SetStatus(element.GetStatus( ));
            if ( !inP->_status.ok( ) )
                return;

            RetStatus status = SetListElement(element);
            if ( !status.ok( ) )
                return inP->SetStatus(RetStatus {E_RetStatus::Parsing});

            if ( !inP->TestNext(",") )
                break;
            inP->Read( );  // just skip comma
        }
    }

    bool IsWordInCliPartOfList ( ) override { return true; }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     BitMapDynamic tmp;
    //     tmp.Init(_bitMap.MinNum( ), _bitMap.MaxNum( ));

    //     bool isOk = varP->AsBitmap(&tmp);
    //     if ( !isOk )
    //         return false;

    //     for ( BitMap::Iterator it = tmp.Begin( ); it.IsValid( ); it++ )
    //         SetListElement(ListElement::NumberSnmp(*it));

    //     return true;
    // }

    UINT32 DoDataControl (T_DataControl dataControl) override
    {
        if ( _wasTouched ) {
            if ( dataControl == IS_DATA_CHANGED )
                return !_bitMap.IsEqual(&_keep);
            if ( dataControl == DATA_KEEP ) {
                _keep.CopyFrom(&_bitMap);
                _wasTouched = false;
            } else if ( dataControl == DATA_RESTORE ) {
                _bitMap.CopyFrom(&_keep);
                _wasTouched = false;
            } else if ( dataControl == DATA_SWAP )
                _bitMap.Swap(&_keep);
            return 0;
        }
        return 0;
    }

    // void DoGetSnmp (SnmpVariable* pduP) override
    // {
    //     std::unique_ptr<uint8_t[]> tmp {new uint8_t[_bitMap.NumBytes( )]};
    //     int                        numBytes = _bitMap.GetBytes(tmp.get( ), sizeof(tmp));
    //     pduP->setValueArray(tmp.get( ), numBytes);
    // }

    void DoGetCli (MyOutStream* outP) override
    {
        BitMapDynamic tmp;
        tmp.Init(_bitMap.MinNum( ), _bitMap.MaxNum( ));
        tmp.CopyFrom(&_bitMap);

        bool               notFirst = false;
        DictAutoCompletion dict;
        DoGetCliAutoCompletion(&dict);
        UINT32 context;
        for ( dict.___FirstEntry(&context); dict.___GetEntry(context).IsValid( ); dict.___NextEntry(&context) ) {
            int number = dict.___GetEntry(context)._value;
            if ( tmp.Get(number) ) {
                tmp.Set(number, false);

                if ( notFirst )
                    *outP << ",";
                notFirst = true;

                *outP << dict.___GetEntry(context)._nameP;
            }
        }

        if ( !tmp.IsEmpty( ) ) {
            if ( notFirst )
                *outP << ",";
            tmp.PrintAsRange(outP);
        }
    }
};

// ***************************************************************

template<class IO_BASE, int MIN_NUM, int MAX_NUM>
class ValueListOfDictionaryT : public ValueListOf
{
public:
    ValueListOfDictionaryT( ) : ValueListOf(MIN_NUM, MAX_NUM) { }

protected:
    Value* GetIoBaseP ( ) override
    {
        static IO_BASE ioBase;
        return &ioBase;
    }

    RetStatus SetListElement (ListElement element) override
    {
        if ( element.IsSnmp( ) ) {
            bool isOk = IO_BASE( ).SetUint32(element.GetInt( ));
            if ( isOk )
                SetBit(element.GetInt( ), true);
            return RetStatus {isOk ? E_RetStatus::Ok : E_RetStatus::Parsing};
        }

        if ( element.IsRange( ) )
            return RetStatus {E_RetStatus::Parsing};
        IO_BASE   ioBase;
        RetStatus status = ioBase.SetString(element.GetStrP( ));
        if ( status.ok( ) )
            SetBit(ioBase.GetUint32( ), true);
        return status;
    }
};

// ***************************************************************

template<int MIN_NUM, int MAX_NUM>
class ValueListOfLongRangeT : public ValueListOf
{
public:
    ValueListOfLongRangeT( ) : ValueListOf(MIN_NUM, MAX_NUM) { }

protected:
    typedef ValueLongRangeT<MIN_NUM, MAX_NUM> IO_BASE;

    Value* GetIoBaseP ( ) override
    {
        static IO_BASE ioBase;
        return &ioBase;
    }

    RetStatus SetListElement (ListElement element) override
    {
        if ( element.IsSnmp( ) ) {
            bool isOk = IO_BASE( ).SetUint32(element.GetInt( ));
            if ( isOk )
                SetBit(element.GetInt( ), true);
            return RetStatus {isOk ? E_RetStatus::Ok : E_RetStatus::Parsing};
        }
        if ( element.IsRange( ) )
            return RetStatus {_SetBitRange(element.GetMinRange( ), element.GetMaxRange( )) ? E_RetStatus::Ok : E_RetStatus::Parsing};
        IO_BASE   ioBase;
        RetStatus status = ioBase.SetString(element.GetStrP( ));
        if ( status.ok( ) )
            SetBit(ioBase.GetUint32( ), true);
        return status;
    }

public:
    void SetAll ( ) { _SetAllBits( ); }
};

// ***************************************************************

// class ValueFdbIdList : public ValueListOfLongRangeT<1, MAX_FDB_ID>
// {
// public:
//     ValueFdbIdList( ) { SetBit(1, true); }
// };

class ValueVlanIdList : public ValueListOfLongRangeT<MIN_VLAN_ID, MAX_VLAN_ID>
{
};

/*
class ValueCfmComponentIndex: public ValueLongRangeT<1, 255>
{
public:
    bool GetBit(int value)
    {
        return (GetUint32() >> value) & 0x01;
    }
};
class ValueCfmComponentIndexList: public ValueListOfLongRangeT<1, 255> {};
*/

// class ValueCfmMdIndex : public ValueLongRangeT<1, CFM_CA_MD_MAX_INDEX>
// {
// };

// class ValueCfmMdIndexList : public ValueListOfLongRangeT<1, CFM_CA_MD_MAX_INDEX>
// {
// };

// class ValueCfmMaIndex : public ValueLongRangeT<1, CFM_CA_MA_MAX_INDEX>
// {
// };

// class ValueCfmMaIndexList : public ValueListOfLongRangeT<1, CFM_CA_MA_MAX_INDEX>
// {
// };

// class ValueCfmMepId : public ValueLongRangeT<1, CFM_MEP_ID_MAX_VALUE>
// {
// };

// class ValueCfmMepIdList : public ValueListOfLongRangeT<1, CFM_MEP_ID_MAX_VALUE>
// {
// };

// class ValueInventoryList : public ValueListOfLongRangeT<E_INVENT_ROOT, E_INVENT_MCU>
// {
// };

class ValueEvcId : public ValueLongRangeT<0, 4095>
{
};

class ValueEvcIdList : public ValueListOfLongRangeT<0, 4095>
{
};

class ValueCosId : public ValueLongRangeT<0, 7>
{
};

class ValueCosIdList : public ValueListOfLongRangeT<0, 7>
{
};

struct ValueFdbQuotaId : public ValueLongRangeT<1, 255> {
};

struct ValueQuota : public ValueLongRangeT<1, 3712> {
};

struct ValueFdbEvc2QuotaId : public ValueLongRangeT<1, 255> {
};

struct ValueFdbEvc2QuotaIdList : public ValueListOfLongRangeT<1, 255> {
};

struct ValuePcpWriteProfileId : public ValueLongRangeT<1, 255> {
};

// ***************************************************************
#if 0
class ValueIpV6 : public Value
{
public:
    ValueIpV6( );
    ValueIpV6(const IpV6Address& ipv6);

    void               SetIpV6(const IpV6Address& ipv6);
    void               SetIpV6(UINT8* ipv6Bytes);
    const IpV6Address& GetIpV6( ) const;
    const IpV6Address& GetOldIpV6( ) const;

    IpV6Address AsStruct ( ) { return _value; }

    void DoGetCli(MyOutStream* outP) override;
    void DoGetCliHelp( ) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    UINT32 DoDataControl(T_DataControl dataControl) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << _value; }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     char*  srcP;
    //     size_t srcLen;
    //     bool   result = varP->AsString(srcP, srcLen);
    //     if ( result && (srcLen == 6) ) {
    //         SetIpV6((UINT8*)srcP);
    //         return true;
    //     }
    //     return false;
    // }

private:
    IpV6Address _value;
    IpV6Address _keepValue;
};
#endif
class ValueIp : public ValueLong
{
public:
    ValueIp( );

    IpAddress GetIp( ) const;
    void      SetIp(IpAddress aValue);

    IpAddress GetOldIp( );

    void DoGetCli(MyOutStream* outP) override;

    void DoGetCliHelp ( ) override { ocli.Print("%sip", Separator( )); }

    void DoSetCli(WordReader* inP) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << GetIp( ); }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     SetIp(varP->AsIp( ));
    //     return true;
    // }
};
#if 0
// TODO: redo methods to const where it is appropriate -- blocked by genius design of Value class...
struct ValueIpV4orV6 : public ValueUnion {
    bool IsV4 ( ) const { return _valueIndex == 0; }

    bool IsV6 ( ) const { return !IsV4( ); }

    IpAddress GetIpV4 ( ) { return _valueIpV4.GetIp( ); }

    // IpV6Address GetIpV6 ( ) { return _valueIpV6.GetIpV6( ); }

    // void DoGetSnmp (SnmpVariable* outP) override
    // {
    //     if ( IsV4( ) )
    //         *outP << GetIpV4( );
    //     else
    //         *outP << GetIpV6( );
    // }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     if ( varP->_valueLen == 4 ) {
    //         _valueIndex = 0;
    //         return _valueIpV4.DoSetSnmp(varP);
    //     }
    //     if ( varP->_valueLen == 16 ) {
    //         _valueIndex = 1;
    //         return _valueIpV6.DoSetSnmp(varP);
    //     }
    //     return false;
    // }

    void SetValue (const IpAddress& ip)
    {
        _valueIndex = 0;
        _valueIpV4.SetIp(ip);
    }

    void SetValue (const IpV6Address& ip)
    {
        _valueIndex = 1;
        _valueIpV6.SetIpV6(ip);
    }

    bool operator== (const IpV6Address& ipv6) { return IsV6( ) && _valueIpV6.GetIpV6( ) == ipv6; }

    bool operator== (const IpAddress& ipv4) { return IsV4( ) && _valueIpV4.GetIp( ) == ipv4; }

    Value* GetCurrentValueP (UINT8 valueIndex) override
    {
        switch ( valueIndex ) {
            case 0: return &_valueIpV4;
            case 1: return &_valueIpV6;
            default: return nullptr;
        }
    }

    T_IP_V4_OR_V6 AsStruct ( )
    {
        T_IP_V4_OR_V6 result;

        if ( _valueIndex == 1 ) {
            // V6
            result.isV6 = E_TRUE;
            strncpy(result.ip, _valueIpV6.GetIpV6( ).str( ).c_str( ), std::size(result.ip));
        } else {
            // V4
            result.isV6 = E_FALSE;
            strncpy(result.ip, _valueIpV4.GetIp( ).AsString( ), std::size(result.ip));
        }
        return result;
    }

    ValueIp   _valueIpV4;
    ValueIpV6 _valueIpV6;
};
#endif
// ***************************************************************

class ValueIpMask : public ValueIp
{
public:
    bool IsValidMask( ) const;
};

// ***************************************************************

class ValueIpAndMask : public ValueIp
{
public:
    UINT32 GetMask( ) const;
    UINT32 GetMaskAsPrefix( ) const;

protected:
    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;

    UINT32 _mask {0};
};

// ***************************************************************

class ValueIpAndUdpPort : public ValueIp
{
public:
    uint32_t GetUdpPort ( ) const { return _udpPort; }

protected:
    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;

    uint32_t _udpPort {0};
};

// ***************************************************************
#if 0
class ValueMac : public Value
{
public:
    ValueMac( ) = default;
    ValueMac(const Mac& mac);

    void SetMac(const Mac& mac);
    void SetMac(const UINT8* macBytes);
    Mac  GetMac( );

    void DoGetCli(MyOutStream* outP) override;
    void DoGetCliHelp( ) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    UINT32 DoDataControl(T_DataControl dataControl) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << _value; }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     char*  srcP;
    //     size_t srcLen;
    //     bool   result = varP->AsString(srcP, srcLen);
    //     if ( result && srcLen == 6 ) {
    //         SetMac((UINT8*)srcP);
    //         return true;
    //     }
    //     return false;
    // }

private:
    Mac _value;
    Mac _keepValue;
};
#endif
// ***************************************************************

class ValueCounter64 : public Value
{
public:
    ValueCounter64(UINT64 value = 0);

    void AssignSignedPrintFormat(bool signedPrintFormat);

    void SetUint64(UINT64 value);

    UINT64 GetUint64 ( ) { return _value; }

protected:
    bool DoGetUint32 (UINT32* dstP) override
    {
        *dstP = _value;
        return true;
    }

    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << GetUint64( ); }

private:
    bool   _signedPrintFormat {false};
    UINT64 _value;
};

// ***************************************************************

class ValueCounterFloat : public Value
{
public:
    void SetFloat(float value);

protected:
    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

private:
    float _value {0};
};

// ***************************************************************

class ValueString : public Value
{
public:
    virtual char*  GetP( )      = 0;
    virtual UINT32 GetMaxLen( ) = 0;

    virtual void set(std::string_view src) = 0;

    void CopyString (const char* srcP) { SetIntValue(srcP); }

    void SetIntValue (const char* srcP)
    {
        strncpy(GetP( ), srcP, GetMaxLen( ));
        GetP( )[GetMaxLen( )] = '\0';
    }

    bool StartsWith(const char* secondP);
    bool EndsWith(const char* secondP);

    void ToString (char* dstP, UINT32 maxDstLen)
    {
        UINT32 len = strlen(GetP( ));
        if ( len >= (maxDstLen - 1) )
            len = maxDstLen - 1;
        memcpy(dstP, GetP( ), len);
        dstP[len] = 0;
    }

    std::string_view view ( ) /*const*/ { return {GetP( ), strlen(GetP( ))}; }

    // protected:
    void DoGetCli(MyOutStream* outP) override;

    void DoGetCliHelp ( ) override { ocli.Print("%sstring", Separator( )); }

    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    // void DoGetSnmp (SnmpVariable* outP) override { *outP << view( ); }

    // bool DoSetSnmp (SnmpVariable* varP) override
    // {
    //     char*  srcP;
    //     size_t srcLen;
    //     bool   result = varP->AsString(srcP, srcLen);
    //     if ( result )
    //         SetIntValue(srcP);
    //     return result;
    // }
};

// ***************************************************************

template<UINT32 MAX>
class ValueStringT : public ValueString
{
public:
    char* GetP ( ) override { return _value.AsStr( ); }

    UINT32 GetMaxLen ( ) override { return MAX; }

    void set (std::string_view src) override { _value.Set(src); }

    MyStringT<MAX>* GetStringP ( ) { return &_value; }

    void Clean ( ) { _value.Clean( ); }

    UINT32 DoDataControl (T_DataControl dataControl) override
    {
        if ( dataControl == IS_DATA_CHANGED ) {
            return (_keepValue == _value) ? 0 : 1;
        }
        if ( dataControl == DATA_KEEP )
            _keepValue = _value;
        else if ( dataControl == DATA_RESTORE )
            SetIntValue(_keepValue.AsStr( ));
        else if ( dataControl == DATA_SWAP ) {
            MyStringT<MAX> tmp = _value;
            SetIntValue(_keepValue.AsStr( ));
            _keepValue = tmp;
        }
        return 0;
    }

protected:
    MyStringT<MAX> _value;
    MyStringT<MAX> _keepValue;
};

class ValueString16 : public ValueStringT<16>
{
public:
    ValueString16( ) : ValueStringT<16>( ) { }
};

class ValueString32 : public ValueStringT<32>
{
public:
    ValueString32( ) : ValueStringT<32>( ) { }
};

class ValueString64 : public ValueStringT<64>
{
public:
    ValueString64( ) : ValueStringT<64>( ) { }
};

class ValueString256 : public ValueStringT<256>
{
public:
    ValueString256( ) : ValueStringT<256>( ) { }
};

// ***************************************************************

// class ValueTimeSecs : public Value
// {
// public:
//     ValueTimeSecs( ) : _baseTimeSecs(0) { }

//     ValueTimeSecs(time_t baseTimeSecs) : _baseTimeSecs(baseTimeSecs) { }

//     void UpdateBaseTime( );

//     // void DoGetSnmp (SnmpVariable* outP) override { *outP << GetUint32( ) * 100; }

//     time_t GetBaseTime ( ) { return _baseTimeSecs; }

//     bool DoGetUint32 (UINT32* dstP) override
//     {
//         *dstP = mygettime( ) - _baseTimeSecs;
//         return true;
//     }

//     void DoGetCli(MyOutStream* outP) override;
//     void DoSetCli(WordReader* inP) override;
//     bool IsWordInCliPartOfList( ) override;

//     time_t _baseTimeSecs;

//     //  char    itsBuff[8]; // for SNMP only
// };

// ***************************************************************

struct ValueVlanId : public ValueLongRangeT<MIN_VLAN_ID, MAX_VLAN_ID> {
};

// ***************************************************************

class ValueSnmpOnly : public Value
{
public:
    void DoGetCli (MyOutStream* outP) override { *outP << "SNMP-ONLY"; }

    void DoSetCli (WordReader* inP) override  // stub
    {
        (void)inP;
    }

    bool IsWordInCliPartOfList ( ) override { return false; }
};

// ***************************************************************

class ValueVlanIdOrNone : public ValueUnion
{
public:
    Value* GetCurrentValueP(UINT8 valueIndex) override;

    DICTIONARY_1(NoneWord, 0, "none");
    ValueDictT<NoneWord> _valueNone;
    ValueVlanId          _valueVlanId;
};

// ***************************************************************

class ValueDateAndTime : public Value
{
public:
    ValueDateAndTime( );
    ValueDateAndTime(time_t absTime);

    void PrintTimeOnly ( ) { _printTimeOnly = true; }

    void   UpdateTime( );
    void   SetTime(time_t absTime);
    time_t GetTime( );

    void DoGetCli(MyOutStream* outP) override;
    void DoSetCli(WordReader* inP) override;
    bool IsWordInCliPartOfList( ) override;

    time_t _absTime;
    bool   _printTimeOnly;
};

// ***************************************************************

template<class V>
struct ValueTouchable : public V {
    void Touch ( ) { _isTouched = true; }

    void DoSetCli (WordReader* inP) override
    {
        Touch( );
        V::DoSetCli(inP);
    }

    UINT32 DoDataControl (T_DataControl dataControl) override
    {
        if ( (dataControl == IS_DATA_CHANGED) && _isTouched )
            return 1;
        if ( (dataControl == DATA_KEEP) || (dataControl == DATA_RESTORE) )
            _isTouched = false;
        return V::DoDataControl(dataControl);
    }

    bool _isTouched {false};
};

// ***************************************************************

class ValueBridgeId : public ValueLongRangeT<1, MAX_NEW_BRIDGE_ID, 1>
{
};

class ValueBridgeIdList : public ValueListOfLongRangeT<1, MAX_NEW_BRIDGE_ID>
{
};

// struct ValueLeg : public Value {
//     void DoGetCliHelp ( ) override { ocli.Print("<ethnum>[.s-<vlan>][.<vlan>]"); }

//     void DoGetCli (MyOutStream* outP) override { _leg.PrintTo(outP); }

//     UINT32 DoDataControl(T_DataControl dataControl) override;

//     void DoSetCli(WordReader* inP) override;
//     bool IsWordInCliPartOfList( ) override;

//     BridgeLeg _leg;
//     BridgeLeg _keepLeg;
// };

// ***************************************************************

#endif  // VALUE_H_INCLUDED
