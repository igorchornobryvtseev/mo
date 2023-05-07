#include "igorc_common.h"

// #include "Value.h"

// #include <ctype.h>
// #include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// #include "Database.h"
// #include "Mo.h"
// #include "MoBridge.h"

void ErrSw (const char* textP)
{
    (void)textP;
}

// ***************************************************************

// void Value::DoGetXmlHelp(Xml* xmlP)
// {
//     xmlP->Print("<format type=\"common\"/>");
// }

UINT32 Value::GetOldUint32( )
{
    DoDataControl(DATA_SWAP);
    UINT32 uint32Value = GetUint32( );
    DoDataControl(DATA_SWAP);
    return uint32Value;
}

static bool isFirst = true;

char* Separator ( )
{
    const char* retP = isFirst ? "" : " | ";
    isFirst          = false;
    return (char*)retP;
}

void Value::PrintHelp( )
{
    isFirst = true;
    DoGetCliHelp( );
}

void Value::SetCli(WordReader* contextP)
{
    DoSetCli(contextP);
}

bool Value::operator== (const Value* secondP)
{
    UINT32 firstUint32;
    UINT32 secondUint32;
    if ( DoGetUint32(&firstUint32) && ((Value*)secondP)->DoGetUint32(&secondUint32) )
        return (firstUint32 == secondUint32);

    enum { MAX_STRING_LENGTH = 64 };

    MyStringT<MAX_STRING_LENGTH + 2> firstOut;
    firstOut << this;
    UINT32 firstLen = firstOut.GetLen( );

    MyStringT<MAX_STRING_LENGTH + 2> secondOut;
    secondOut << *secondP;
    UINT32 secondLen = secondOut.GetLen( );

    if ( firstLen != secondLen )
        return false;

    if ( firstLen > MAX_STRING_LENGTH )
        return false;

    return (memcmp(firstOut.AsStr( ), secondOut.AsStr( ), firstLen) == 0);
}

bool Value::operator== (const char* stringP)
{
    enum { MAX_STRING_LENGTH = 256 };

    MyStringT<MAX_STRING_LENGTH + 2> out;
    out << this;

    if ( out.GetLen( ) > MAX_STRING_LENGTH )
        return false;

    return (out == stringP);
}

RetStatus Value::SetString(const char* stringP)
{
    WordReaderString reader(stringP);
    DoSetCli(&reader);
    return reader.Status( );
}

RetStatus Value::SetString(std::string_view view)
{
    std::string      str {view};
    WordReaderString reader(str.c_str( ));
    DoSetCli(&reader);
    return reader.Status( );
}

// ***************************************************************

MyOutStream& operator<< (MyOutStream& dstR, const Value* srcP)
{
    ((Value*)srcP)->Print(&dstR);
    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, const Value& srcR)
{
    ((Value&)srcR).Print(&dstR);
    return dstR;
}

// ***************************************************************

void ValueDictionary::DoGetCliAutoCompletion(DictAutoCompletion* dstP)
{
    for ( Start( ); IsValid( ); Next( ) )
        if ( DoIsMatch( ) )
            dstP->Add(Entry( ));
}

void ValueDictionary::DoGetCli(MyOutStream* outP)
{
    outP->Print("%s", Entry( )._nameP);
}

bool ValueDictionary::DoGetUint32(UINT32* dstP)
{
    *dstP = Entry( )._value;
    return true;
}

void ValueDictionary::DoGetCliHelp( )
{
    for ( Start( ); IsValid( ); Next( ) ) {
        if ( DoIsMatch( ) ) {
            MyStringT<64> tmp;
            Print(&tmp);
            ocli.Print("%s%s", Separator( ), tmp.AsStr( ));
        }
    }
}

void ValueDictionary::DoSetCli(WordReader* inP)
{
    if ( inP->NowAutoComplete( ) ) {
        DictAutoCompletion dict;
        DoGetCliAutoCompletion(&dict);

        inP->ReturnAutoComplete(&dict);
        return;
    }

    if ( !inP->Read( ).IsOk( ) )
        return;

    for ( Start( ); IsValid( ); Next( ) ) {
        if ( !DoIsMatch( ) )
            continue;

        MyStringT<128> current;
        DoGetCli(&current);
        if ( current == inP->_dst )
            return;
    }
    inP->SetStatus(RetStatus {E_RetStatus::Parsing});
}

