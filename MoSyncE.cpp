#include "igorc_common.h"
#include "MoSyncE.h"
//#include "MoLicense.h"

//#include "siklu_fmt.h"

constexpr static bool localDebug = true;

/////////////////////////////////////////////////////////////
struct ValueSyncEPort : public ValueEthernetPort {
    bool DoIsMatch ( ) override  // this condition should be in sync with condition in CreateAll loop
    {
        LOG("entry");
        EthNum ethNum = GetEthNum( );
        if (ethNum == EthNum(0) || ethNum == EthNum(2))
            return true;
        return false;
    }
};

using ValueSyncEPortList = ValueListOfDictionaryT<ValueSyncEPort, /*MIN_NUM*/ 0, /*MAX_NUM*/ 31>;

/////////////////////////////////////////////////////////////

void MoSyncE::BuildScript(MyOutStream* dstP)
{
    LOG("entry");

    *dstP << "\n# configuring " << SampleMoT<MoSyncE>( )->TypeName( ) << "\n";

    ValueMo::for_each<MoSyncE>([dstP] (MoSyncE* moP) {
        *dstP << FOCUS(moP) << "set " << moP << " " << NAME_VAL(ATTR_ADMIN_STATE) << "\n";
    });
}

void MoSyncE::Create()
{
    LOG("entry ethNum");

    MoSyncE* moP = BuildNewT<MoSyncE>( );
    if ( moP == nullptr )
        return;

    moP->ethPort.SetEthNum(EthNum(0));
    moP->adminState.SetEnum(E_AdminState::Disable);
    Db( )->AddMo(moP);
    LOG("exit");
}

void MoSyncE::DownloadConfig( )
{
    LOG("entry");

    ValueMo valueMo(MO_SYNCE);
    for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
        MoSyncE* moP = (MoSyncE*)valueMo.MoP( );
    }

    //SkMessage::SkMessage msg {SkMessage::E_QueueId::MSG_NPU_Q};

    // ValueMo::for_each<MoSyncE>([&msg] (MoSyncE* moP) {
    //     if ( moP->Action( ) == ACTION_MODIFIED ) {
    //         EthNum       ethNum     = moP->ethPort.GetEthNum( );
    //         E_AdminState adminState = moP->adminState.GetEnum<E_AdminState>( );

    //         //fTRACE(localDebug, "ethNum:{} adminState:{}", ethNum, to_string(adminState));

    //         // TODO: send msg to NPU
    //     }
    // });
}

RetStatus MoSyncE::CheckConstraints( )
{
    LOG("entry");
    // EthNum       ethNum = ethPort.GetEthNum( );
    // E_AdminState state  = adminState.GetEnum<E_AdminState>( );

    // fTRACE(localDebug, "CONFIRM_CHANGE ethNum:{} adminState:{}", ethNum, to_string(state));

    // if ( state == E_AdminState::Enable ) {

    //     if ( !LicenseController::Self( )->GetActual(LICENSE_SYNCE) )
    //         return RetStatus {"SyncE cannot be enabled because SyncE license is disabled"};
    // }

    return RetStatus {E_RetStatus::Ok};
}

RetStatus MoSyncE::DoPerform(ActionType performType, void* dataP)
{
    LOG("entry performType:%s (%d)\n", to_string(performType), performType);
    // if ( performType != GET_MO_TYPE )
    //     fTRACE(true, "entry performType:{}", to_string(performType));

    switch ( performType ) {
        case GET_MO_NUMBER:
            // fTRACE(true, "GET_MO_NUMBER:{}", ethPort.GetUint32( ));
            *((MoId*)dataP) = MoId(1);
            break;
        case PRINT_FULL_NAME:
            *((MyOutStream*)dataP) << TypeName( );
            break;
        case GET_MO_TYPE:
            *((T_MoType*)dataP) = MO_SYNCE;
            break;
        case PRINT_TYPE_NAME:
            *((MyOutStream*)dataP) << "synce";
            break;
        case REGISTER_STATIC:
            RegisterAttr(ATTR_ETH_PORT, CLI_INFO_CONF, &ethPort, "source");
            RegisterAttr(ATTR_ADMIN_STATE, CLI_INFO_CONF, &adminState, "admin");
            break;
        case CONFIRM_CHANGE: return CheckConstraints( );
        default: break;
    }

    LOG("exit");
    return RetStatus {E_RetStatus::Ok};
}

// EH-8010FX-AES-L>show synce
// synce admin                     : down
// synce source                    : eth2|eth0
#if 0
void MoSyncE::DoHandleShow(ActionType cmdType)
{
    LOG("entry");
    CliReaderIdList<ValueSyncEPortList> id_ports(ATTR_ETH_PORT);
    CliModifierShow                     modifier(Type( ));

    if ( cmdType == ACTION_CLI_HELP )
        return Cli_PrintStandardHelpShow(&modifier);

    *cliP >> id_ports >> modifier >> END( );
    if ( cliP->CheckExit(true) )
        return;

    Cli_PrintStandardShow(&modifier, nullptr);
    LOG("exit");
}
#else
void MoSyncE::DoHandleShow(ActionType cmdType)
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
#endif

void MoSyncE::DoHandleSet(ActionType cmdType)
{
    LOG("entry");
    CliReaderId<ValueSyncEPort> id_port(ATTR_ETH_PORT);
    AttrListSet                    attrList(Type( ));
    attrList.AddSettable( );

    if ( cmdType == ACTION_CLI_HELP )
        return Cli_PrintStandardHelpSet(&attrList);

    *cliP >> id_port >> attrList >> END( );
    if ( cliP->CheckExit(true) )
        return;

    EthNum ethNum  = id_port._valueVal.GetEthNum( );
    UINT32 portVal = id_port._valueVal.GetUint32( );
    LOG("ethNum:%d portVal:%d", ethNum.id, portVal);

    MoSyncE* pmP = FindMoT<MoSyncE>(MoId(portVal));
    if ( pmP == nullptr ) {
        //cliP->ReturnErrorAndPrint(RetStatus {"SyncE MO object is not found for port '{}'. Operation failed.", ethNum});
        return;
    }

    std::ignore = CopySampleTo(pmP, &attrList);  // DoPerform(CONFIRM_CHANGE, nullptr) is called inside, so it's ok get error here
    LOG("exit");
}
