#include "igorc_common.h"
// #include "MoEthernet.h"

// #include <stdio.h>

// #include "CaExternal.h"
// #include "CliMo.h"
// #include "Database.h"
// #include "event.h"
// #include "gl_types.h"
// #include "Mo.h"
// #include "MoBridge.h"
// #include "MoClassifierCos.h"
// #include "MoClassifierEvc.h"
// #include "MoInventory.h"
// #include "MoIp.h"
// #include "MoLag.h"
// #include "MoLicense.h"
// #include "MoLinkBackup.h"
// #include "MoRf.h"
// #include "MoSystem.h"
// #include "MyOid.h"
// #include "MyString.h"
// #include "rf.h"
// #include "siklu_fmt.h"
// #include "siklunet.h"

// #define ATTR_LOCAL_ACTIVE_CONFIG                           (ATTR_START_LOCAL + 1)
// #define ATTR_LOCAL_ACTIVE_ACTUAL                           (ATTR_START_LOCAL + 2)
// #define ATTR_LOCAL_AUTO_NEG_ADMIN                          (ATTR_START_LOCAL + 3)
// #define ATTR_LOCAL_ETHER_TYPE                              (ATTR_START_LOCAL + 4)
// #define ATTR_LOCAL_ETHER_TYPE_ACTUAL                       (ATTR_START_LOCAL + 5)
// #define ATTR_LOCAL_CONNECTOR_TYPE                          (ATTR_START_LOCAL + 6)
// #define ATTR_LOCAL_LOOP_MAC                                (ATTR_START_LOCAL + 7)
// #define ATTR_LOCAL_LOOP_MODE                               (ATTR_START_LOCAL + 8)
// #define ATTR_BLOCKED_BY_EXTEND_MM_STATE                    ((T_AttrType)(ATTR_START_LOCAL + 9))
// #define ATTR_LOCAL_SFP_DDM_TEMPERATURE                     (ATTR_START_LOCAL + 10)
// #define ATTR_LOCAL_SFP_DDM_VCC                             (ATTR_START_LOCAL + 11)
// #define ATTR_LOCAL_SFP_DDM_TX_BIAS                         (ATTR_START_LOCAL + 12)
// #define ATTR_LOCAL_SFP_DDM_TX_POWER                        (ATTR_START_LOCAL + 13)
// #define ATTR_LOCAL_SFP_DDM_RX_POWER                        (ATTR_START_LOCAL + 14)
// #define ATTR_LOCAL_SFP_DDM_TEMP_STATUS                     (ATTR_START_LOCAL + 15)
// #define ATTR_LOCAL_SFP_DDM_VCC_STATUS                      (ATTR_START_LOCAL + 16)
// #define ATTR_LOCAL_SFP_DDM_TX_BIAS_STATUS                  (ATTR_START_LOCAL + 17)
// #define ATTR_LOCAL_SFP_DDM_TX_POWER_STATUS                 (ATTR_START_LOCAL + 18)
// #define ATTR_LOCAL_SFP_DDM_RX_POWER_STATUS                 (ATTR_START_LOCAL + 19)

// #define SIKLU_JUMBO_PACKET_MAX_SIZE_ON_ALL_NETW_INTERFACES 1900  // search this define in all projects

// static bool HasDoublePort ( )
// {
//     for ( auto it = BeginEthNumAnyExists( ); it.IsValid( ); it++ ) {
//         if ( Eth( )->IsDoublePort(*it) )
//             return true;
//     }
//     return false;
// }

// ////////////////////////////////////////////////////

// void ValueEtherTypeConfigDualMedia::DoSetCli(WordReader* inP)
// {
//     DICTIONARY_2(TableSelect, 1, "rj45", 2, "sfp");
//     ValueDictT<TableSelect> valueSelect;

//     valueSelect.DoSetCli(inP);
//     if ( inP->CheckExit( ) )
//         return;

//     if ( valueSelect == "rj45" )
//         _valueCopper.DoSetCli(inP);
//     else
//         _valueSfp.DoSetCli(inP);
//     if ( inP->CheckExit( ) )
//         return;

//     if ( !inP->TestNext(",") )
//         return;
//     inP->Read( );  // skip ','

//     ValueDictInternalT<2> valueSelect_1;
//     valueSelect_1._dictionary.Add(1, (char*)((valueSelect == "rj45") ? "sfp" : "rj45"));
//     valueSelect_1.DoSetCli(inP);
//     if ( inP->CheckExit( ) )
//         return;

//     if ( valueSelect == "rj45" )
//         _valueSfp.DoSetCli(inP);
//     else
//         _valueCopper.DoSetCli(inP);
//     if ( inP->CheckExit( ) )
//         return;
// }

// UINT32 ValueEtherTypeConfigDualMedia::DoDataControl(T_DataControl dataControl)
// {
//     if ( dataControl == IS_DATA_CHANGED ) {
//         return (_valueCopper.IsDataChanged( ) || _valueSfp.IsDataChanged( )) ? 1 : 0;
//     }
//     if ( dataControl == DATA_KEEP ) {
//         _valueCopper.DoDataControl(DATA_KEEP);
//         _valueSfp.DoDataControl(DATA_KEEP);
//     } else if ( dataControl == DATA_RESTORE ) {
//         _valueCopper.DoDataControl(DATA_RESTORE);
//         _valueSfp.DoDataControl(DATA_RESTORE);
//     } else if ( dataControl == DATA_SWAP ) {
//         _valueCopper.DoDataControl(DATA_SWAP);
//         _valueSfp.DoDataControl(DATA_SWAP);
//     }
//     return 0;
// }

// static T_MO_ETHERNET_CONNECTOR_TYPE CalcConnector (EthNum ethNum)
// {
//     if ( Eth( )->IsDoublePort(ethNum) )
//         return E_ETHER_CONNECTOR_DUAL_MEDIA;

//     return Bsp( )->FlashPortType(ethNum);
// }

// void MoEthernet::ValueEtherTypeConfig::CopyFrom(MoEthernet* srcP)
// {
//     if ( srcP == nullptr ) {
//         return;
//     }

//     AssignConnector(CalcConnector(srcP->GetEthNum( )));

//     if ( _isDoubleMua ) {
//         _valueDualMedia._valueCopper.SetUint32(srcP->_valueEtherTypeConfig._valueDualMedia._valueCopper.GetUint32( ));
//         _valueDualMedia._valueSfp.SetUint32(srcP->_valueEtherTypeConfig._valueDualMedia._valueSfp.GetUint32( ));
//     } else {
//         ((ValueDictionary*)_valueP)->SetUint32(((ValueDictionary*)srcP->_valueEtherTypeConfig._valueP)->GetUint32( ));
//     }
// }

// ////////////////////////////////////////////////////

// /*
//  * Follow code used on 10G NXP Platform
//  *  NXP platform supports only internal loopback with MAC swap on eth2
//  */
// static void ApiConfigLoopNXP ( )
// {
//     ValueMo valueMo(MO_ETHERNET);

//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         MoEthernet* ethP = (MoEthernet*)valueMo.MoP( );
//         if ( ethP == nullptr ) {
//             continue;
//         }

//         EthNum ethNum = ethP->_valuePortNum.GetEthNum( );

//         T_EthLoop oldLoop = (T_EthLoop)ethP->_valueLoop.GetOldUint32( );
//         T_EthLoop loop    = (T_EthLoop)ethP->_valueLoop.GetUint32( );

//         if ( oldLoop == loop )
//             continue;  // do nothing

//         // NXP supports only 1 loopback type: internal mac swap
//         switch ( loop ) {
//             case E_ETH_LOOP_INTERNAL_MAC_SWAP: Eth( )->SetHwLoopback(ethNum, E_ETH_LOOP_INTERNAL_MAC_SWAP, ethP->_valueLoopMac.GetMac( )); break;
//             case E_ETH_LOOP_DISABLED:
//             default: Eth( )->SetHwLoopback(ethNum, E_ETH_LOOP_DISABLED); break;
//         }
//     }
// }

// void MoEthernet::DownloadConfigNxp( )
// {
//     static bool isFirst = true;

//     ApiConfigLoopNXP( );

//     ValueMo iterEth(MO_ETHERNET);
//     for ( iterEth.SetFirst( ); iterEth.IsValid( ); iterEth.SetNext( ) ) {
//         MoEthernet* ethP = (MoEthernet*)iterEth.MoP( );
//         if ( ethP == nullptr ) {
//             continue;
//         }

//         EthNum ethNum         = ethP->_valuePortNum.GetEthNum( );
//         UINT32 ethLoop        = ethP->_valueLoop.GetUint32( );
//         bool   isInternalLoop = (ethLoop == E_ETH_LOOP_INTERNAL) || (ethLoop == E_ETH_LOOP_INTERNAL_MAC_SWAP);

//         if ( (ethP->_valueAutoNegAdmin.IsDataChanged( ) || ethP->_valueEtherTypeConfig.IsDataChanged( ) || ethP->AttrWasChanged({ATTR_ADMIN_STATE, ATTR_CLOCK_MODE})) && !isInternalLoop ) {
//             E_AdminState admin_enabled = static_cast<E_AdminState>(ethP->AttrUint32(ATTR_ADMIN_STATE));
//             Eth( )->SetAdmin(ethNum, admin_enabled);
//             if ( admin_enabled == E_AdminState::Enable ) {
//                 Eth( )->SetEthernetWorkConfig(ethNum, 0, admin_enabled, ethP->_valueAutoNegAdmin.GetEnum<E_AutoNegState>( ), ethP->_valueEtherTypeConfig.GetType(0),
//                     static_cast<T_ClockMode>(ethP->AttrUint32(ATTR_CLOCK_MODE)));  // @todo use AttrEnum

//                 if ( ethP->IsDoubleMauPort( ) ) {
//                     Eth( )->SetEthernetWorkConfig(ethNum, 0, admin_enabled, ethP->_valueAutoNegAdmin.GetEnum<E_AutoNegState>( ), ethP->_valueEtherTypeConfig.GetType(1),
//                         static_cast<T_ClockMode>(ethP->AttrUint32(ATTR_CLOCK_MODE)));  //@todo redo to AttrEnum
//                 }
//             }
//         }

//         if ( ethP->IsDoubleMauPort( ) ) {
//             if ( isFirst || ethP->_valueActiveConfig.IsDataChanged( ) )
//                 Eth( )->ConfigDoublePort(ethNum, (EthActiveConfig)ethP->_valueActiveConfig.GetUint32( ));
//         }

//         if ( ethNum == EthNum::Phy(2) ) {
//             if ( ethP->_valueAlarmPropagation_eh.IsDataChanged( ) || ethP->_valueAdminState.IsDataChanged( ) ) {
//                 E_AdminState admin_enabled = static_cast<E_AdminState>(ethP->AttrUint32(ATTR_ADMIN_STATE));                                        // @todo replace with AttrEnum
//                 Eth( )->SetAlarmPropagation(ethNum, admin_enabled, static_cast<E_AlarmPropagation>(ethP->AttrUint32(ATTR_ALARM_PROPAGATION_EH)));  // @todo replace with AttrEnum
//             }
//         }

//         if ( ethNum.IsRf( ) || ethNum == EthNum::Phy(2) ) {  // @todo replace with iterator over 10G ports
//             if ( ethP->_valuePauseFrames.IsDataChanged( ) ) {
//                 E_PauseFrames newValue = static_cast<E_PauseFrames>(ethP->AttrUint32(ATTR_LOCAL_PAUSE_FRAMES));  //@todo replace with AttrEnum
//                 fLOG_EX_DEBUG(false, "Pause Frames value changed to {} for {}", to_string(newValue), ethNum);
//                 Eth( )->SetPauseFrames(ethNum, newValue);
//             }
//         }
//     }

//     isFirst = false;
// }

// void MoEthernet::DownloadConfigAdmin( )
// {
// }

// void MoEthernet::ConfigAlarmPropagation( )
// {
//     ValueMo::for_each<MoEthernet>([] (MoEthernet* moP) {
//         T_MoActionStatus action_status = moP->Action( );
//         // In case of a new / modified Etherent port
//         switch ( action_status ) {
//             case T_MoActionStatus::ACTION_CREATED:
//             case T_MoActionStatus::ACTION_MODIFIED:
//                 if ( moP->AttrWasChanged({ATTR_ALARM_PROPAGATION_EH, ATTR_ADMIN_STATE, ATTR_REMOTE_ETH_PORT}) ) {
//                     const EthNum                    ethNum = moP->GetEthNum( );
//                     const T_ALM_PROP_ETH_CONF_PARAM eth_port_conf {
//                         .eth_port_num           = ethNum,
//                         .rem_eth_port_num       = moP->_valueRemEthPort.GetEthNum( ),
//                         .eth_port_type          = moP->_valueEtherTypeConfig.GetType((moP->IsDoubleMauPort( ) && Eth( )->GetActualDoublePort(ethNum) == ETH_ACTIVE_SFP) ? 1 : 0),
//                         .eth_port_admin_state   = static_cast<E_AdminState>(moP->AttrUint32(ATTR_ADMIN_STATE)),  // @todo use AttrEnum
//                         .eth_port_alm_prop_mode = static_cast<E_AlarmPropagation>(moP->AttrUint32(ATTR_ALARM_PROPAGATION_EH)),
//                     };