bool ValueDictionary::IsWordInCliPartOfList( )
{
    return false;
}

UINT32 ValueDictionary::DoDataControl(T_DataControl dataControl)
{
    if ( dataControl == IS_DATA_CHANGED )
        return (_keepContext != _context) ? 1 : 0;
    if ( dataControl == DATA_KEEP )
        _keepContext = _context;
    else if ( dataControl == DATA_RESTORE )
        _context = _keepContext;
    else if ( dataControl == DATA_SWAP ) {
        int tmp      = _context;
        _context     = _keepContext;
        _keepContext = tmp;
    }
    return 0;
}

// ***************************************************************

void ValueLong::DoGetCliAutoCompletion(DictAutoCompletion* dstP)
{
    if ( GetUint32Mapper( ) != nullptr ) {
        UINT32 context;
        for ( GetUint32Mapper( )->___FirstEntry(&context);; GetUint32Mapper( )->___NextEntry(&context) ) {
            if ( !GetUint32Mapper( )->___GetEntry(context).IsValid( ) )
                break;
            dstP->Add(GetUint32Mapper( )->___GetEntry(context));
        }
    }
}

void ValueLong::DoSetCli(WordReader* inP)
{
    if ( inP->NowAutoComplete( ) ) {
        if ( GetUint32Mapper( ) != nullptr ) {
            DictAutoCompletion dict;
            DoGetCliAutoCompletion(&dict);
            inP->ReturnAutoComplete(&dict);
        } else
            inP->ReturnAutoComplete(nullptr);

        return;
    }

    if ( !inP->Read( ).IsOk( ) )
        return;

    bool  foundEnum = false;
    INT32 result;

    if ( GetUint32Mapper( ) != nullptr ) {
        UINT32 context;
        for ( GetUint32Mapper( )->___FirstEntry(&context);; GetUint32Mapper( )->___NextEntry(&context) ) {
            Dictionary::Entry entry = GetUint32Mapper( )->___GetEntry(context);
            if ( !entry.IsValid( ) )
                break;
            if ( 0 == strcasecmp(entry._nameP, inP->_dst.AsStr( )) ) {
                result    = entry._value;
                foundEnum = true;
                break;
            }
        }
    }

    if ( !foundEnum ) {
        errno = 0;
        char* endP;
        result = strtol(inP->_dst.AsStr( ), &endP, 0);
        if ( (*endP != 0) || (errno == ERANGE) )  // stopped due to unknown char due to RANGE
        {
            inP->SetStatus(RetStatus {E_RetStatus::Parsing});
            return;
        }
    }

    if ( !SetUint32(result) )
        inP->SetStatus(RetStatus {E_RetStatus::Range});
}

void ValueLong::DoGetCli(MyOutStream* outP)
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
    outP->Print("%ld", _value);
}

bool ValueLong::IsWordInCliPartOfList( )
{
    return false;
}

void ValueLong::DoGetCliHelp( )
{
    if ( GetUint32Mapper( ) != nullptr ) {
        UINT32 context;
        for ( GetUint32Mapper( )->___FirstEntry(&context); GetUint32Mapper( )->___GetEntry(context).IsValid( ); GetUint32Mapper( )->___NextEntry(&context) )
            ocli.Print("%s%s", Separator( ), GetUint32Mapper( )->___GetEntry(context)._nameP);
    }
    ocli.Print("%sinteger", Separator( ));
}

UINT32 ValueLong::DoDataControl(T_DataControl dataControl)
{
    if ( dataControl == IS_DATA_CHANGED )
        return (_keepValue != _value) ? 1 : 0;
    if ( dataControl == DATA_KEEP )
        _keepValue = _value;
    else if ( dataControl == DATA_RESTORE )
        _value = _keepValue;
    else if ( dataControl == DATA_SWAP ) {
        UINT32 tmp = (UINT32)_value;
        _value     = _keepValue;
        _keepValue = (INT32)tmp;
    }
    return 0;
}

// ***************************************************************

