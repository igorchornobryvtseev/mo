#include "igorc_common.h"
#include "MoPipeMode.h"
//#include "MoLldp.h"

static bool localDebug = false;

/////////////////////////////////////////////////////////////
struct ValuePipeModePort : public ValueEthernetPort {
    bool DoIsMatch ( ) override { return GetEthNum( ) == EthNum(1); }
};

struct ValuePipeModePortList : public ValueListOfDictionaryT<ValuePipeModePort, /*MIN_NUM*/ 0, /*MAX_NUM*/ 31> {
};

/////////////////////////////////////////////////////////////

// void MoPipeMode::BuildScript(MyOutStream* dstP)
// {
//     printf("entry\n");

//     *dstP << "\n# configuring " << SampleMoT<MoPipeMode>()->TypeName( ) << "\n";

//     ValueMo valueMo(MO_PIPE_MODE);
//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         Mo* moP = valueMo.MoP( );

//         // TODO: assert(moP)
//         if ( moP == nullptr ) {
//             printf("obj is NULL\n");
//             continue;
//         }

//         *dstP << FOCUS(moP) << "set " << moP << 
//         " " << NAME_VAL(ATTR_ADMIN_STATE) << "\n";
//     }
// }

void MoPipeMode::Create(EthNum ethNum)
{
    LOG("entry ethNum=%d\n", ethNum.id);

    MoPipeMode* moP = BuildNewT<MoPipeMode>( );
    if ( moP == nullptr )
        return;

    // moP->ethPort.SetEthNum(ethNum);
    // moP->adminState.SetEnum(E_AdminState::Disable);
    Db( )->AddMo(moP);
}

void MoPipeMode::CreateAll( )
{
    LOG("entry\n");

    // for ( auto iter = BeginEthNumPhy( ); iter->IsValid( ); iter++ ) {
    //     Create(*iter);
    // }

    EthNum eth0 = EthNum {static_cast<T_EthernetNum>(1)};
    EthNum eth1 = EthNum {static_cast<T_EthernetNum>(2)};
    EthNum eth2 = EthNum {static_cast<T_EthernetNum>(3)};
    Create(eth0);
    Create(eth1);
    Create(eth2);
}

void MoPipeMode::DownloadConfig( )
{
    LOG("entry\n");

    ValueMo valueMo(MO_PIPE_MODE);
    for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
        MoPipeMode* moP = (MoPipeMode*)valueMo.MoP( );

//         if ( moP->Action( ) == ACTION_MODIFIED ) {
//             EthNum       ethNum     = moP->ethPort.GetEthNum( );
//             E_AdminState adminState = moP->adminState.GetEnum<E_AdminState>( );

//             fTRACE(localDebug, "ethNum:{} adminState:{}", ethNum.AsName(), to_string(adminState));

// #if !defined (X86_TARGET)
//             SkMessage::SkMessage msg {SkMessage::E_QueueId::MSG_CAD_API_Q};
//             MsgSetPipeMode       data {.ethNum = ethNum, .adminState = adminState};

//             if ( msg.send(SkMessage::E_QueueId::MSG_NPU_Q, E_NPU_SET_PIPE_MODE, data) == E_Status::Error )
//                 fLOG_EX_ERR(true, "fail to send msg {} to queue {}", msg.msgName( ), msg.qName( ));
//             else
//                 fTRACE(localDebug, "successfully sent msg {} to queue {}", msg.msgName( ), msg.qName( ));
// #endif
//         }
    }
}

RetStatus MoPipeMode::CheckConstraints()
{
    // EthNum       ethNum = ethPort.GetEthNum( );
    // E_AdminState state  = adminState.GetEnum<E_AdminState>( );

    // fTRACE(localDebug, "CONFIRM_CHANGE ethNum:{} adminState:{}", ethNum.AsName(), to_string(state));

    // if (state == E_AdminState::Enable) {

    //     // LLDP constraint
    //     ValueMo valueMo(MO_LLDP);
    //     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
    //         MoLldp* lldpP = valueMo.MoP<MoLldp>( );

    //         // TODO: assert(lldpP)
    //         if ( lldpP == nullptr ) {
    //             fLOG_EX_ERR(true, "obj is NULL");
    //             continue;
    //         }

    //         if (lldpP->_valuePortNum == ethPort && lldpP->_valueAdminState.GetUint32() != T_LldpAdmin::E_LLDP_ADMIN_DISABLED) {
    //             return RetStatus {"Specified port cannot be enabled for pipe-mode because LLDP is enabled for this port. Operation failed."};
    //         }
    //     }
    // }

    return RetStatus {E_RetStatus::Ok};
}

RetStatus MoPipeMode::DoPerform(ActionType performType, void* dataP)
{
    LOG("entry performType:%s (%d)\n", to_string(performType), performType);

    // switch (performType) {
    // case GET_MO_NUMBER:
    //     //fTRACE(true, "GET_MO_NUMBER:{}", ethPort.GetUint32( ));
    //     *((MoId*)dataP) = MoId(ethPort.GetUint32( ));
    //     break;
    // case PRINT_FULL_NAME:
    //     *((MyOutStream*)dataP) << TypeName( ) << " " << ethPort;
    //     break;
    // case GET_MO_TYPE:
    //     *((T_MoType*)dataP) = MO_PIPE_MODE;
    //     break;
    // case PRINT_TYPE_NAME:
    //     *((MyOutStream*)dataP) << "pipe-mode";
    //     break;
    // case REGISTER_STATIC:
    //     RegisterAttr(ATTR_ETH_PORT, NO_CLI, &ethPort, "eth");
    //     RegisterAttr(ATTR_ADMIN_STATE, CLI_INFO_CONF, &adminState, "admin");
    //     break;
    // case CONFIRM_CHANGE:
    //     return CheckConstraints();
    // default:
    //     break;
    // }

    return RetStatus {E_RetStatus::Ok};
}

void MoPipeMode::DoHandleShow(ActionType cmdType)
{
    LOG("entry\n");

    CliReaderIdList<ValuePipeModePortList> id_ports(ATTR_ETH_PORT);
    CliModifierShow modifier(Type( ));

    if ( cmdType == ACTION_CLI_HELP )
        return Cli_PrintStandardHelpShow(&modifier);

    *cliP >> id_ports >> modifier >> END( );
    if ( cliP->CheckExit(true) )
        return;

    Cli_PrintStandardShow(&modifier, nullptr);
}

void MoPipeMode::DoHandleSet(ActionType cmdType)
{
    LOG("entry\n");

    CliReaderId<ValuePipeModePort> id_port(ATTR_ETH_PORT);
    AttrListSet attrList(Type( ));
    attrList.AddSettable( );

    if ( cmdType == ACTION_CLI_HELP )
        return Cli_PrintStandardHelpSet(&attrList);

    *cliP >> id_port >> attrList >> END( );
    if ( cliP->CheckExit(true) )
        return;

    EthNum ethNum  = id_port._valueVal.GetEthNum();
    UINT32 portVal = id_port._valueVal.GetUint32( );
    LOG("ethNum:%d portVal:%d\n", ethNum.id, portVal);

    MoPipeMode* pmP = FindMoT<MoPipeMode>(MoId(portVal));
    if ( pmP == nullptr ) {
        //cliP->ReturnErrorAndPrint(RetStatus {"Pipe-mode MO object is not found for port '{}'. Operation failed.", ethNum.AsName()});
        return;
    }

    std::ignore = CopySampleTo(pmP, &attrList); // DoPerform(CONFIRM_CHANGE, nullptr) is called inside, so it's ok get error here
}