//                     constexpr SkMessage::E_QueueId queueId = SkMessage::E_QueueId::MSG_ALM_PROP_Q;
//                     constexpr int                  msgId   = E_ALM_PROP_ETH_CONF_MSG;

//                     SkMessage::SkMessage msg(SkMessage::E_QueueId::MSG_ALM_PROP_API_Q);

//                     if ( msg.send(queueId, msgId, eth_port_conf) == E_Status::Error )
//                         fLOG_EX_ERR(false, "fail to send message {} to queue {} ", msg.msgName( ), msg.qName( ));
//                 }
//                 break;
//             case T_MoActionStatus::ACTION_DELETED: fLOG_EX_ERR(false, "Delete of Ethernet MO - Not supported"); break;
//             default: break;
//         }
//     });
// }

// ////////////////////////////////////////////////////

// void MoEthernet::SendEthernetEvents( )
// {
//     static UINT32 first_time = 1;

//     ValueMo valueMo(MO_ETHERNET);
//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         MoEthernet* ethP = (MoEthernet*)valueMo.MoP( );
//         if ( ethP == nullptr ) {
//             continue;
//         }

//         EthNum ethNum = ethP->_valuePortNum.GetEthNum( );

//         if ( ethP->AttrWasChanged(ATTR_DISABLE_ALARM) ) {
//             UINT32            masked = (ethP->AttrUint32(ATTR_DISABLE_ALARM) == E_AGENT_TRUE);
//             T_ETH_ALRAM_EVENT event  = masked ? E_ETH_ALARM_EVENT_MASK_ON : E_ETH_ALARM_EVENT_MASK_OFF;
//             SendEventLinkStateEthernet(ethNum, event);
//         }
//         // if admin state has changed, start or stop the timer
//         if ( (first_time == 1) || ethP->AttrWasChanged(ATTR_ADMIN_STATE) ) {
//             bool              enabled = static_cast<E_AdminState>(ethP->AttrUint32(ATTR_ADMIN_STATE)) != E_AdminState::Disable;  //@todo use AttrEnum
//             T_ETH_ALRAM_EVENT event   = enabled ? E_ETH_ALARM_EVENT_ADMIN_UP : E_ETH_ALARM_EVENT_ADMIN_DOWN;
//             SendEventLinkStateEthernet(ethNum, event);
//         }
//     }
//     first_time = 0;
// }

////////////////////////////////////////////////////

Dictionary::Entry DictionaryEthernet::___GetEntry(UINT32 context)
{
    EthNum ethNum {(T_EthernetNum)context};

    if ( ethNum.IsValid( ) ) {
        Entry entry(context, ethNum.AsName( ));
        // if ( ethNum.IsPhy( ) ) {
        //     if ( _controls & USE_LAG ) {
        //         MoEthernet* ethP = FindEthernet(ethNum);
        //         if ( (ethP != nullptr) && ethP->_valueLagRef.IsLagMaster( ) )
        //             entry._nameP = ethP->_valueLagRef.LagName( );
        //     }
        // }
        return entry;
    }
    if ( ethNum.IsNone( ) ) {
        if ( _controls & USE_NONE )
            return Entry(context, "none");
        return Entry(context, "???");
    }
    return Entry( );
}

void DictionaryEthernet::___FirstEntry(UINT32* contextP)
{
    *contextP = 0 ;//EthNum::Host( ).AsEnum( );
    if ( !IsMatch(EthNum((T_EthernetNum)*contextP)) )
        ___NextEntry(contextP);
}

void DictionaryEthernet::___NextEntry(UINT32* contextP)
{
    if ( *contextP > EthNum::MaxEthNumPossible( ) )
        return;
    for ( (*contextP)++; (*contextP <= EthNum::MaxEthNumPossible( )) && (!IsMatch(EthNum((T_EthernetNum)*contextP))); (*contextP)++ ) { }
}