// UINT32 ValueFloat::DoDataControl(T_DataControl dataControl)
// {
//     if ( dataControl == IS_DATA_CHANGED )
//         return (_keepValue != _value) ? 1 : 0;
//     if ( dataControl == DATA_KEEP )
//         _keepValue = _value;
//     else if ( dataControl == DATA_RESTORE )
//         _value = _keepValue;
//     else if ( dataControl == DATA_SWAP ) {
//         float tmp  = _value;
//         _value     = _keepValue;
//         _keepValue = tmp;
//     }
//     return 0;
// }

// ValueFloat::ValueFloat( )
// {
//     _value = _keepValue = 0;
// }

// void ValueFloat::SetFloat(float value)
// {
//     _value = value;
// }

// void ValueFloatScientific::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%e", _value);
// }

// void ValueFloatScientific::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     float value;
//     int   numOf = sscanf(inP->_dst.AsStr( ), "%e", &value);

//     if ( numOf != 1 )
//         inP->SetStatus(RetStatus {E_RetStatus::Parsing});
//     else
//         _value = value;
// }

// bool ValueFloatScientific::IsWordInCliPartOfList( )
// {
//     return false;
// }

// ***************************************************************

// void ValueFloatNormal::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%f", _value);
// }

// void ValueFloatNormal::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     float value;
//     int   numOf = sscanf(inP->_dst.AsStr( ), "%f", &value);

//     if ( numOf != 1 )
//         inP->SetStatus(RetStatus {E_RetStatus::Parsing});
//     else
//         _value = value;
// }

// bool ValueFloatNormal::IsWordInCliPartOfList( )
// {
//     return false;
// }

// ***************************************************************
// ValueIpV6::ValueIpV6( ) : _value( ), _keepValue( )
// {
// }

// ValueIpV6::ValueIpV6(const IpV6Address& ipv6)
// {
//     SetIpV6(ipv6);
// }

// void ValueIpV6::SetIpV6(const IpV6Address& ipv6)
// {
//     _value = ipv6;
// }

// void ValueIpV6::SetIpV6(UINT8* bytes)
// {
//     SetIpV6(IpV6Address(bytes));
// }

// const IpV6Address& ValueIpV6::GetIpV6( ) const
// {
//     return _value;
// }

// const IpV6Address& ValueIpV6::GetOldIpV6( ) const
// {
//     return _keepValue;
// }

// void ValueIpV6::DoGetCliHelp( )
// {
//     ocli.Print("%sipv6", Separator( ));
// }

// void ValueIpV6::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%s", _value.str( ).c_str( ));
// }

// void ValueIpV6::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     std::optional<IpV6Address> res = String2IpV6Address(inP->_dst.AsStr( ));

//     if ( !res ) {
//         inP->SetStatus(RetStatus {E_RetStatus::Parsing});
//         return;
//     }

//     SetIpV6(res.value( ));
// }

// bool ValueIpV6::IsWordInCliPartOfList( )
// {
//     return false;
// }

// UINT32 ValueIpV6::DoDataControl(T_DataControl dataControl)
// {
//     if ( dataControl == IS_DATA_CHANGED )
//         return (_keepValue == _value) ? 0 : 1;
//     if ( dataControl == DATA_KEEP )
//         _keepValue = _value;
//     else if ( dataControl == DATA_RESTORE )
//         _value = _keepValue;
//     else if ( dataControl == DATA_SWAP ) {
//         IpV6Address tmp = _value;
//         _value          = _keepValue;
//         _keepValue      = tmp;
//     }
//     return 0;
// }

// ***************************************************************

// ValueIp::ValueIp( )
// {
//     SetIp(IpAddress( ));
// }

// IpAddress ValueIp::GetIp( ) const
// {
//     return IpAddress(GetUint32( ));
// }

// void ValueIp::SetIp(IpAddress aValue)
// {
//     SetUint32(aValue.AsUINT32( ));
// }

// IpAddress ValueIp::GetOldIp( )
// {
//     return IpAddress(GetOldUint32( ));
// }

// void ValueIp::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%s", GetIp( ).AsString( ).itsData);
// }

