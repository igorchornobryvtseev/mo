
#pragma once

//#include "../authentication/CaAuthentication.h"
//#include "gl_types.h"
#include "Mo.h"
#include "MoType.h"
#include "Value.h"

enum class E_AuthMode {
    Wrong  = -1,
    Local  = 1,
    Radius = 2,
    Tacacs = 3,
};

constexpr const char* to_string (E_AuthMode e, const char* defValue = nullptr)
{
    switch ( e ) {
        case E_AuthMode::Wrong: return "E_AuthMode::Wrong";
        case E_AuthMode::Local: return "E_AuthMode::Local";
        case E_AuthMode::Radius: return "E_AuthMode::Radius";
        case E_AuthMode::Tacacs: return "E_AuthMode::Tacacs";
    }
    return defValue;
}

/////////////////////////////////////////////////////////////

DICTIONARY_3(AuthModeTable, E_AuthMode::Local, "local", E_AuthMode::Radius, "radius", E_AuthMode::Tacacs, "tacacs");
using ValueAuthMode = ValueDictT<AuthModeTable, static_cast<uint>(E_AuthMode::Local)>;

struct ValueAuthSecret : ValueStringUnquoted {
    // void DoSetCli(WordReader* inP) override;
    // bool IsWordInCliPartOfList ( ) override { return true; }
};

struct ValueAuthConnectTimeout : ValueLongRangeT<1, 10, 3> {
};

// DICTIONARY_4(UserType, E_UserType::User, "user", E_UserType::Tech, "tech", E_UserType::Super, "super", E_UserType::Admin, "admin");

// struct ValueAuthUserLevel : public ValueDictT<UserType, static_cast<int>(E_UserType::User)> {
// };

class MoAaa : public Mo
{
public:
    static const T_MoType MO_TYPE = MO_AAA;

    static void BuildScript(MyOutStream* dstP);
    static void Create( );

    static void DownloadConfig( );

    RetStatus DoPerform(ActionType performType, void* dataP) override;
    //	virtual void DoSnmpPerform( ActionType performType);

    void DoHandleShow(ActionType cmdType) override;
    void DoHandleSet(ActionType cmdType) override;

    std::string sharedSecret ( )
    {
        std::string str {_valueAuthSecret.GetP( )};
        if ( str == "none" )
            str.clear( );
        return str;
    }

private:
    ValueAuthMode           _valueAuthMode;
    ValueAuthSecret         _valueAuthSecret;
    ValueAuthConnectTimeout _valueConnectionTimeoutSec;
};
