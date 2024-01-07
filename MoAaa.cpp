#include "igorc_common.h"
#include "MoAaa.h"

////PAM_Info_S* PamInfo( );

void MoAaa::DownloadConfig( )
{
    LOG("entry");
    MoAaa* moP = (MoAaa*)Mo::FindMo(MO_AAA);
    if ( !moP )
        return;

    // LOG:
    // moP->_valueAuthSecret
    // moP->AttrUint32(ATTR_AAA_MODE)
    // moP->AttrUint32(ATTR_TIMEOUT);

    if ( moP->AttrWasChanged(ATTR_AAA_SECRET) ) {
        // moP->_valueAuthSecret.ToString(PamInfo( )->shared_secret, MAX_SHARED_SECRET - 1);
        // fLOG_EX_DEBUG(false, "secret {}", PamInfo( )->shared_secret);
    }

    if ( moP->AttrWasChanged(ATTR_AAA_MODE) ) {
        // PamInfo( )->auth_PAMMethod              = (E_AuthMode)moP->AttrUint32(ATTR_AAA_MODE);
        // fLOG_EX_DEBUG(false, "Auth Method: new method {}", to_string(PamInfo( )->auth_PAMMethod));
    }

    if ( moP->AttrWasChanged(ATTR_TIMEOUT) ) {
        // PamInfo( )->connection_timeout          = moP->AttrUint32(ATTR_TIMEOUT);
        // fLOG_EX_DEBUG(false, "AAA connection timeout: {}", PamInfo( )->connection_timeout);
    }
    LOG("exit");
}

void MoAaa::BuildScript(MyOutStream* dstP)
{
    LOG("entry");
    *dstP << "\n";
    dstP->Print("# %s configuring", SampleMo(MO_AAA)->TypeName( ));
    *dstP << "\n";

    Mo* moP = Mo::FindMo(MO_AAA);

    *dstP << FOCUS(moP) << "set " << moP << " " << NAME_VAL(ATTR_AAA_MODE) << " " << NAME_VAL(ATTR_AAA_SECRET) << " " << NAME_VAL(ATTR_TIMEOUT) << "\n";
    LOG("exit");
}

void MoAaa::Create( )
{
    LOG("entry");
    MoAaa* aaaP = (MoAaa*)BuildNew(MO_AAA);

    aaaP->_valueAuthSecret.SetString("none");
    aaaP->_valueConnectionTimeoutSec.SetUint32(5);

    Db( )->AddMo(aaaP);
    LOG("exit");
}

RetStatus MoAaa::DoPerform(ActionType performType, void* dataP)
{
    LOG("entry performType:%s (%d)\n", to_string(performType), performType);

    if ( performType == GET_MO_NUMBER )
        *((MoId*)dataP) = MoId(1);
    else if ( performType == GET_MO_TYPE )
        *((T_MoType*)dataP) = MO_AAA;
    else if ( performType == PRINT_TYPE_NAME )
        *((MyOutStream*)dataP) << "aaa";
    else if ( performType == PRINT_FULL_NAME )
        *((MyOutStream*)dataP) << "aaa";
    else if ( performType == REGISTER_STATIC ) {
        RegisterAttr(ATTR_AAA_MODE, CLI_INFO_CONF, &_valueAuthMode, "mode");
        RegisterAttr(ATTR_AAA_SECRET, CLI_INFO_CONF, &_valueAuthSecret, "shared-secret");
        RegisterAttr(ATTR_TIMEOUT, CLI_INFO_CONF, &_valueConnectionTimeoutSec, "connection-timeout");
    }
    LOG("exit");
    return E_RetStatus::Ok;
}

/////////////////////////////////////////////////////////////

#include "CliMo.h"

void MoAaa::DoHandleShow(ActionType cmdType)
{
    LOG("entry");
    CliModifierShow modifier(Type( ));

    if ( cmdType == ACTION_CLI_HELP )
        return Cli_PrintStandardHelpShow(&modifier);

    *cliP >> modifier >> END( );
    if ( cliP->CheckExit(true) )
        return;

    Cli_PrintStandardShow(&modifier, nullptr);
    LOG("exit");
}

void MoAaa::DoHandleSet(ActionType cmdType)
{
    LOG("entry");
    AttrListSet attrList(Type( ));
    attrList.AddSettable( );

    if ( cmdType == ACTION_CLI_HELP )
        return Cli_PrintStandardHelpSet(&attrList);

    *cliP >> attrList >> END( );
    if ( cliP->CheckExit(true) )
        return;

    Mo* moP = FindMo(Type( ));
    if ( moP == nullptr )
        cliP->ReturnErrorAndPrint(RetStatus {"Specified object doesn't exist. Operation failed."});
    CopySampleTo(moP, &attrList);
    LOG("exit");
}