// void ValueIp::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     if ( IpAddress::IsValidDottedString(inP->_dst.AsStr( )) ) {
//         SetIp(IpAddress(inP->_dst.AsStr( )));
//     } else
//         inP->SetStatus(RetStatus {E_RetStatus::Parsing});
// }

// ***************************************************************

// bool ValueIpMask::IsValidMask( ) const
// {
//     // static UINT32 table[] = {0x80000000, 0xc0000000, 0xe0000000, 0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 0xfff00000, 0xfff80000, 0xfffc0000,
//     //     0xfffe0000, 0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 0xfffffff0, 0xfffffff8, 0xfffffffc,
//     //     0xfffffffe, 0xffffffff};
//     // for ( int i = 0; i < 32; i++ ) {
//     //     if ( GetUint32( ) == table[i] )
//     //         return true;
//     // }
//     return false;
// }

// void ValueIpAndMask::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     int   i;
//     char* sourceP = inP->_dst.AsStr( );
//     for ( i = 0; (sourceP[i] != 0) && (sourceP[i] != '/') && (i < 20); i++ )
//         ;
//     if ( sourceP[i] != '/' ) {
//         inP->SetStatus(E_RetStatus::Parsing);
//         return;
//     }

//     char ipBuf[20 + 1];
//     memcpy(ipBuf, sourceP, i);
//     ipBuf[i] = 0;

//     ValueIp   valueIp;
//     RetStatus status = valueIp.SetString(ipBuf);
//     if ( !status.ok( ) ) {
//         inP->SetStatus(status);
//         return;
//     }

//     i++;
//     ValueLong valueWidth;
//     status = valueWidth.SetString(sourceP + i);
//     if ( !status.ok( ) ) {
//         inP->SetStatus(status);
//         return;
//     }
//     if ( uint width = valueWidth.GetUint32( ); width == 0 ) {
//         _mask = 0;
//     } else if ( width < 32 ) {
//         _mask = std::numeric_limits<uint32_t>::max( ) << (32 - width);
//     } else {
//         inP->SetStatus(E_RetStatus::Range);
//         return;
//     }

//     SetIp(valueIp.GetIp( ));
// }

// void ValueIpAndMask::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%s/%d", GetIp( ).AsString( ).itsData, GetMaskAsPrefix( ));
// }

// UINT32 ValueIpAndMask::GetMask( ) const
// {
//     return _mask;
// }

// UINT32 ValueIpAndMask::GetMaskAsPrefix( ) const
// {
//     return IpAddress(_mask).Mask2PrefixSize( );
// }

// ***************************************************************

// void ValueIpAndUdpPort::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     int   i;
//     char* sourceP = inP->_dst.AsStr( );
//     for ( i = 0; (sourceP[i] != 0) && (sourceP[i] != ':') && (i < 20); i++ )
//         ;
//     if ( sourceP[i] != ':' ) {
//         inP->SetStatus(E_RetStatus::Parsing);
//         return;
//     }

//     char ipBuf[20 + 1];
//     memcpy(ipBuf, sourceP, i);
//     ipBuf[i] = 0;

//     ValueIp   valueIp;
//     RetStatus status = valueIp.SetString(ipBuf);
//     if ( !status.ok( ) ) {
//         inP->SetStatus(status);
//         return;
//     }

//     i++;
//     ValueLong valueUdpPort;
//     status = valueUdpPort.SetString(sourceP + i);
//     if ( !status.ok( ) ) {
//         inP->SetStatus(status);
//         return;
//     }

//     SetIp(valueIp.GetIp( ));
//     _udpPort = valueUdpPort.GetUint32( );
// }

// void ValueIpAndUdpPort::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%s:%d", GetIp( ).AsString( ).itsData, _udpPort);
// }

// ***************************************************************

// ValueMac::ValueMac(const Mac& mac)
// {
//     SetMac(mac);
// }

// void ValueMac::SetMac(const Mac& mac)
// {
//     _value = mac;
// }

// void ValueMac::SetMac(const UINT8* macBytes)
// {
//     Mac mac(macBytes);
//     SetMac(mac);
// }

// Mac ValueMac::GetMac( )
// {
//     return _value;
// }