bool DictionaryEthernet::IsMatch(EthNum ethNum)
{
    // if ( ethNum.IsNone( ) )
    //     return (_controls & USE_NONE) != 0;
    // if ( ethNum.IsHost( ) )
    //     return (_controls & SKIP_HOST) == 0;
    // if ( ethNum.IsRf( ) )
    //     return (_controls & SKIP_ETH0) == 0;
    // if ( ethNum.IsPhy( ) ) {
    //     if ( _controls & USE_LAG ) {
    //         MoEthernet* ethP = FindEthernet(EthNum(ethNum));
    //         if ( (ethP != nullptr) && ethP->_valueLagRef.IsLagSlave( ) )
    //             return false;
    //     }
    //     if ( _controls & SKIP_LAG ) {
    //         MoEthernet* ethP = FindEthernet(EthNum(ethNum));
    //         if ( (ethP != nullptr) && ethP->IsLag( ) )
    //             return false;
    //     }
    //     return true;
    // }

    if (ethNum.IsValid()) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////
/*
MoEthernet::ValueAlarmPropagation::ValueAlarmPropagation()
{
    SetUint32( E_ALARM_PROPAGATION_DISABLED);
}

Dictionary * MoEthernet::ValueAlarmPropagation::GetDictionary()
{
    static DictionaryT<5>  dict;
    if( dict.IsEmpty())
    {
        dict.Add( E_ALARM_PROPAGATION_DISABLED, "disabled");
        dict.Add( E_ALARM_PROPAGATION_BACKWARD, "backward");

        dict.Add( E_ALARM_PROPAGATION_FORWARD, "forward");
        dict.Add( E_ALARM_PROPAGATION_BOTH_DIRECT, "both-directions");

    }
    return & dict;
}
*/
////////////////////////////////////////////////////

// void MoEthernet::BuildScript(MyOutStream* dstP)
// {
//     *dstP << "\n# configuring " << SampleMo(MO_ETHERNET)->TypeName( ) << "\n";

//     ValueMo valueMo(MO_ETHERNET);
//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         MoEthernet* moP = (MoEthernet*)valueMo.MoP( );
//         if ( moP == nullptr ) {
//             continue;
//         }

//         if ( moP->IsRfGuest( ) )
//             continue;

//         *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_ADMIN_STATE) << "\n";
//         if ( strlen(moP->_valueAlias.GetP( )) != 0 )
//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_ALIAS) << " "
//                   << "\n";

//         if ( (!moP->_valuePortNum.GetEthNum( ).IsHost( )) && (!moP->_valuePortNum.GetEthNum( ).IsRf( )) ) {
//             if ( moP->IsDoubleMauPort( ) )
//                 *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_LOCAL_ACTIVE_CONFIG) << "\n";

//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_LOCAL_ETHER_TYPE) << "\n";

//             if ( /* moP->FindAttr(ATTR_ALARM_PROPAGATION)->IsSettable() || */ moP->FindAttr(ATTR_REMOTE_ETH_PORT)->IsSettable( ) ) {
//                 *dstP << FOCUS(moP) << "set " << moP << "  " /* << NAME_VAL_FILTERED(ATTR_ALARM_PROPAGATION) << " " */
//                       << NAME_VAL_FILTERED(ATTR_REMOTE_ETH_PORT) << "\n";
//             }
//         }
//         if ( !moP->IsRfWigig( ) )
//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_LOCAL_AUTO_NEG_ADMIN) << "\n";

//         if ( moP->FindAttr(ATTR_ALARM_PROPAGATION_EH)->IsSettable( ) ) {
//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_ALARM_PROPAGATION_EH) << "\n";
//         }

//         if ( (!moP->_valuePortNum.GetEthNum( ).IsHost( )) ) {
//             if ( moP->IsDoubleMauPort( ) || moP->_valuePortNum.GetEthNum( ).IsRf( ) ) {
//                 if ( moP->FindAttr(ATTR_LOCAL_PAUSE_FRAMES)->IsSettable( ) ) {
//                     *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL(ATTR_LOCAL_PAUSE_FRAMES) << "\n";
//                 }
//             }
//         }

//         if ( moP->FindAttr(ATTR_PCP_WRITE_PROFILE_ID)->IsSettable( ) || moP->FindAttr(ATTR_PFC_MODE)->IsSettable( ) ) {
//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL_FILTERED(ATTR_PCP_WRITE_PROFILE_ID) << " " << NAME_VAL_FILTERED(ATTR_PFC_MODE) << "\n";
//         }

//         if ( moP->FindAttr(ATTR_CLOCK_MODE)->IsSettable( ) || moP->FindAttr(ATTR_DISABLE_ALARM)->IsSettable( ) ) {
//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL_FILTERED(ATTR_CLOCK_MODE) << " " << NAME_VAL_FILTERED(ATTR_DISABLE_ALARM) << "\n";
//         }

//         if ( moP->FindAttr(ATTR_ETH_MIRRORING_TARGET)->IsSettable( ) || moP->FindAttr(ATTR_ETH_MIRRORING_MODE)->IsSettable( ) ) {
//             *dstP << FOCUS(moP) << "set " << moP << "  " << NAME_VAL_FILTERED(ATTR_ETH_MIRRORING_TARGET) << " " << NAME_VAL_FILTERED(ATTR_ETH_MIRRORING_MODE) << "\n";
//         }
//     }
// }

// ////////////////////////////////////////////////////

// bool MoEthernet::IsRfWigig( )  // wigig dynamic or static
// {
//     return GetEthNum( ).IsRf( );
// }

// bool MoEthernet::IsRfGuest( )
// {
//     return false;
// }

MoEthernet* FindEthernet (EthNum ethNum)
{
    return (MoEthernet*)Mo::FindMo(MO_ETHERNET, MoId(ethNum.AsEnum( )));
}

// MoEthernet::MoEthernet( ) : _valueBlockedByExtendMMState(this), _loopTimer(this)
// {
// }

// Mac MoEthernet::GetHostSelfMac( )
// {
//     MoEthernet* hostP = FindEthernet(EthNum::Host( ));
//     if ( hostP == nullptr )
//         return Mac { };

//     hostP->UpdateDynamic( );
//     return hostP->_valueLocalMac.GetMac( );
// }

// MoEthernet* MoEthernet::CreateRf(UINT32 /*lineNum*/)
// {
//     EthNum ethNum = EthNum::Rf(1);
//     return MoEthernet::Create(ethNum);
// }

// MoEthernet* MoEthernet::Create(EthNum ethNum)
// {
//     fTRACE(false, "creating ethernet {}", ethNum);
//     MoEthernet* ethP = BuildNewT<MoEthernet>( );
//     if ( ethP == nullptr )
//         return nullptr;

//     ethP->_valuePortNum.SetEthNum(ethNum);

//     T_MO_ETHERNET_CONNECTOR_TYPE connector = CalcConnector(ethNum);
//     ethP->_valueEtherTypeConfig.AssignConnector(connector);
//     ethP->_valueEtherConnectorType.SetUint32(connector);

//     ethP->_valueName.SetString(ethNum.AsName( ));
//     ethP->_valueName.GetStringP( )->ToUpper(0);

//     ethP->_valueDescription.SetString(ethNum.AsName( ));
//     ethP->_valueDescription.GetStringP( )->ToUpper(0);
//     if ( ethNum.IsPhy( ) )
//         ethP->_valueDescription.GetStringP( )->InsertSubstr(3, " ");
//     else if ( ethNum.IsRf( ) ) {
//         ethP->_valueDescription.GetStringP( )->SubstituteSubstr("-", " ");
//     }

//     ethP->_valuePromiscMode._mode         = ethNum.IsHost( ) ? 2 : 1;
//     ethP->_valueConnectorPresent._present = (ethNum.IsHost( ) || ethNum.IsRf( )) ? 2 : 1;
//     ethP->_valueIfType._ifType            = ethNum.IsHost( ) ? 1 : (ethNum.IsRf( ) ? 188 : 6);
//     if ( ethNum.IsPhy( ) )
//         ethP->_valueRemEthPort.SetString(ethNum.AsName( ));
//     ethP->_valueMirroringTarget.SetEthNum(EthNum::None( ));

//     ethP->_alarm_state = E_ETH_ALARM_STATE_DOWN;
//     ethP->_valueAutoNegAdmin.SetEnum(E_AutoNegState::Enable);
//     ethP->_valueLastChangeTime.UpdateBaseTime( );
//     ethP->_valueMtuSize.SetUint32(10240);
//     ethP->_valueAdminState.SetString("up");
//     ethP->_valueOperationalState.SetString("down");

//     ethP->_valueActiveConfig.DoDataControl(DATA_KEEP);
//     if ( !ethP->IsDoubleMauPort( ) ) {
//         ethP->_valueActiveConfig._inUse = false;
//         ethP->_valueActiveActual._inUse = false;
//     }

//     Db( )->AddMo(ethP);

//     return ethP;
// }

// /****************************************************************************************************************
//  * DESCRIPTION:
//  *		GetSfpDdmStatus: Check in the SFP DDM the alarm and warning.
//  *
//  * INPUTS:
//  *       first_or_second_byte  - 0 - Check the first byte from SFP DDM that contains the alarms and warnings
//  *                               1 - use the second byte
//  *       mask_val_low,mask_val_high  - Mask of the low and high alarm on warning of the parameter
//  *       ddmi_alarm_p - holds the SFP DDM alarm and warning data read from HW
//  *
//  * RETURNS:
//  *       T_SFPDDM_STATUS
//  *
//  ****************************************************************************************************************/
// static T_SFPDDM_STATUS GetSfpDdmStatus (UINT8 first_or_second_byte, UINT8 mask_val_low, UINT8 mask_val_high, T_BSP_NETWORK_PORT_SFP_ALARM_DATA* ddmi_alarm_p, UINT8 Sfpinserted, int active_port)
// {
//     UINT8 alarm_val = 0, warning_val = 0;

//     /* Check if SFP inserted */
//     if ( Sfpinserted == 0 )
//         return (ETH_SFPDDM_STATUS_NO_SFP);

//     /* Check if active link of ETH2 is not SFP port then we return "Link Down", Else we check the SFP DDM statuses */
//     if ( active_port != ETH_ACTIVE_SFP )
//         return (ETH_SFPDDM_STATUS_LINK_DOWN);

//     if ( first_or_second_byte == 0 ) {
//         alarm_val   = ddmi_alarm_p->alarm1;
//         warning_val = ddmi_alarm_p->warning1;
//     } else {
//         alarm_val   = ddmi_alarm_p->alarm2;
//         warning_val = ddmi_alarm_p->warning2;
//     }

//     if ( ((alarm_val & (mask_val_low | mask_val_high)) == 0) && ((warning_val & (mask_val_low | mask_val_high)) == 0) )
//         return (ETH_SFPDDM_STATUS_OK);

//     if ( (alarm_val & (mask_val_low | mask_val_high)) && ((warning_val & (mask_val_low | mask_val_high)) == 0) ) {
//         if ( alarm_val & mask_val_low )
//             return (ETH_SFPDDM_STATUS_ALM_LOW);
//         return (ETH_SFPDDM_STATUS_ALM_HIGH);
//     }
//     if ( ((alarm_val & (mask_val_low | mask_val_high)) == 0) && (warning_val & (mask_val_low | mask_val_high)) ) {
//         if ( warning_val & mask_val_low )
//             return (ETH_SFPDDM_STATUS_WARN_LOW);
//         return (ETH_SFPDDM_STATUS_WARN_HIGH);
//     }
//     if ( (alarm_val & (mask_val_low | mask_val_high)) && (warning_val & (mask_val_low | mask_val_high)) ) {
//         if ( (alarm_val & mask_val_high) && (warning_val & mask_val_low) )
//             return (ETH_SFPDDM_STATUS_ALM_HIGH_WARN_LOW);
//         if ( (alarm_val & mask_val_low) && (warning_val & mask_val_high) )
//             return (ETH_SFPDDM_STATUS_ALM_LOW_WARN_HIGH);
//         if ( (alarm_val & mask_val_high) && (warning_val & mask_val_high) )
//             return (ETH_SFPDDM_STATUS_ALM_HIGH_WARN_HIGH);
//         if ( (alarm_val & mask_val_low) && (warning_val & mask_val_low) )
//             return ETH_SFPDDM_STATUS_ALM_LOW_WARN_LOW;
//     }

//     return ETH_SFPDDM_STATUS_OK;
// }

// RetStatus MoEthernet::DoPerform(ActionType performType, void* dataP)
// {
//     if ( performType == GET_MO_NUMBER )
//         *((MoId*)dataP) = MoId(_valuePortNum.GetEthNum( ).AsEnum( ));
//     else if ( performType == PRINT_FULL_NAME )
//         *((MyOutStream*)dataP) << TypeName( ) << " " << _valuePortNum;
//     else if ( performType == GET_MO_TYPE )
//         *((T_MoType*)dataP) = MO_ETHERNET;
//     else if ( performType == PRINT_TYPE_NAME )
//         *((MyOutStream*)dataP) << "eth";
//     else if ( performType == REGISTER_STATIC ) {
//         RegisterAttr(ATTR_ETH_PORT, NO_CLI, &_valuePortNum, "eth");
//         RegisterAttr(ATTR_DESCRIPTION, CLI_INFO, &_valueDescription, "description");
//         RegisterAttr(ATTR_MTU_SIZE, CLI_INFO, &_valueMtuSize, "mtu");
//         RegisterAttr(ATTR_LOCAL_MAC, CLI_INFO, &_valueLocalMac, "mac-addr");
//         RegisterAttr(ATTR_ADMIN_STATE, CLI_INFO_CONF, &_valueAdminState, "admin");
//         RegisterAttr(ATTR_OPERATIONAL_STATE, CLI_INFO, &_valueOperationalState, "operational");
//         RegisterAttr(ATTR_BLOCKED_BY_EXTEND_MM_STATE, CLI_INFO, &_valueBlockedByExtendMMState, "blocked-by-extend-mm");
//         RegisterAttr(ATTR_LAST_CHANGE_TIME, CLI_INFO, &_valueLastChangeTime, "last-change");
//         RegisterAttr(ATTR_NAME, CLI_INFO, &_valueName, "name");
//         RegisterAttr(ATTR_PROMISCUOUS_MODE, NO_CLI, &_valuePromiscMode, "promisc-mode");
//         RegisterAttr(ATTR_CONNECTOR, NO_CLI, &_valueConnectorPresent, "connector");
//         RegisterAttr(ATTR_IF_TYPE, NO_CLI, &_valueIfType, "ifType");
//         RegisterAttr(ATTR_ALIAS, CLI_INFO_CONF, &_valueAlias, "alias");
//         RegisterAttr(ATTR_LOCAL_ETHER_TYPE, CLI_INFO_CONF, &_valueEtherTypeConfig, "eth-type");
//         RegisterAttr(ATTR_LOCAL_ETHER_TYPE_ACTUAL, CLI_INFO, &_valueActualEtherType, "eth-act-type");
//         RegisterAttr(ATTR_LOCAL_AUTO_NEG_ADMIN, CLI_INFO_CONF, &_valueAutoNegAdmin, "auto-neg");
//         RegisterAttr(ATTR_LOCAL_LOOP_MODE, CLI_INFO_CONF, &_valueLoop, "loopback-mode");
//         RegisterAttr(ATTR_LOCAL_LOOP_MAC, CLI_INFO_CONF, &_valueLoopMac, "loopback-dst-mac");
//         RegisterAttr(ATTR_ETH_LOOP_TIMEOUT, CLI_INFO_CONF, &_valueLoopTimeout, "loopback-timeout");
//         RegisterAttr(ATTR_ETH_MIRRORING_TARGET, CLI_INFO_CONF, &_valueMirroringTarget, "mirroring-source");
//         RegisterAttr(ATTR_ETH_MIRRORING_MODE, CLI_INFO_CONF, &_valueMirroringMode, "mirroring-mode");
//         RegisterAttr(ATTR_ALARM_PROPAGATION_EH, CLI_INFO_CONF, &_valueAlarmPropagation_eh, "alarm-propagation");
//         RegisterAttr(ATTR_LOCAL_PAUSE_FRAMES, CLI_INFO_CONF, &_valuePauseFrames, "pause-frames");
//         RegisterAttr(ATTR_REMOTE_ETH_PORT, CLI_INFO_CONF, &_valueRemEthPort, "remote-eth-port");

//         if ( Sys( )->IsSynceIeee1588Supported( ) )
//             RegisterAttr(ATTR_CLOCK_MODE, CLI_INFO_CONF, &_valueClockMode, "clock");
//         else
//             RegisterAttr(ATTR_CLOCK_MODE, NO_CLI, &_valueClockMode, "clock");

//         RegisterAttr(ATTR_LOCAL_CONNECTOR_TYPE, CLI_INFO, &_valueEtherConnectorType, "connector-type");

//         RegisterAttr(ATTR_LOCAL_SFP_DDM_TEMPERATURE, CLI_INFO, &_valueSfpDdmTemperature, "sfp-ddm-temperature");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_TEMP_STATUS, CLI_INFO, &_valueSfpDdmTemperatureStatus, "sfp-ddm-temperature-status");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_VCC, CLI_INFO, &_valueSfpDdmVcc, "sfp-ddm-vcc");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_VCC_STATUS, CLI_INFO, &_valueSfpDdmVccStatus, "sfp-ddm-vcc-status");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_TX_BIAS, CLI_INFO, &_valueSfpDdmTxBias, "sfp-ddm-tx-bias");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_TX_BIAS_STATUS, CLI_INFO, &_valueSfpDdmTxBiasStatus, "sfp-ddm-tx-bias-status");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_TX_POWER, CLI_INFO, &_valueSfpDdmTxPower, "sfp-ddm-tx-power");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_TX_POWER_STATUS, CLI_INFO, &_valueSfpDdmTxPowerStatus, "sfp-ddm-tx-power-status");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_RX_POWER, CLI_INFO, &_valueSfpDdmRxPower, "sfp-ddm-rx-power");
//         RegisterAttr(ATTR_LOCAL_SFP_DDM_RX_POWER_STATUS, CLI_INFO, &_valueSfpDdmRxPowerStatus, "sfp-ddm-rx-power-status");

//         RegisterAttr(ATTR_PCP_WRITE_PROFILE_ID, CLI_INFO_CONF, &_valuePcpId, "pcp-write-profile-id");
//         RegisterAttr(ATTR_PFC_MODE, CLI_INFO_CONF, &_valuePfcMode, "pfc-mode");
//         RegisterAttr(ATTR_DISABLE_ALARM, CLI_INFO_CONF, &_valueAlarmSuppression, "alarm-suppression");

//         RegisterAttr(ATTR_LAG_MEMBER, CLI_INFO, &_valueLagRef, "lag-member");

//         if ( HasDoublePort( ) ) {
//             RegisterAttr(ATTR_LOCAL_ACTIVE_CONFIG, CLI_INFO_CONF, &_valueActiveConfig, "port-type");
//             RegisterAttr(ATTR_LOCAL_ACTIVE_ACTUAL, CLI_INFO, &_valueActiveActual, "port-actual-type");
//         }

//         RegisterAttr(ATTR_TX_THROUGHPUT, CLI_INFO, &_valueTxThroughput, "tx-throughput");
//         RegisterAttr(ATTR_RX_THROUGHPUT, CLI_INFO, &_valueRxThroughput, "rx-throughput");
//     } else if ( performType == CONFIRM_CHANGE ) {
//         if ( (GetEthNum( ).PhyNum( ) == 1) && (_valueLoop != E_ETH_LOOP_DISABLED) )
//             return RetStatus {"Cannot enable loop for eth1. Operation failed."};

//         if ( (GetEthNum( ).IsRf( )) && (_valueLoop != E_ETH_LOOP_DISABLED) )
//             return RetStatus {"Cannot enable loop for eth0. Operation failed."};

//         if ( _valueActiveConfig.IsDataChanged( ) ) {
//             if ( !_valueActiveConfig._inUse )
//                 return RetStatus {"Cannot be assigned for the port. Operation failed."};
//         }

//         if ( _valueEtherTypeConfig.GetType(0) == E_EthernetType::Type2500xfd ) {
//             // @todo: MH code, delete it
//             if ( GetEthNum( ) != EthNum::Phy(3) )
//                 return RetStatus {"This eth-type can be used for eth3 only."};
//         }

//         if ( _valueActiveConfig.IsDataChanged( ) ) {
//             if ( (_valueEtherTypeConfig.GetType(1) == E_EthernetType::Type1000xfd) && ((EthActiveConfig)_valueActiveConfig.GetUint32( ) != ETH_ACTIVE_SFP) ) {
//                 return RetStatus {"Cannot set to rj45 or auto when the sfp speed is 1000xfd. Please change eth-type value to 10000xfd first"};
//             }
//         }

//         if ( _valueEtherTypeConfig.IsDataChanged( ) ) {
//             if ( (_valueEtherTypeConfig.GetType(1) == E_EthernetType::Type1000xfd) && ((EthActiveConfig)_valueActiveConfig.GetUint32( ) != ETH_ACTIVE_SFP) ) {
//                 return RetStatus {"Cannot set sfp speed to 1000xfd. Please change port-type value to sfp first"};
//             }
//         }

//         if ( IsRfGuest( ) )
//             return RetStatus {"The port belongs to the learned rf channel. Operation failed."};

//         if ( IsRfWigig( ) ) {
//             if ( _valueEtherTypeConfig.IsDataChanged( ) )
//                 return RetStatus {"The eth-type cannot be configured for rf port. Operation failed."};
//             if ( _valueAutoNegAdmin.IsDataChanged( ) )
//                 return RetStatus {"The auto-neg cannot be configured for rf port. Operation failed."};
//             if ( _valueAlarmSuppression.IsDataChanged( ) )
//                 return RetStatus {"The alarm-suppression cannot be configured for rf port. Operation failed."};
//         }

//         if ( _valueAlarmPropagation_eh.IsDataChanged( ) ) {
//             if ( GetEthNum( ) != EthNum::Phy(2) ) {
//                 return RetStatus {"The Alarm Propagation can be configured for eth2 only. Operation failed."};
//             }
//             MoLinkBackup* extendMMP = FindMoT<MoLinkBackup>( );
//             if ( extendMMP && extendMMP->IsActive( ) && extendMMP->_valuePortNum.GetEthNum( ) == EthNum::Phy(2) ) {
//                 return RetStatus {"Extend MM use eth2 as backup. Alarm Propogation can't be enabled"};
//             }
//         }

//         if ( GetEthNum( ) == EthNum::Phy(1) || _valuePortNum.GetEthNum( ).IsHost( ) ) {
//             if ( _valuePauseFrames.IsDataChanged( ) )
//                 return RetStatus {"Pause frames cannot be changed on this port. Operation failed."};
//         }

//         int     numOfPfcEnabled = 0;
//         ValueMo valueMo(Type( ));
//         for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//             if ( valueMo.MoP( )->AttrUint32(ATTR_PFC_MODE) != E_PFC_MODE_DISABLE )
//                 numOfPfcEnabled++;
//         }
//         if ( numOfPfcEnabled > 1 )
//             return RetStatus {"Only one port can be pfc enabled. Operation failed."};

//         if ( (_valuePortNum.GetEthNum( ).IsHost( ) || _valuePortNum.GetEthNum( ).IsRf( )) && (_valuePfcMode != E_PFC_MODE_DISABLE) )
//             return RetStatus {"PFC can't be enabled for HOST and ETH0"};

//         if ( _valueLoop.IsDataChanged( ) ) {
//             if ( _valuePortNum.GetEthNum( ).IsHost( ) )
//                 return RetStatus {"Loopback can't be changed for host. Operation failed."};

//             UINT32 loopPermission = FindSystem( )->_valueMaxLoop.GetUint32( );

//             if ( (_valueLoop == E_ETH_LOOP_EXTERNAL) || (_valueLoop == E_ETH_LOOP_INTERNAL) ) {
//                 if ( loopPermission != E_MAX_LOOP_ENABLED )
//                     return RetStatus {"No permission to start the loop. Operation failed."};
//             } else if ( (_valueLoop == E_ETH_LOOP_EXTERNAL_MAC_SWAP) || (_valueLoop == E_ETH_LOOP_INTERNAL_MAC_SWAP) ) {
//                 if ( (loopPermission != E_MAX_LOOP_ENABLED) && (loopPermission != E_MAX_LOOP_MAC_SWAP) )
//                     return RetStatus {"No permission to start the loop. Operation failed."};
//             }

//             RetStatus status = CheckLoopUnique( );
//             if ( !status.ok( ) )
//                 return status;
//         }

//         if ( _valueLoopTimeout.IsDataChanged( ) && (_valueLoop.GetUint32( ) != E_ETH_LOOP_DISABLED) )
//             return RetStatus {"Loop timeout cannot be changed when the loop is active."};

//         if ( _valueLagRef.IsLag( ) ) {
//             if ( _valueEtherTypeConfig.IsDataChanged( ) )
//                 return RetStatus {"The port is lag member, cannot change eth-type. Operation failed."};
//             if ( _valueAutoNegAdmin.IsDataChanged( ) )
//                 return RetStatus {"The port is lag member, cannot change auto-neg. Operation failed."};
//             if ( _valuePcpId.IsDataChanged( ) )
//                 return RetStatus {"The port is lag member, cannot change pcp-write-profile-id. Operation failed."};
//             if ( _valuePfcMode.IsDataChanged( ) )
//                 return RetStatus {"The port is lag member, cannot change pfc-mode. Operation failed."};
//             if ( _valueAlarmSuppression.IsDataChanged( ) )
//                 return RetStatus {"The port is lag member, cannot change alarm-suppression. Operation failed."};
//         }

//         if ( _valuePcpId.IsDataChanged( ) ) {
//             UINT32 pcpId = _valuePcpId.GetUint32( );
//             if ( (pcpId != 0) && (nullptr == FindMo(MO_PCP_WRITE_PROFILE, MoId(pcpId))) )
//                 return RetStatus {"Specified pcp profile does not exist."};
//         }

//         if ( _valueAdminState.GetEnum<E_AdminState>( ) == E_AdminState::Disable ) {
//             if ( _valuePortNum.GetEthNum( ).IsRf( ) )
//                 return RetStatus {"The rf eth admin cannot be disabled. Operation failed."};

//             Mo* oamP = FindMo(MO_LINK_OAM, MoId(_valuePortNum.GetUint32( )));
//             if ( oamP && static_cast<E_AdminState>(oamP->AttrUint32(ATTR_ADMIN_STATE)) == E_AdminState::Enable )  //@todo use AttrEnum
//                 return RetStatus {"The eth admin cannot be disabled if the link-oam admin is enabled"};
//         }
//         /*
//                 if(_valueAlarmPropagation.GetUint32() != E_ALARM_PROPAGATION_DISABLED)
//                 {

//                     if(! _valuePortNum.GetEthNum().IsPhy())
//                         return "Alarm propagation can be assigned for eth1 to eth4 only.";
//                 }
//         */
//         if ( _valueRemEthPort.IsDataChanged( ) && (!_valuePortNum.GetEthNum( ).IsPhy( )) ) {
//             return RetStatus {"Remote eth port can be assigned for eth1 to eth4 only."};
//         }

//         if ( 0 == strcmp(_valueAlias.GetP( ), "\"\"") )
//             _valueAlias.SetIntValue("");

//         if ( (_valueClockMode != E_CLOCK_MODE_AUTO) && (_valuePortNum.GetEthNum( ).IsHost( ) || _valuePortNum.GetEthNum( ).IsRf( )) )
//             return RetStatus {"host and eth0 clock can be auto only. Operation failed"};

//         // update bridges if necessary

//         _valueLastChangeTime.UpdateBaseTime( );

//         if ( _valuePortNum.GetEthNum( ).IsHost( ) || _valuePortNum.GetEthNum( ).IsRf( ) )
//             if ( (!_valueMirroringTarget.GetEthNum( ).IsNone( )) || (_valueMirroringMode.GetUint32( ) != E_MIRORRING_DISABLED) ) {
//                 return RetStatus {"Eth mirror can't be defined on host or eth0 port"};
//             }

//         if ( _valueMirroringTarget.GetUint32( ) == _valuePortNum.GetUint32( ) ) {
//             return RetStatus {"Mirror source and destination should be defined on different port."};
//         }

//         if ( _valueMirroringMode.GetUint32( ) != E_MIRORRING_DISABLED ) {
//             for ( auto it = BeginEthNumPhy( ); it.IsValid( ); it++ ) {
//                 if ( it->AsEnum( ) != _valuePortNum.GetUint32( ) ) {
//                     if ( MoEthernet* ethP = FindEthernet(*it) )
//                         if ( ethP->_valueMirroringMode.GetUint32( ) != E_MIRORRING_DISABLED && ethP->_valueMirroringTarget == _valueMirroringTarget ) {
//                             return RetStatus {"Mirror source already defined on different port."};
//                         }
//                 }
//             }
//         }

//         if ( _valueMirroringMode.GetUint32( ) != E_MIRORRING_DISABLED ) {
//             if ( MoEthernet* ethP = FindEthernet(_valuePortNum.GetEthNum( )) ) {
//                 if ( ethP->IsLag( ) )
//                     return RetStatus {"Lag member port can't be mirroring-destination"};
//             }
//         }
//     } else if ( performType == UPDATE_DYNAMIC ) {
//         T_MO_ETHERNET_DYNAMIC_DATA src;

//         UploadDynamicData(&src);

//         _dynamicData = src;

//         if ( !_valueOperationalState.SetEnum(src.operState) )
//             _valueOperationalState.SetEnum(E_OperationalState::Down);

//         _valueActualEtherType.SetEnum(src.actualEthType);

//         _valueLocalMac.SetMac(src.localMacAddress);

//         if ( _valueActiveActual._inUse )
//             _valueActiveActual.SetUint32(Eth( )->GetActualDoublePort(GetEthNum( )));

//         _valueTxThroughput.SetUint64(src.tx_throughput);
//         _valueRxThroughput.SetUint64(src.rx_throughput);

//         if ( _valueEtherConnectorType.GetUint32( ) == E_ETHER_CONNECTOR_DUAL_MEDIA ) {
//             T_BSP_NETWORK_PORT_SFP_DATA ddmi_data;

//             if ( Eth( )->GetEthernetSfpDdmiData(GetEthNum( ).PhyNum( ) - 1, ddmi_data) == E_Status::Ok ) {
//                 _valueSfpDdmTemperature.SetFloat(ddmi_data.temp);
//                 _valueSfpDdmVcc.SetFloat(ddmi_data.vcc);
//                 _valueSfpDdmTxBias.SetFloat(ddmi_data.bias);
//                 _valueSfpDdmTxPower.SetFloat(ddmi_data.tx_power);
//                 _valueSfpDdmRxPower.SetFloat(ddmi_data.rx_power);
//             }

//             T_BSP_NETWORK_PORT_SFP_ALARM_DATA ddmi_alarm_data;
//             UINT8                             Sfpinserted = 0;
//             int                               active_port;

//             if ( Eth( )->GetEthernetSfpInsertStatus(Sfpinserted) != E_Status::Ok ) {
//                 fLOG_EX_ERR(false, "fail to retrieve sfp insert status");
//                 Sfpinserted = 0;
//             }
//             active_port = Eth( )->GetActualDoublePort(GetEthNum( ));

//             if ( Eth( )->GetEthernetSfpDdmiAlarmData(GetEthNum( ).PhyNum( ) - 1, ddmi_alarm_data) == E_Status::Ok ) {
//                 _valueSfpDdmTemperatureStatus.SetUint32(GetSfpDdmStatus(0, E_BSP_Temp_Low_Alarm, E_BSP_Temp_High_Alarm, &ddmi_alarm_data, Sfpinserted, active_port));
//                 _valueSfpDdmVccStatus.SetUint32(GetSfpDdmStatus(0, E_BSP_Vcc_Low_Alarm, E_BSP_Vcc_High_Alarm, &ddmi_alarm_data, Sfpinserted, active_port));
//                 _valueSfpDdmTxBiasStatus.SetUint32(GetSfpDdmStatus(0, E_BSP_TX_Bias_Low_Alarm, E_BSP_TX_Bias_High_Alarm, &ddmi_alarm_data, Sfpinserted, active_port));
//                 _valueSfpDdmTxPowerStatus.SetUint32(GetSfpDdmStatus(0, E_BSP_TX_Power_Low_Alarm, E_BSP_TX_Power_High_Alarm, &ddmi_alarm_data, Sfpinserted, active_port));
//                 _valueSfpDdmRxPowerStatus.SetUint32(GetSfpDdmStatus(1, E_BSP_RX_Power_Low_Alarm, E_BSP_RX_Power_High_Alarm, &ddmi_alarm_data, Sfpinserted, active_port));
//             }
//         }

//         GetCountersLine( )->Update(this);
//     }

//     return E_RetStatus::Ok;
// }

// void MoEthernet::DoPostConfig(T_MoActionStatus action)
// {
//     if ( _valueLoop.IsDataChanged( ) ) {
//         if ( _valueLoop.GetUint32( ) != E_ETH_LOOP_DISABLED ) {
//             if ( _valueLoopTimeout.GetUint32( ) != 0 )
//                 _loopTimer.Start(_valueLoopTimeout.GetUint32( ));
//             SendEventLoop(_valuePortNum.GetEthNum( ), true);
//         } else {
//             SendEventLoop(_valuePortNum.GetEthNum( ), false);
//             _loopTimer.Stop( );
//         }
//     }

//     Mo::DoPostConfig(action);
// }

// static UINT64 EthTypeToDataRate (E_EthernetType ethType)
// {
//     switch ( ethType ) {
//         case E_EthernetType::Type10hd: return 10;
//         case E_EthernetType::Type10fd: return 10;
//         case E_EthernetType::Type100hd: return 100;
//         case E_EthernetType::Type100fd: return 100;
//         case E_EthernetType::Type1000xfd: return 1000;
//         case E_EthernetType::Type1000hd: return 1000;
//         case E_EthernetType::Type1000fd: return 1000;
//         case E_EthernetType::Type10000xfd: return 10000;
//         case E_EthernetType::Type10000fd: return 10000;
//         case E_EthernetType::Type2500xfd: return 2500;
//         case E_EthernetType::Type2500fd: return 2500;
//         case E_EthernetType::Type5000fd: return 5000;
//         default: return 0;
//     }
// }

// std::optional<T_MO_ETHERNET_DYNAMIC_DATA> MoEthernet::DynamicData( )
// {
//     T_MO_ETHERNET_DYNAMIC_DATA data;
//     EthNum                     ethNum = GetEthNum( );

//     MsgGetStat getStatData {.ethNum = ethNum};
//     if ( SkMessage::SkMessage(SkMessage::E_QueueId::MSG_CAD_API_Q).send(SkMessage::E_QueueId::MSG_NPU_Q, E_NPU_STATISTICS_GET_MSG, getStatData) == E_Status::Error )
//         return std::nullopt;

//     data = std::move(getStatData.data);

//     if ( ethNum.IsRf( ) ) {
//         uint32_t rate = GetCurrentDataRate( );
//         if ( rate ) {
//             data.operState = E_OperationalState::Up;
//             data.dataRate  = rate;
//         } else {
//             data.operState = E_OperationalState::Down;
//         }
//     } else {
//         if ( ethNum.IsHost( ) )
//             data.dataRate = 100;
//         else {
//             if ( static_cast<E_AdminState>(AttrUint32(ATTR_ADMIN_STATE)) == E_AdminState::Disable )
//                 data.operState = E_OperationalState::Down;
//             data.dataRate = (data.operState == E_OperationalState::Down) ? 0 : EthTypeToDataRate(data.actualEthType);
//         }
//     }

//     data.dataRate *= 1'000'000;

//     Eth( )->GetMac(ethNum).copy(data.localMacAddress);

//     return data;
// }

// void MoEthernet::UploadDynamicData(T_MO_ETHERNET_DYNAMIC_DATA* dstP)
// {
//     if ( !dstP )
//         return;

//     auto opt = DynamicData( );
//     if ( opt )
//         *dstP = opt.value( );
// }

// void MoEthernet::EthernetCountersLine::Update(Mo* moP)
// {
//     if ( moP == nullptr ) {
//         return;
//     }

//     T_MO_ETHERNET_DYNAMIC_DATA src;
//     ((MoEthernet*)moP)->UploadDynamicData(&src);

//     _data._inOctets               = src.inOctets;
//     _data._inUcastPackets         = src.inUcastPackets;
//     _data._inDiscardedPackets     = src.inDiscardPackets;
//     _data._inErrors               = src.inErrorPackets;
//     _data._outOctets              = src.outOctets;
//     _data._outUcastPackets        = src.outUcastPackets;
//     _data._outErrors              = src.outErrorPackets;
//     _data._inMcastPackets         = src.inMcastPackets;
//     _data._inBcastPackets         = src.inBcastPackets;
//     _data._outMcastPackets        = src.outMcastPackets;
//     _data._outBcastPackets        = src.outBcastPackets;
//     _data._outDiscardedPackets    = src.outDiscardPackets;
//     _data._inNoRuleDiscardPackets = src.inNoRuleDiscardPackets;
//     _data._inLagDiscardPackets    = src.inLagDiscardPackets;
//     _data._inRate                 = src.inRate;
//     _data._outRate                = src.outRate;
//     _data._util                   = src.util;
//     _data._inPackets              = src.inPackets;
//     _data._outPackets             = src.outPackets;
// }

// void MoEthernet::EthernetCountersLine::PrintLine(int index, bool isHistory, MyOutStream* outP, Mo* moP)
// {
//     if ( moP == nullptr ) {
//         return;
//     }

//     if ( index != 0 )
//         PrintListHeader(index, outP);

//     (void)isHistory;
//     //  bool isDelta = (isHistory && (index != 0));

//     ValueCounter64 valueInOctets(_data._inOctets);

//     ValueCounter64 valueInDiscards(_data._inDiscardedPackets);
//     ValueCounter64 valueInErrors(_data._inErrors);
//     ValueCounter64 valueOutOctets(_data._outOctets);

//     ValueCounter64 valueOutErrors(_data._outErrors);
//     ValueCounter64 valueOutDiscardedPkts(_data._outDiscardedPackets);
//     ValueCounter64 valueNoRule(_data._inNoRuleDiscardPackets);
//     ValueCounter64 valuenInLagDiscard(_data._inLagDiscardPackets);

//     ValueCounter64 valueUcastPkts(_data._outUcastPackets);
//     ValueCounter64 valueOutMcastPkts(_data._outMcastPackets);
//     ValueCounter64 valueOutBcastPkts(_data._outBcastPackets);

//     ValueCounter64 valueInUcastPkts(_data._inUcastPackets);
//     ValueCounter64 valueInMcastPkts(_data._inMcastPackets);
//     ValueCounter64 valueInBcastPkts(_data._inBcastPackets);

//     *outP << moP << " " << FIELD("in-octets", CliNameColumnLen) << ": " << valueInOctets << "\n";
//     *outP << moP << " " << FIELD("in-ucast-pkts", CliNameColumnLen) << ": " << valueInUcastPkts << "\n";

//     if ( !IsDebugMode( ) || _data._inDiscardedPackets == 0 )
//         *outP << moP << " " << FIELD("in-discards", CliNameColumnLen) << ": " << valueInDiscards << "\n";
//     else
//         *outP << moP << " " << FIELD("in-discards", CliNameColumnLen) << ": "
//               << "\x1b[31m" << valueInDiscards << "\x1b[39m"
//               << "\n";

//     if ( !IsDebugMode( ) || _data._inErrors == 0 )
//         *outP << moP << " " << FIELD("in-errors", CliNameColumnLen) << ": " << valueInErrors << "\n";
//     else
//         *outP << moP << " " << FIELD("in-errors", CliNameColumnLen) << ": "
//               << "\x1b[31m" << valueInErrors << "\x1b[39m"
//               << "\n";
//     *outP << moP << " " << FIELD("out-octets", CliNameColumnLen) << ": " << valueOutOctets << "\n";
//     *outP << moP << " " << FIELD("out-ucast-pkts", CliNameColumnLen) << ": " << valueUcastPkts << "\n";

//     if ( !IsDebugMode( ) || _data._outErrors == 0 )
//         *outP << moP << " " << FIELD("out-errors", CliNameColumnLen) << ": " << valueOutErrors << "\n";
//     else
//         *outP << moP << " " << FIELD("out-errors", CliNameColumnLen) << ": "
//               << "\x1b[31m" << valueOutErrors << "\x1b[39m"
//               << "\n";
//     *outP << moP << " " << FIELD("in-mcast-pkts", CliNameColumnLen) << ": " << valueInMcastPkts << "\n";
//     *outP << moP << " " << FIELD("in-bcast-pkts", CliNameColumnLen) << ": " << valueInBcastPkts << "\n";

//     *outP << moP << " " << FIELD("out-mcast-pkts", CliNameColumnLen) << ": " << valueOutMcastPkts << "\n";
//     *outP << moP << " " << FIELD("out-bcast-pkts", CliNameColumnLen) << ": " << valueOutBcastPkts << "\n";

//     if ( !IsDebugMode( ) || _data._outDiscardedPackets == 0 )
//         *outP << moP << " " << FIELD("out-discards", CliNameColumnLen) << ": " << valueOutDiscardedPkts << "\n";
//     else
//         *outP << moP << " " << FIELD("out-discards", CliNameColumnLen) << ": "
//               << "\x1b[31m" << valueOutDiscardedPkts << "\x1b[39m"
//               << "\n";

//     if ( !IsDebugMode( ) || _data._inNoRuleDiscardPackets == 0 )
//         *outP << moP << " " << FIELD("in-no-rule-discards", CliNameColumnLen) << ": " << valueNoRule << "\n";
//     else
//         *outP << moP << " " << FIELD("in-no-rule-discards", CliNameColumnLen) << "\x1b[31m"
//               << ": " << valueNoRule << "\x1b[39m"
//               << "\n";

//     *outP << moP << " " << FIELD(GetTimeName( ), CliNameColumnLen) << ": ";
//     PrintTime(outP, moP);
//     *outP << "\n";
//     *outP << "\n";  // additional separator for blocks
// }

// void MoEthernet::EthernetCountersLine::PrintSummaryHead( )
// {
//     ocli << "\n";
//     ocli << FIELD("#", 2) << FIELD("start", 20) << FIELD("ifc", 8) << FIELD("in-octets", 16) << FIELD("out-octets", 16) << FIELD("in-rate", 16) << FIELD("out-rate", 16) << FIELD("util", 10);
//     ocli << "\n";
// }

// void MoEthernet::EthernetCountersLine::PrintSummaryLine(int index, bool isHistory, Mo* moP)
// {
//     (void)isHistory;

//     if ( moP == nullptr ) {
//         return;
//     }

//     ValueCounter64 inOctets(_data._inOctets);
//     ValueCounter64 outOctets(_data._outOctets);
//     ValueCounter64 inRate(_data._inRate);
//     ValueCounter64 outRate(_data._outRate);
//     ValueCounter64 util(_data._util);

//     ValueDateAndTime valueStart(_startAbsTime);
//     ocli << FIELD(index, 2) << FIELD(&valueStart, 20) << FIELD(&((MoEthernet*)moP)->_valuePortNum, 8) << FIELD(&inOctets, 16) << FIELD(&outOctets, 16) << FIELD(&inRate, 16) << FIELD(&outRate, 16)
//          << FIELD(&util, 10);
//     ocli << "\n";
// }

// struct SnmpIfSpeed : SnmpColumn {
//     SnmpIfSpeed(bool isHigh) : _isHigh(isHigh) { }

//     bool _isHigh;

//     u_char AsnType ( ) override { return ASN_UNSIGNED; }

//     void ToSnmp (SnmpVariable* outP, Mo* moP) override
//     {
//         if ( moP == nullptr || outP == nullptr ) {
//             return;
//         }

//         if ( moP->Type( ) == MO_ETHERNET ) {
//             T_MO_ETHERNET_DYNAMIC_DATA src;

//             ((MoEthernet*)moP)->UploadDynamicData(&src);
//             UINT64 speed = src.dataRate;

//             if ( !_isHigh )
//                 *outP << ((speed > 0xFFFFFFFFL) ? 0xFFFFFFFFL : speed);
//             else
//                 *outP << (speed / 1000000);
//         } else
//             *outP << 0;
//     }
// };

// void MoEthernet::LoopCadTimer::Action( )
// {
//     _ownerP->_valueLoop.SetUint32(E_ETH_LOOP_DISABLED);
//     SendEventLoop(_ownerP->_valuePortNum.GetEthNum( ), false);
//     Stop( );
// }

// struct SnmpStatistic_Rmon_IfTable : SnmpStatistic {
//     SnmpStatistic_Rmon_IfTable(u_char asnType, CountersLine* lineP, INT64* dataP) : SnmpStatistic(asnType, lineP, (UINT64*)dataP) { MoEthernet::EthernetCountersLine::_snmpObjects.Add(this, _offset); }
// };

// void IfTable_CreateColumns (SnmpBaseTable* tableP)
// {
//     PlaceSnmpColumns p(__PRETTY_FUNCTION__);

//     struct ColumnIfTable : SnmpColumn {
//         u_char AsnType ( ) override { return _asnType; }

//         u_char _asnType;
//         int    _column;

//         ColumnIfTable(int column, u_char asnType) : _asnType(asnType), _column(column) { }

//         void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//         {
//             if ( moP == nullptr || pduP == nullptr ) {
//                 return;
//             }

//             UINT32 ifIndex = OidFromEnd(pduP, 0);

//             if ( moP->Type( ) == MO_ETHERNET ) {
//                 MoEthernet* ethP = (MoEthernet*)moP;

//                 if ( _column == 1 )
//                     ValueLong(ifIndex).DoGetSnmp(pduP);
//                 else if ( _column == 2 )
//                     ethP->_valueDescription.DoGetSnmp(pduP);
//                 else if ( _column == 3 )
//                     ethP->_valueIfType.DoGetSnmp(pduP);
//                 else if ( _column == 4 )
//                     ethP->_valueMtuSize.DoGetSnmp(pduP);
//                 else if ( _column == 6 )
//                     ethP->_valueLocalMac.DoGetSnmp(pduP);
//                 else if ( _column == 8 )
//                     ethP->_valueOperationalState.DoGetSnmp(pduP);
//                 else if ( _column == 9 )
//                     ethP->_valueLastChangeTime.DoGetSnmp(pduP);
//             } else {
//                 if ( _column == 1 )
//                     ValueLong(ifIndex).DoGetSnmp(pduP);
//                 else if ( _column == 2 ) {
//                     MyStringT<32> tmp;
//                     tmp << "vlan " << MoIp::FindVlanByIfIndex(ifIndex);
//                     *pduP << tmp;
//                 } else if ( _column == 3 )
//                     ValueLong(135).DoGetSnmp(pduP);
//                 else if ( _column == 4 ) {
//                     ValueLong(16384).DoGetSnmp(pduP);
//                 } else if ( _column == 6 )
//                     FindEthernet(EthNum::Host( ))->_valueLocalMac.DoGetSnmp(pduP);
//                 else if ( _column == 8 )
//                     ValueLong(1).DoGetSnmp(pduP);
//                 else if ( _column == 9 )
//                     ValueLong(0).DoGetSnmp(pduP);
//             }
//         }
//     };

//     tableP->Column("ifIndex", 1, new (p) ColumnIfTable(1, ASN_INTEGER));
//     tableP->Column("ifDescr", 2, new (p) ColumnIfTable(2, ASN_OCTET_STR));
//     tableP->Column("ifType", 3, new (p) ColumnIfTable(3, ASN_INTEGER));
//     tableP->Column("ifMtu", 4, new (p) ColumnIfTable(4, ASN_INTEGER));
//     tableP->Column("ifSpeed", 5, new (p) SnmpIfSpeed(false));
//     tableP->Column("ifPhysAddress", 6, new (p) ColumnIfTable(6, ASN_OCTET_STR));

//     struct ColumnIfAdminStatus : SnmpColumn {
//         u_char AsnType ( ) override { return ASN_INTEGER; }

//         bool FromSnmp (SnmpVariable* varP, Mo* moP) override
//         {
//             if ( moP == nullptr || varP == nullptr ) {
//                 return false;
//             }

//             return (moP->Type( ) == MO_ETHERNET) ? ((MoEthernet*)moP)->_valueAdminState.DoSetSnmp(varP) : false;
//         }

//         void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//         {
//             if ( moP == nullptr || pduP == nullptr ) {
//                 return;
//             }

//             if ( moP->Type( ) == MO_ETHERNET )
//                 ((MoEthernet*)moP)->_valueAdminState.DoGetSnmp(pduP);
//             else
//                 ValueLong(1).DoGetSnmp(pduP);
//         }
//     };

//     tableP->ColumnWrite("ifAdminStatus", 7, new (p) ColumnIfAdminStatus( ));

//     tableP->Column("ifOperStatus", 8, new (p) ColumnIfTable(8, ASN_INTEGER));
//     tableP->Column("ifLastChange", 9, new (p) ColumnIfTable(9, ASN_TIMETICKS));

//     // statistics

//     MoEthernet::EthernetCountersLine l;

//     tableP->Column("ifInOctets", 10, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._inOctets));
//     tableP->Column("ifInUcastPkts", 11, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._inUcastPackets));
//     tableP->Column("ifInDiscards", 13, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._inDiscardedPackets));
//     tableP->Column("ifInErrors", 14, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._inErrors));
//     tableP->Column("ifOutOctets", 16, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._outOctets));
//     tableP->Column("ifOutUcastPkts", 17, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._outUcastPackets));
//     tableP->Column("ifOutDiscards", 19, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._outDiscardedPackets));
//     tableP->Column("ifOutErrors", 20, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._outErrors));
// }

// void IfXTable_CreateColumns (SnmpBaseTable* tableP)
// {
//     PlaceSnmpColumns p(__PRETTY_FUNCTION__);

//     struct ColumnIfName : SnmpColumn {
//         u_char AsnType ( ) override { return ASN_OCTET_STR; }

//         void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//         {
//             if ( moP == nullptr || pduP == nullptr ) {
//                 return;
//             }

//             if ( moP->Type( ) == MO_ETHERNET )
//                 ((MoEthernet*)moP)->_valueName.DoGetSnmp(pduP);
//             else {
//                 UINT32        ifIndex = OidFromEnd(pduP, 0);
//                 MyStringT<32> tmp;
//                 tmp << "vlan " << MoIp::FindVlanByIfIndex(ifIndex);
//                 *pduP << tmp;
//             }
//         }
//     };

//     tableP->Column("ifName", 1, new (p) ColumnIfName( ));

//     tableP->Column("ifLinkUpDownTrapEnable", 14, new (p) SnmpConst_Integer(2));
//     tableP->Column("ifHighSpeed", 15, new (p) SnmpIfSpeed(true));

//     struct ColumnIfXTable : SnmpColumn {
//         u_char AsnType ( ) override { return _asnType; }

//         u_char _asnType;
//         int    _column;

//         ColumnIfXTable(int column, u_char asnType) : _asnType(asnType), _column(column) { }

//         void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//         {
//             if ( moP == nullptr || pduP == nullptr ) {
//                 return;
//             }

//             if ( moP->Type( ) == MO_ETHERNET ) {
//                 MoEthernet* ethP = (MoEthernet*)moP;

//                 if ( _column == 16 )
//                     ethP->_valuePromiscMode.DoGetSnmp(pduP);
//                 else if ( _column == 17 )
//                     ethP->_valueConnectorPresent.DoGetSnmp(pduP);
//             } else {
//                 if ( _column == 16 )
//                     ValueLong(2).DoGetSnmp(pduP);
//                 else if ( _column == 17 )
//                     ValueLong(2).DoGetSnmp(pduP);
//             }
//         }
//     };

//     tableP->Column("ifPromiscuousMode", 16, new (p) ColumnIfXTable(16, ASN_INTEGER));
//     tableP->Column("ifConnectorPresent", 17, new (p) ColumnIfXTable(17, ASN_INTEGER));

//     struct ColumnIfAlias : SnmpColumn {
//         u_char AsnType ( ) override { return ASN_OCTET_STR; }

//         bool FromSnmp (SnmpVariable* varP, Mo* moP) override
//         {
//             if ( moP == nullptr || varP == nullptr ) {
//                 return false;
//             }

//             return (moP->Type( ) == MO_ETHERNET) ? ((MoEthernet*)moP)->_valueAlias.DoSetSnmp(varP) : false;
//         }

//         void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//         {
//             if ( moP == nullptr || pduP == nullptr ) {
//                 return;
//             }

//             if ( moP->Type( ) == MO_ETHERNET )
//                 ((MoEthernet*)moP)->_valueAlias.DoGetSnmp(pduP);
//             else
//                 ValueString32( ).DoGetSnmp(pduP);
//         }
//     };

//     tableP->ColumnWrite("ifAlias", 18, new (p) ColumnIfAlias( ));

//     // statistics

//     MoEthernet::EthernetCountersLine l;

//     tableP->Column("ifInMulticastPkts", 2, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._inMcastPackets));
//     tableP->Column("ifInBroadcastPkts", 3, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._inBcastPackets));
//     tableP->Column("ifOutMulticastPkts", 4, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._outMcastPackets));
//     tableP->Column("ifOutBroadcastPkts", 5, new (p) SnmpStatistic_Rmon_IfTable(ASN_COUNTER, &l, &l._data._outBcastPackets));
//     tableP->Column("ifHCInOctets", 6, new (p) SnmpStatistic(ASN_COUNTER64, &l, (UINT64*)(&l._data._inOctets)));
//     tableP->Column("ifHCOutOctets", 10, new (p) SnmpStatistic(ASN_COUNTER64, &l, (UINT64*)(&l._data._outOctets)));
// }

// struct SnmpIfIndexTable : public SnmpDynamicTable {
//     struct OidIteratorIfIndex : public OidIterator {
//         Mo*    _table[50];
//         UINT32 _numOf;
//         UINT32 _index;

//         MyOid GetOid ( ) override
//         {
//             MyOid myOid;
//             myOid << GetIfIndex(_table[_index]);
//             return myOid;
//         }

//         void SetFirst ( ) override
//         {
//             _numOf = 0;
//             _index = 0;

//             ValueMo valueMo(MO_ETHERNET);
//             for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) )
//                 _table[_numOf++] = valueMo.MoP( );

//             valueMo = ValueMo(MO_IP);
//             for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//                 MoIp* ipP = (MoIp*)valueMo.MoP( );
//                 if ( ipP->_valueVlan.GetUint32( ) != 0 )
//                     _table[_numOf++] = ipP;
//             }

//             qsort(_table, _numOf, sizeof(_table[0]), CompareIfIndex);
//         }

//         void SetNext ( ) override
//         {
//             if ( IsValid( ) )
//                 _index++;
//         }

//         bool IsValid ( ) override { return (_index < _numOf); }

//         static int CompareIfIndex (const void* lv, const void* rv)
//         {
//             UINT32 ifIndex_1 = GetIfIndex(*((Mo**)lv));
//             UINT32 ifIndex_2 = GetIfIndex(*((Mo**)rv));

//             if ( ifIndex_1 < ifIndex_2 )
//                 return -1;
//             if ( ifIndex_1 > ifIndex_2 )
//                 return 1;

//             return 0;
//         }

//         static UINT32 GetIfIndex (Mo* moP)
//         {
//             if ( moP == nullptr ) {
//                 return 0;
//             }

//             if ( moP->Type( ) == MO_ETHERNET )
//                 return ((MoEthernet*)moP)->GetIfIndex( );
//             if ( moP->Type( ) == MO_IP )
//                 return ((MoIp*)moP)->GetIfIndexForVlan( );
//             return 0;
//         }
//     };

//     SnmpIfIndexTable(SnmpTableRoot root) : SnmpDynamicTable(root) { AssignTop(&_itIfIndex); }

//     Mo* MoP ( ) override { return _itIfIndex._table[_itIfIndex._index]; }

//     OidIteratorIfIndex _itIfIndex;
// };

// void MoEthernet::DoSnmpPerform(ActionType performType)
// {
//     static SnmpIfIndexTable ifTable(SnmpTableRoot("ifEntry", MyOid {"1.3.6.1.2.1.2.2.1"}));
//     static SnmpIfIndexTable ifXTable(SnmpTableRoot("ifXEntry", MyOid {"1.3.6.1.2.1.31.1.1.1"}));

//     struct MoToOid {
//         static MyOid Exec (MoEthernet* moP)
//         {
//             static MyOid myOid;  // dummy
//             if ( moP == nullptr ) {
//                 return myOid;
//             }

//             return MyOid(moP->GetIfIndex( ));
//         }
//     };

//     static SnmpTableMoT<OidIteratorMoT<MoEthernet, MoToOid>> reg_sikluEthernetEntry(sikluEthernetEntry);

//     if ( performType == CREATE_SNMP_COLUMNS ) {
//         IfTable_CreateColumns(&ifTable);
//         IfXTable_CreateColumns(&ifXTable);
//         //      regDot3StatsEntry.CreateSnmpColumn( "ether:duplex", "19", FINT_Integer, ATTR_ETHER_ACTUAL_DUPLEX);

//         PlaceSnmpColumns p(__PRETTY_FUNCTION__);

//         struct SnmpIfIndex : SnmpColumn {
//             u_char AsnType ( ) override { return ASN_INTEGER; }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 UINT32 ifIndex = ((MoEthernet*)moP)->GetIfIndex( );
//                 *pduP << ifIndex;
//             }
//         };

//         //        reg_sikluEthernetEntry.ColumnWrite( "ethernetAlarmPropagation", 2, new(p) SnmpAttr_Integer( ATTR_ALARM_PROPAGATION));
//         reg_sikluEthernetEntry.ColumnWrite("ethernetLoopMode", 3, new (p) SnmpAttr_Integer(ATTR_LOCAL_LOOP_MODE));
//         reg_sikluEthernetEntry.ColumnWrite("ethernetLoopTimeout", 4, new (p) SnmpAttr_Integer(ATTR_ETH_LOOP_TIMEOUT));
//         reg_sikluEthernetEntry.ColumnWrite("ethernetPcpWriteProfileId", 6, new (p) SnmpAttr_Integer32(ATTR_PCP_WRITE_PROFILE_ID));
//         //        reg_sikluEthernetEntry.ColumnWrite( "ethernetClassifierMode", 7, new(p) SnmpAttr_Integer( ATTR_CLASSIFIER_MODE));
//         reg_sikluEthernetEntry.ColumnWrite("ethernetAlarmPropagationRemotePort", 8, new (p) SnmpAttr_Integer(ATTR_REMOTE_ETH_PORT));
//         reg_sikluEthernetEntry.Column("ethernetTxThroughput", 9, new (p) SnmpAttr_Counter64(ATTR_TX_THROUGHPUT));
//         reg_sikluEthernetEntry.Column("ethernetRxThroughput", 10, new (p) SnmpAttr_Counter64(ATTR_RX_THROUGHPUT));
//     }

//     SnmpPerformMau(performType);

//     SnmpPerformSfpDdm(performType);
// }

// void MoEthernet::SnmpPerformMau(ActionType performType)
// {
//     struct MoToOid {
//         static MyOid Exec (MoEthernet* moP)
//         {
//             static MyOid myOid;  // dummy
//             if ( moP == nullptr ) {
//                 return myOid;
//             }

//             return MyOid(moP->GetIfIndex( ));
//         }
//     };

//     struct SnmpEthernetMau : public SnmpTableMoT<OidIteratorMoT<MoEthernet, MoToOid>> {
//         SnmpEthernetMau(SnmpTableRoot root) : SnmpTableMoT(root), _itMau(this) { _topP->AddChild(&_itMau); }

//         struct OidIteratorMau : public OidIterator {
//             OidIteratorMau(SnmpEthernetMau* ownerP) : _ownerP(ownerP) { }

//             MyOid GetOid ( ) override { return MyOid {_mauNumber}; }

//             void SetFirst ( ) override { _mauNumber = 1; }

//             void SetNext ( ) override { _mauNumber++; }

//             bool IsValid ( ) override
//             {
//                 if ( _ownerP->_oidIterator.MoP( ) == nullptr ) {
//                     return false;
//                 }

//                 MoEthernet* ethP     = _ownerP->_oidIterator.MoP( );
//                 UINT32      numOfMau = ethP->IsDoubleMauPort( ) ? 2 : 1;
//                 return (_mauNumber <= numOfMau);
//             }

//             SnmpEthernetMau* _ownerP;
//             MyOidNum         _mauNumber;
//         };

//         OidIteratorMau _itMau;
//     };

//     static SnmpEthernetMau reg_ifMauEntry(ifMauEntry);
//     static SnmpEthernetMau reg_ifMauAutoNegEntry(ifMauAutoNegEntry);

//     if ( performType == CREATE_SNMP_COLUMNS ) {
//         PlaceSnmpColumns p(__PRETTY_FUNCTION__);

//         struct SnmpColumnMau : SnmpColumn {
//             SnmpColumnMau(u_char asnType) : _asnType(asnType) { }

//             u_char _asnType;

//             u_char AsnType ( ) override { return _asnType; }

//             UINT32 MauNumber (SnmpVariable* pduP) { return OidFromEnd(pduP, 0); }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 UINT32 columnNum = pduP->_columnP->_columnNum;

//                 UINT32      mauIndex = MauNumber(pduP) - 1;
//                 MoEthernet* ethP     = (MoEthernet*)moP;

//                 //              MyStringT<128> tmp;
//                 //              tmp << ethP;
//                 //              MyDump().Put( "============== [%-20s] [%d] columnNum = %d", tmp.AsStr(), mauIndex, columnNum).Print();

//                 switch ( columnNum ) {
//                     case 1:  // ifMauIfIndex
//                         *pduP << ((MoEthernet*)moP)->GetIfIndex( );
//                         break;
//                     case 2:  // ifMauIndex
//                         *pduP << MauNumber(pduP);
//                         break;
//                     case 3:   // ifMauType - actual
//                     case 11:  // ifMauDefaultType - config
//                         {
//                             E_EthernetType mauType = (columnNum == 3) ? ethP->_valueActualEtherType.GetType( ) : ethP->_valueEtherTypeConfig.GetType(mauIndex);
//                             if ( mauType != E_EthernetType::Illegal ) {
//                                 MyOid dot3MauType("1.3.6.1.2.1.26.4");
//                                 dot3MauType.AddNum(static_cast<MyOidNum>(mauType));
//                                 *pduP << dot3MauType;
//                             } else
//                                 *pduP << MyOid("0.0");
//                             break;
//                         }
//                     case 4:  // ifMauStatus
//                         *pduP << 3;
//                         break;
//                     case 5:  // ifMauMediaAvailable
//                         *pduP << ((MoEthernet*)moP)->_dynamicData.mediaAvailable;
//                         break;
//                     case 6:  // ifMauMediaAvailableStateExits
//                         *pduP << ((MoEthernet*)moP)->_dynamicData.mediaAvailableExitsCount;
//                         break;
//                     case 7:          // ifMauJabberState
//                         *pduP << 3;  // noJabber
//                         break;
//                     case 8:  // ifMauJabberingStateEnters
//                         *pduP << 0;
//                         break;
//                     case 9:  // ifMauFalseCarriers
//                         *pduP << 0;
//                         break;
//                     case 12:  // ifMauAutoNegSupported
//                         *pduP << 1;
//                         break;
//                     case 13:  // ifMauTypeListBits
//                         {
//                             std::array<uint8_t, 5> ifMauTypeListBits = {0x0, 0x31, 0x85, 0xE6, 0x0};
//                             pduP->setValueArray(ifMauTypeListBits.data( ), ifMauTypeListBits.size( ));
//                             break;
//                         }
//                     case 14:  // ifMauHCFalseCarriers
//                         *pduP << 0;
//                         break;
//                 }
//             }

//             bool FromSnmp (SnmpVariable* varP, Mo* moP) override
//             {
//                 if ( moP == nullptr || varP == nullptr ) {
//                     return false;
//                 }

//                 UINT32      columnNum = varP->_columnP->_columnNum;
//                 MoEthernet* ethP      = (MoEthernet*)moP;

//                 switch ( columnNum ) {
//                     case 11:  // ifMauDefaultType - config
//                         {
//                             MyOid oid = varP->AsOid( );

//                             MyOid dot3MauType("1.3.6.1.2.1.26.4");

//                             if ( oid.Len( ) != (dot3MauType.Len( ) + 1) )
//                                 return false;
//                             if ( oid.RelatesTo(dot3MauType) != OidRelation::UnderSecond )
//                                 return false;

//                             UINT32 mauIndex = MauNumber(varP) - 1;
//                             UINT32 mauType  = oid.Get(dot3MauType.Len( ));
//                             return ethP->_valueEtherTypeConfig.SetSnmpType(mauIndex, mauType);
//                         }
//                     default: return false;
//                 }
//             }
//         };

//         reg_ifMauEntry.Column("ifMauIfIndex", 1, new (p) SnmpColumnMau(ASN_INTEGER));
//         reg_ifMauEntry.Column("ifMauIndex", 2, new (p) SnmpColumnMau(ASN_INTEGER));
//         reg_ifMauEntry.Column("ifMauType", 3, new (p) SnmpColumnMau(ASN_OBJECT_ID));  // actual
//         reg_ifMauEntry.Column("ifMauStatus", 4, new (p) SnmpColumnMau(ASN_INTEGER));
//         reg_ifMauEntry.Column("ifMauMediaAvailable", 5, new (p) SnmpColumnMau(ASN_INTEGER));
//         reg_ifMauEntry.Column("ifMauMediaAvailableStateExits", 6, new (p) SnmpColumnMau(ASN_COUNTER));
//         reg_ifMauEntry.Column("ifMauJabberState", 7, new (p) SnmpColumnMau(ASN_INTEGER));
//         reg_ifMauEntry.Column("ifMauJabberingStateEnters", 8, new (p) SnmpColumnMau(ASN_COUNTER));
//         reg_ifMauEntry.Column("ifMauFalseCarriers", 9, new (p) SnmpColumnMau(ASN_COUNTER));
//         reg_ifMauEntry.ColumnWrite("ifMauDefaultType", 11, new (p) SnmpColumnMau(ASN_OBJECT_ID));
//         reg_ifMauEntry.Column("ifMauAutoNegSupported", 12, new (p) SnmpColumnMau(ASN_INTEGER));
//         reg_ifMauEntry.Column("ifMauTypeListBits", 13, new (p) SnmpColumnMau(ASN_OCTET_STR));
//         reg_ifMauEntry.Column("ifMauHCFalseCarriers", 14, new (p) SnmpColumnMau(ASN_COUNTER64));

//         struct SnmpColumnMauAutoNeg : SnmpColumn {
//             SnmpColumnMauAutoNeg(u_char asnType) : _asnType(asnType) { }

//             u_char _asnType;

//             u_char AsnType ( ) override { return _asnType; }

//             UINT32 MauNumber (SnmpVariable* pduP) { return OidFromEnd(pduP, 0); }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 UINT32      columnNum = pduP->_columnP->_columnNum;
//                 UINT32      mauIndex  = MauNumber(pduP) - 1;
//                 MoEthernet* ethP      = (MoEthernet*)moP;

//                 (void)mauIndex;

//                 switch ( columnNum ) {
//                     case 1:  // ifMauAutoNegAdminStatus
//                         ethP->_valueAutoNegAdmin.DoGetSnmp(pduP);
//                         break;
//                     case 9:  // ifMauAutoNegCapabilityBits
//                         {
//                             uint16_t tmp {0};
//                             pduP->setValue(tmp);  // 2 bytes
//                         }
//                         break;
//                 }
//             }

//             bool FromSnmp (SnmpVariable* varP, Mo* moP) override
//             {
//                 if ( moP == nullptr || varP == nullptr ) {
//                     return false;
//                 }

//                 UINT32      columnNum = varP->_columnP->_columnNum;
//                 UINT32      mauIndex  = MauNumber(varP) - 1;
//                 MoEthernet* ethP      = (MoEthernet*)moP;

//                 (void)mauIndex;

//                 switch ( columnNum ) {
//                     case 1:  // ifMauAutoNegAdminStatus
//                         return ethP->_valueAutoNegAdmin.DoSetSnmp(varP);
//                     default: return false;
//                 }
//             }
//         };

//         reg_ifMauAutoNegEntry.ColumnWrite("ifMauAutoNegAdminStatus", 1, new (p) SnmpColumnMauAutoNeg(ASN_INTEGER));
//         reg_ifMauAutoNegEntry.Column("ifMauAutoNegCapabilityBits", 9, new (p) SnmpColumnMauAutoNeg(ASN_OCTET_STR));
//     }
// }

// void MoEthernet::SnmpPerformSfpDdm(ActionType performType)
// {
//     struct MoToOid {
//         static MyOid Exec (MoEthernet* moP)
//         {
//             static MyOid myOid;  // dummy
//             if ( moP == nullptr ) {
//                 return myOid;
//             }

//             return MyOid(moP->GetIfIndex( ));
//         }
//     };

//     struct OidIteratorMoTSfp : public OidIterator {
//         ValueMo _valueMo;

//         OidIteratorMoTSfp( ) : _valueMo(MoEthernet::MO_TYPE) { }

//         MoEthernet* MoP ( ) { return (MoEthernet*)_valueMo.MoP( ); }

//         bool IsValid ( ) override { return _valueMo.IsValid( ); }

//         // skip not SFP interfaces and not active SFP interfaces
//         void SkipToTheActiveSFP ( )
//         {
//             while ( _valueMo.IsValid( ) ) {
//                 if ( MoP( )->IsDoubleMauPort( ) ) {
//                     EthNum ethNum = MoP( )->GetEthNum( );

//                     if ( Eth( )->GetActualDoublePort(ethNum) == ETH_ACTIVE_SFP ) {
//                         if ( MoP( )->_valueSfpDdmTemperatureStatus.GetUint32( ) != ETH_SFPDDM_STATUS_NO_SFP ) {
//                             // active SFP port found, don't skip to the next port
//                             break;
//                         }
//                     }
//                 }

//                 // not active SFP port, try the next one
//                 _valueMo.SetNext( );
//             }
//         }

//         void SetFirst ( ) override
//         {
//             _valueMo.SetFirst( );

//             // skip non-SFP interfaces
//             SkipToTheActiveSFP( );
//         }

//         void SetNext ( ) override
//         {
//             _valueMo.SetNext( );

//             // skip non-SFP interfaces
//             SkipToTheActiveSFP( );
//         }

//         MyOid GetOid ( ) override { return MoToOid::Exec(MoP( )); }
//     };

//     static SnmpTableMoT<OidIteratorMoTSfp> regEntry(rbSfpDdmEntry);

//     if ( performType == CREATE_SNMP_COLUMNS ) {
//         PlaceSnmpColumns p(__PRETTY_FUNCTION__);

//         regEntry.Column("rbSfpIndex", 1, new (p) SnmpAttr_Unsigned32(ATTR_ETH_PORT));

//         struct ColumnSfpTemperature : SnmpColumn {
//             u_char AsnType ( ) override { return ASN_INTEGER; }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 if ( moP->Type( ) == MO_ETHERNET ) {
//                     float valueF = ((MoEthernet*)moP)->_valueSfpDdmTemperature.GetFloat( ) * 256.;  // see BSP_GetNetworkPortSfpData() in file bsp_common.cpp
//                     int   value  = (int)valueF;
//                     pduP->setValue(value);
//                 } else {
//                     ValueLong(0).DoGetSnmp(pduP);
//                 }
//             }
//         };

//         regEntry.Column("rbSfpTemperature", 2, new (p) ColumnSfpTemperature( ));

//         struct ColumnSfpVcc : SnmpColumn {
//             u_char AsnType ( ) override { return ASN_UNSIGNED; }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 if ( moP->Type( ) == MO_ETHERNET ) {
//                     float valueF = ((MoEthernet*)moP)->_valueSfpDdmVcc.GetFloat( ) * 10000.;  // see BSP_GetNetworkPortSfpData() in file bsp_common.cpp
//                     int   value  = (int)valueF;
//                     pduP->setValue(value);
//                 } else {
//                     ValueLong(0).DoGetSnmp(pduP);
//                 }
//             }
//         };

//         regEntry.Column("rbSfpVcc", 3, new (p) ColumnSfpVcc( ));

//         struct ColumnSfpTxBias : SnmpColumn {
//             u_char AsnType ( ) override { return ASN_UNSIGNED; }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 if ( moP->Type( ) == MO_ETHERNET ) {
//                     float valueF = ((MoEthernet*)moP)->_valueSfpDdmTxBias.GetFloat( ) * 500.;  // see BSP_GetNetworkPortSfpData() in file bsp_common.cpp
//                     int   value  = (int)valueF;
//                     pduP->setValue(value);
//                 } else {
//                     ValueLong(0).DoGetSnmp(pduP);
//                 }
//             }
//         };

//         regEntry.Column("rbSfpTxBias", 4, new (p) ColumnSfpTxBias( ));

//         struct ColumnSfpTxPower : SnmpColumn {
//             u_char AsnType ( ) override { return ASN_UNSIGNED; }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 if ( moP->Type( ) == MO_ETHERNET ) {
//                     float valueF = ((MoEthernet*)moP)->_valueSfpDdmTxPower.GetFloat( ) * 10000.;  // see BSP_GetNetworkPortSfpData() in file bsp_common.cpp
//                     int   value  = (int)valueF;
//                     pduP->setValue(value);
//                 } else {
//                     ValueLong(0).DoGetSnmp(pduP);
//                 }
//             }
//         };

//         regEntry.Column("rbSfpTxPower", 5, new (p) ColumnSfpTxPower( ));

//         struct ColumnSfpRxPower : SnmpColumn {
//             u_char AsnType ( ) override { return ASN_UNSIGNED; }

//             void ToSnmp (SnmpVariable* pduP, Mo* moP) override
//             {
//                 if ( moP == nullptr || pduP == nullptr ) {
//                     return;
//                 }

//                 if ( moP->Type( ) == MO_ETHERNET ) {
//                     float valueF = ((MoEthernet*)moP)->_valueSfpDdmRxPower.GetFloat( ) * 10000.;  // see BSP_GetNetworkPortSfpData() in file bsp_common.cpp
//                     int   value  = (int)valueF;
//                     pduP->setValue(value);
//                 } else {
//                     ValueLong(0).DoGetSnmp(pduP);
//                 }
//             }
//         };

//         regEntry.Column("rbSfpRxPower", 6, new (p) ColumnSfpRxPower( ));
//     }
// }

// ////////////////////////////////////////////////////

// MoEthernet::EthernetCountersLine::SnmpObjects MoEthernet::EthernetCountersLine::_snmpObjects;

// ////////////////////////////////////////////////////

// #include "CaStream.h"
// #include "CliContext.h"
// #include "CliMo.h"

// void MoEthernet::SingleShowInfo(CliModifierShow* modifierP)
// {
//     UpdateDynamic( );

//     ocli << FOCUS(this);
//     ocli << "\n";

//     for ( UINT32 i = 0; i < modifierP->_info._numOf; i++ ) {
//         AttrId attrId = modifierP->_info._array[i]._attrId;

//         if ( (!modifierP->IsInfo( )) && (modifierP->GetAttr( ) != attrId) )
//             continue;

//         if ( IsRfWigig( ) ) {
//             if ( (attrId == ATTR_LOCAL_ETHER_TYPE) || (attrId == ATTR_LOCAL_ETHER_TYPE_ACTUAL) || (attrId == ATTR_LOCAL_AUTO_NEG_ADMIN) || (attrId == ATTR_CONNECTOR)
//                  || (attrId == ATTR_DISABLE_ALARM) )
//                 continue;
//         }

//         if ( _valueEtherConnectorType.GetUint32( ) != E_ETHER_CONNECTOR_DUAL_MEDIA ) {
//             if ( (attrId == ATTR_LOCAL_SFP_DDM_TEMPERATURE) || (attrId == ATTR_LOCAL_SFP_DDM_VCC) || (attrId == ATTR_LOCAL_SFP_DDM_TX_BIAS) || (attrId == ATTR_LOCAL_SFP_DDM_TX_POWER)
//                  || (attrId == ATTR_LOCAL_SFP_DDM_RX_POWER) || (attrId == ATTR_LOCAL_SFP_DDM_TEMP_STATUS) || (attrId == ATTR_LOCAL_SFP_DDM_VCC_STATUS) || (attrId == ATTR_LOCAL_SFP_DDM_TX_BIAS_STATUS)
//                  || (attrId == ATTR_LOCAL_SFP_DDM_TX_POWER_STATUS) || (attrId == ATTR_LOCAL_SFP_DDM_RX_POWER_STATUS) )
//                 continue;
//         }

//         if ( GetEthNum( ).PhyNum( ) != 2 /* ETH2 */ ) {
//             if ( attrId == ATTR_ALARM_PROPAGATION_EH )
//                 continue;
//         }

//         if ( (GetEthNum( ).PhyNum( ) == 1 /* ETH1 */) || (_valuePortNum.GetEthNum( ).IsHost( )) ) {
//             if ( attrId == ATTR_LOCAL_PAUSE_FRAMES )
//                 continue;
//         }

//         ocli << TypeName( ) << " " << WIDTH(&_valuePortNum, 12);
//         ocli << " " << NAME(attrId, CliNameColumnLen) << ocli.GetNameValSeparator( ) << VAL(attrId) << "\n";
//     }
// }

// MoEthernet::ValueBlockedByExtendMMState::BlockedState MoEthernet::ValueBlockedByExtendMMState::GetBlockedState( )
// {
//     MoLinkBackup* extendMMP = FindMoT<MoLinkBackup>( );
//     if ( extendMMP == nullptr )
//         return UNKNOWN;

//     if ( !extendMMP->IsActive( ) )
//         return UNBLOCKED;
//     if ( !extendMMP->IsMaster( ) )
//         return UNBLOCKED;

//     EthNum thisPort           = _ownerP->GetEthNum( );
//     EthNum extendMMbackupPort = extendMMP->_valuePortNum.GetEthNum( );
//     if ( thisPort != extendMMbackupPort )
//         return UNBLOCKED;

//     // cli command "show extend-mm" has some side effects.
//     // To access up-to-date links state, we need to execute DoPerform():
//     extendMMP->DoPerform(UPDATE_DYNAMIC, nullptr);

//     UINT32 backupLinkState = extendMMP->_valueBackupLink.GetUint32( );
//     switch ( backupLinkState ) {
//         case LB_LINK_STATE_ACTIVE:
//         case LB_LINK_STATE_FORCED_ACTIVE: return UNBLOCKED;
//         case LB_LINK_STATE_STANDBY:
//         case LB_LINK_STATE_FORCED_STANDBY: return BLOCKED;
//         case LB_LINK_STATE_DOWN: break;
//         default:  // unexpected state
//             return UNKNOWN;
//     }

//     UINT32 mainLinkState = extendMMP->_valueMainLink.GetUint32( );
//     switch ( mainLinkState ) {
//         case LB_LINK_STATE_ACTIVE:
//         case LB_LINK_STATE_FORCED_ACTIVE: return BLOCKED;
//         case LB_LINK_STATE_STANDBY:
//         case LB_LINK_STATE_FORCED_STANDBY:
//         case LB_LINK_STATE_DOWN: break;
//         default:  // unexpected state
//             return UNKNOWN;
//     }

//     return UNKNOWN;
// }

// void MoEthernet::DoHandleShow(ActionType cmdType)
// {
//     CliReaderIdList<ValueEthernetPortsList> id_ports(ATTR_ETH_PORT);

//     CliModifierShow modifier(Type( ));
//     modifier.AssignHasHistory( );

//     if ( cmdType == ACTION_CLI_HELP )
//         return Cli_PrintStandardHelpShow(&modifier);

//     *cliP >> id_ports >> modifier >> END( );
//     if ( cliP->CheckExit(true) )
//         return;

//     if ( modifier.IsStats( ) || modifier.IsStatsSummary( ) )
//         Cli_PrintStandardShowStatistics(&modifier);
//     else {
//         ValueMoMatchCli valueMo(Type( ));

//         for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//             MoEthernet* moP = (MoEthernet*)valueMo.MoP( );
//             if ( moP != nullptr ) {
//                 moP->SingleShowInfo(&modifier);
//             }
//         }
//     }
// }

// struct ValueEthernetPortExist : public ValueEthernetPort {
//     bool DoIsMatch ( ) override
//     {
//         if ( !ValueEthernetPort::DoIsMatch( ) )
//             return false;

//         EthNum ethNum = GetEthNum( );
//         if ( ethNum.IsRf( ) )
//             return ethNum.IsRfExist( );
//         return true;
//     }
// };

// void MoEthernet::DoHandleSet(ActionType cmdType)
// {
//     CliReaderId<ValueEthernetPortExist> id_port(ATTR_ETH_PORT);

//     AttrListSet attrList(Type( ));
//     attrList.AddSettable( );

//     if ( cmdType == ACTION_CLI_HELP )
//         return Cli_PrintStandardHelpSet(&attrList);

//     *cliP >> id_port;

//     if ( cliP->CheckExit(true) )
//         return;

//     MoEthernet* moP = (MoEthernet*)FindMo(Type( ), MoId(id_port._valueVal.GetUint32( )));
//     if ( moP == nullptr ) {
//         return;
//     }

//     ((MoEthernet*)SampleMo(MO_ETHERNET))->_valueEtherTypeConfig.CopyFrom(moP);

//     *cliP >> attrList >> END( );
//     if ( cliP->CheckExit(true) )
//         return;

//     if ( !CopySampleTo(moP, &attrList) )
//         return;

//     if ( moP->_valueEtherTypeConfig.IsDataChanged( ) ) {
//         if ( moP->_valueEtherTypeConfig.GetType(1) == E_EthernetType::Type10000xfd ) {
//             ocli << "\nIn order that the change of the eth-type SFP from 1000xfd to 10000xfd will work,\nyou need to save running-configuration into startup-configuration and then do reset system";
//         }
//     }
// }

// void MoEthernet::DoHandleClear(ActionType cmdType)
// {
//     CliReaderId<ValueEthernetPortExist> id_port(ATTR_ETH_PORT);

//     CliModifierClear modifier(Type( ));

//     if ( cmdType == ACTION_CLI_HELP )
//         return Cli_PrintStandardHelpClear( );

//     *cliP >> id_port >> modifier >> END( );

//     if ( cliP->CheckExit(true) )
//         return;

//     ValueMoMatchCli valueMo(Type( ));
//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         if ( valueMo.MoP( ) == nullptr ) {
//             continue;
//         }

//         valueMo.MoP( )->ResetStatistics( );
//         ocli << "\n" << valueMo.MoP( ) << " statistics cleared";
//     }
// }

// /////////////////////////////////////////////