// void ValueMac::DoGetCliHelp( )
// {
//     ocli.Print("%smac", Separator( ));
// }

// void ValueMac::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%s", _value.AsString( ).c_str( ));
// }

// void ValueMac::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;

//     if ( !inP->Read( ).IsOk( ) )
//         return;

//     std::optional<Mac> res = Mac::fromString(inP->_dst.AsStr( ));
//     if ( !res ) {
//         inP->SetStatus(RetStatus {E_RetStatus::Parsing});
//         return;
//     }

//     SetMac(res.value( ));
// }

// bool ValueMac::IsWordInCliPartOfList( )
// {
//     return false;
// }

// UINT32 ValueMac::DoDataControl(T_DataControl dataControl)
// {
//     if ( dataControl == IS_DATA_CHANGED )
//         return (_keepValue == _value) ? 0 : 1;
//     if ( dataControl == DATA_KEEP )
//         _keepValue = _value;
//     else if ( dataControl == DATA_RESTORE )
//         _value = _keepValue;
//     else if ( dataControl == DATA_SWAP ) {
//         Mac tmp    = _value;
//         _value     = _keepValue;
//         _keepValue = tmp;
//     }
//     return 0;
// }

// ***************************************************************

// ValueCounter64::ValueCounter64(UINT64 value) : _value(value)
// {
// }

// void ValueCounter64::AssignSignedPrintFormat(bool signedPrintFormat)
// {
//     _signedPrintFormat = signedPrintFormat;
// }

// void ValueCounter64::DoGetCli(MyOutStream* outP)
// {
//     if ( _signedPrintFormat )
//         outP->Print("%lld", _value);
//     else
//         outP->Print("%llu", _value);
// }

// void ValueCounter64::DoSetCli(WordReader* inP)
// {
//     (void)inP;
//     inP->SetStatus(E_RetStatus::Undefined);
// }

// bool ValueCounter64::IsWordInCliPartOfList( )
// {
//     return false;
// }

// void ValueCounter64::SetUint64(UINT64 value)
// {
//     _value = value;
// }

// ***************************************************************

// void ValueCounterFloat::DoGetCli(MyOutStream* outP)
// {
//     outP->Print("%f", _value);
// }

// void ValueCounterFloat::DoSetCli(WordReader* inP)
// {
//     (void)inP;
//     inP->SetStatus(E_RetStatus::Undefined);
// }

// bool ValueCounterFloat::IsWordInCliPartOfList( )
// {
//     return false;
// }

// void ValueCounterFloat::SetFloat(float value)
// {
//     _value = value;
// }

// ***************************************************************

void ValueString::DoGetCli(MyOutStream* outP)
{
    outP->Print("%s", GetP( ));
}

bool ValueString::StartsWith(const char* secondP)
{
    UINT32 secondLen = strlen(secondP);
    UINT32 mylen     = strlen(GetP( ));
    if ( secondLen > mylen )
        return false;

    bool isSubstring = (strncasecmp(GetP( ), secondP, secondLen) == 0);
    return isSubstring;
}

bool ValueString::EndsWith(const char* secondP)
{
    UINT32 secondLen = strlen(secondP);
    UINT32 mylen     = strlen(GetP( ));
    if ( secondLen > mylen )
        return false;

    bool isSubstring = (strncasecmp(GetP( ) + mylen - secondLen, secondP, secondLen) == 0);
    return isSubstring;
}

void ValueString::DoSetCli(WordReader* inP)
{
    if ( inP->HandleAutoComplete( ) )
        return;

    if ( !inP->Read( ).IsOk( ) )
        return;

    SetIntValue(inP->_dst.AsStr( ));

    if ( strlen(inP->_dst.AsStr( )) > GetMaxLen( ) )
        inP->SetStatus(E_RetStatus::Range);
    else
        inP->SetStatus(E_RetStatus::Ok);
}

bool ValueString::IsWordInCliPartOfList( )
{
    return false;
}

// ***************************************************************

// void ValueTimeSecs::UpdateBaseTime( )
// {
//     _baseTimeSecs = mygettime( );
// }

// void ValueTimeSecs::DoSetCli(WordReader* inP)
// {
//     (void)inP;
//     inP->SetStatus(E_RetStatus::Undefined);
// }

// void ValueTimeSecs::DoGetCli(MyOutStream* outP)
// {
//     UINT32 t = GetUint32( );

//     UINT32 days = t / (60 * 60 * 24);
//     t           = t % (60 * 60 * 24);

//     UINT32 hrs = t / (60 * 60);
//     t          = t % (60 * 60);

//     UINT32 min = t / 60;
//     t          = t % 60;

//     UINT32 sec = t;

//     outP->Print("%04d:%02d:%02d:%02d", days, hrs, min, sec);
// }

// bool ValueTimeSecs::IsWordInCliPartOfList( )
// {
//     return false;
// }

// ***************************************************************

void ValueChain::DoGetCli(MyOutStream* outP)
{
    for ( int i = 0; GetCurrentChainValueP(i) != nullptr; i++ ) {
        if ( i != 0 )
            *outP << " ";
        *outP << GetCurrentChainValueP(i);
    }
}

void ValueChain::DoSetCli(WordReader* inP)
{
    for ( int i = 0; GetCurrentChainValueP(i) != nullptr; i++ ) {
        Value* valP = GetCurrentChainValueP(i);
        valP->DoSetCli(inP);
        if ( inP->CheckExit( ) )
            return;
    }
}

bool ValueChain::IsWordInCliPartOfList( )
{
    return false;
}

UINT32 ValueChain::DoDataControl(T_DataControl dataControl)
{
    if ( dataControl == IS_DATA_CHANGED ) {
        for ( int i = 0; GetAllValueP(i) != nullptr; i++ ) {
            if ( GetAllValueP(i)->IsDataChanged( ) )
                return 1;
        }
    } else {
        for ( int i = 0; GetAllValueP(i) != nullptr; i++ )
            GetAllValueP(i)->DoDataControl(dataControl);
    }
    return 0;
}

// ***************************************************************

bool ValueUnion::DoGetUint32(UINT32* dstP)
{
    Value* curP = GetCurrentValueP(_valueIndex);
    if ( curP != nullptr )
        return curP->DoGetUint32(dstP);
    return false;
}

void ValueUnion::DoGetCliHelp( )
{
    for ( _valueIndex = 0; (GetCurrentValueP(_valueIndex) != nullptr); _valueIndex++ )
        GetCurrentValueP(_valueIndex)->DoGetCliHelp( );
}

void ValueUnion::DoGetCli(MyOutStream* outP)
{
    Value* curP = GetCurrentValueP(_valueIndex);
    if ( curP != nullptr )
        curP->DoGetCli(outP);
}

void ValueUnion::DoSetCli(WordReader* inP)
{
    if ( inP->NowAutoComplete( ) ) {
        DictAutoCompletion dict;
        DoGetCliAutoCompletion(&dict);

        inP->ReturnAutoComplete(&dict);
        return;
    }

    Value* curP;
    for ( UINT32 i = 0; (curP = GetCurrentValueP(i)) != nullptr; i++ ) {
        inP->SetStatus(E_RetStatus::Ok);

        UINT32 keepPos = inP->GetPos( );

        curP->DoSetCli(inP);
        if ( inP->Status( ).ok( ) ) {
            _valueIndex = i;
            return;
        }
        inP->SetPos(keepPos);
    }

    inP->SetStatus(E_RetStatus::Parsing);
}

bool ValueUnion::IsWordInCliPartOfList( )
{
    return false;
}

// bool ValueUnion::DoSetSnmp(SnmpVariable* varP)
// {
//     Value* curP;
//     for ( UINT32 i = 0; (curP = GetCurrentValueP(i)) != nullptr; i++ ) {
//         bool isOk = curP->DoSetSnmp(varP);
//         if ( isOk ) {
//             _valueIndex = i;
//             return true;
//         }
//     }
//     return false;
// }

UINT32 ValueUnion::DoDataControl(T_DataControl dataControl)
{
    if ( dataControl == IS_DATA_CHANGED ) {
        if ( _valueIndex != _keepValueIndex )
            return 1;
        Value* curP = GetCurrentValueP(_valueIndex);
        return (curP != nullptr) ? curP->DoDataControl(dataControl) : 0;
    }

    for ( int i = 0; GetCurrentValueP(i) != nullptr; i++ )
        GetCurrentValueP(i)->DoDataControl(dataControl);

    if ( dataControl == DATA_KEEP )
        _keepValueIndex = _valueIndex;
    else if ( dataControl == DATA_RESTORE )
        _valueIndex = _keepValueIndex;
    else if ( dataControl == DATA_SWAP ) {
        UINT32 tmp      = _valueIndex;
        _valueIndex     = _keepValueIndex;
        _keepValueIndex = tmp;
    }
    return 0;
}

// ***************************************************************

// ***************************************************************

// Value* ValueVlanIdOrNone::GetCurrentValueP(UINT8 valueIndex)
// {
//     switch ( valueIndex ) {
//         case 0: return &_valueNone;
//         case 1: return &_valueVlanId;
//         default: return nullptr;
//     };
// }

// ***************************************************************

// ValueDateAndTime::ValueDateAndTime( ) : _absTime(time(nullptr)), _printTimeOnly(false)
// {
// }

// ValueDateAndTime::ValueDateAndTime(time_t absTime) : _absTime(absTime), _printTimeOnly(false)
// {
// }

// void ValueDateAndTime::UpdateTime( )
// {
//     _absTime = time(nullptr);
// }

// void ValueDateAndTime::SetTime(time_t absTime)
// {
//     _absTime = absTime;
// }

// time_t ValueDateAndTime::GetTime( )
// {
//     return _absTime;
// }

// void ValueDateAndTime::DoSetCli(WordReader* inP)
// {
//     if ( inP->HandleAutoComplete( ) )
//         return;
//     MyStringT<64> buf;

//     if ( !inP->Read( ).IsOk( ) )
//         return;
//     buf << inP->_dst;
//     if ( !inP->Read( ).IsOk( ) )
//         return;
//     buf << inP->_dst;

//     struct tm struct_tm;
//     memset(&struct_tm, 0, sizeof(struct_tm));

//     char* res = strptime(buf.AsStr( ), "%Y.%m.%d %H:%M:%S", &struct_tm);

//     if ( res == nullptr ) {
//         inP->SetStatus(E_RetStatus::Parsing);
//         return;
//     }

//     _absTime = mktime(&struct_tm);
// }

// void ValueDateAndTime::DoGetCli(MyOutStream* outP)
// {
//     struct tm* ptm = localtime(&_absTime);

//     if ( !_printTimeOnly ) {
//         outP->Print("%04d.%02d.%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
//         outP->Print(" ");
//     }
//     outP->Print("%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
// }

// bool ValueDateAndTime::IsWordInCliPartOfList( )
// {
//     return false;
// }

// ***************************************************************

// void ValueLeg::DoSetCli(WordReader* inP)
// {
//     if ( inP->NowAutoComplete( ) ) {
//         DictAutoCompletion dict;

//         ValueEthernetPort valueEthernetPort;
//         dict.Add(valueEthernetPort.GetDictionary( ));

//         inP->ReturnAutoComplete(&dict);
//         return;
//     }

//     inP->SetStatus(inP->ReadListElement( ).GetStatus( ));
//     if ( !inP->Status( ).ok( ) )
//         return;

//     _leg = BridgeLeg::ReadFrom(inP->_dst.AsStr( ));
//     if ( !_leg.IsValid( ) )
//         inP->SetStatus(E_RetStatus::Parsing);
// }

// bool ValueLeg::IsWordInCliPartOfList( )
// {
//     return false;
// }

// UINT32 ValueLeg::DoDataControl(T_DataControl dataControl)
// {
//     if ( dataControl == IS_DATA_CHANGED )
//         return (_keepLeg == _leg) ? 0 : 1;
//     if ( dataControl == DATA_KEEP )
//         _keepLeg = _leg;
//     else if ( dataControl == DATA_RESTORE )
//         _leg = _keepLeg;
//     else if ( dataControl == DATA_SWAP ) {
//         BridgeLeg tmp = _leg;
//         _leg          = _keepLeg;
//         _keepLeg      = tmp;
//     }
//     return 0;
// }

// ***************************************************************
