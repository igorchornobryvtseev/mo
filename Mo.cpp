#include "igorc_common.h"


#include "Mo.h"

#include "Database.h"
// #include "MoAlarm.h"
// #include "MoArp.h"
// #include "MoDebugLog.h"
// #include "MoEms.h"
// #include "MoEthernet.h"
// #include "MoFdbEntry.h"
// #include "MoIp.h"
// #include "MoIpV6.h"
// #include "MoIpV6Dns.h"
// #include "MoRf.h"
// #include "MoSfs.h"
// #include "MoSnmpAgent.h"
// #include "MoSnmpManager.h"
// #include "MoSw.h"
// #include "MoSystem.h"
// #include "MyDump.h"
#include "MyString.h"
#include "Value.h"
// // #include "MoInQueue.h"
// #include "MoInventory.h"
// #include "MoNpu.h"
// #include "MoPortDiag.h"
// // #include "MoQueue.h"
// #include "CaStream.h"
// #include "CliContext.h"
// #include "CliMo.h"
// #include "DebugFlag.h"
// #include "HistoryBucket.h"
// #include "MoAaa.h"
// #include "MoAaaServer.h"
// #include "MoAccessList.h"
// #include "MoAcmOam.h"
// #include "MoBridgeCommon.h"
// #include "MoCertificate.h"
// #include "MoCfmMd.h"
// #include "MoClassifierCos.h"
// #include "MoClassifierEvc.h"
// #include "MoD6clientOpts.h"
// #include "MoDhcpV4.h"
// #include "MoDhcpV6.h"
// #include "MoEgressQos.h"
// #include "MoEncryption.h"
// #include "MoErp.h"
// #include "MoEventConfig.h"
// #include "MoFdbEvcQuota.h"
// #include "MoFdbQuota.h"
// #include "MoIeee1588.h"
// #include "MoIngressQos.h"
// #include "MoLag.h"
// #include "MoLicense.h"
// #include "MoLinkBackup.h"
// #include "MoLinkOam.h"
// #include "MoLinkOamLoop.h"
// #include "MoLinkOamPeer.h"
// #include "MoLldp.h"
// #include "MoLldpCommon.h"
// #include "MoLldpRemote.h"
// #include "MoMeter.h"
// #include "MoMom.h"
// #include "MoNtp.h"
// #include "MoPcpWriteProfile.h"
// #include "MoPfc.h"
#include "MoPipeMode.h"
#include "MoSyncE.h"
// #include "MoPse.h"
// #include "MoQosCommon.h"
// #include "MoRfDebug.h"
// #include "MoRollback.h"
// #include "MoRoute.h"
// #include "MoRoute6.h"
// #include "MoRulesBridgePort.h"
// #include "MoRulesBridgeVlanRule.h"
// #include "MoScript.h"
// #include "MoSnmpUser.h"
// #include "MoSyslog.h"
// #include "MoUser.h"
// #include "MoWred.h"
// #include "SnmpDb.h"

// bool* DebugModeP( );

//////////////////////////////////////////////////////////

// void CreateDefaultDatabase (bool firstTime)
// {
//     MoLicense::CreateAll( );
//     MoSystem::Create( );
//     MoAaa::Create( );
//     MoDhcpV4::Create( );
//     MoDhcpV6::Create( );
//     MoInventory::CreateAll( );
//     MoNpu::Create( );
//     MoScript::Create( );
//     MoSw::Create( );
//     MoSfs::Create( );
//     MoEncryption::Create( );
//     MoNtp::Create( );
//     MoRollback::Create( );
//     MoModulation::CreateDefault( );
//     MoRf::Create( );
//     MoRfDebug::Create( );
//     MoEthernet::Create(EthNum::Host( ));
//     for ( auto it = BeginEthNumPhy( ); it.IsValid( ); it++ )
//         MoEthernet::Create(*it);
//     MoPortDiag::CreateAll( );
//     MoUser::LoadUsersList( );
//     MoSnmpAgent::Create( );
//     MoEms::CreateAll( );
//     MoPse::CreateAll( );
//     MoLldp::CreateAll( );
//     MoLldpCommon::CreateAll( );
//     MoEventConfig::CreateAll( );
//     MoLinkOam::CreateAll( );
//     MoLinkOamLoop::CreateAll( );
//     MoLinkOamPeer::CreateAll( );
//     MoLinkBackup::Create( );
//     MoCfmMd::Create( );
//     MoBridgeCommon::Create( );
//     MoRulesBridgePort::CreateAll( );
//     if ( firstTime ) {
//         MoIp* ipP;

// #ifdef UKRAINE
//         MoIp::_CreateMo(&ipP, 1, E_IP_MODE_STATIC, IpAddress("10.70.126.32"));

// #else   // #ifdef UKRAINE
//         UINT32 sectorIndex = 1;

//         std::optional<std::string> opt = Eeprom( )->ReadKeyValue("sector-index");
//         if ( opt ) {
//             std::string val = opt.value( );
//             size_t      pos;
//             int         intVal = stoi(val, &pos);
//             if ( pos > 0 ) {
//                 if ( intVal >= 1 && intVal <= 4 )
//                     sectorIndex = intVal;
//             }
//         }

//         MyStringT<16> ipStr;
//         ipStr << "192.168.0." << sectorIndex;

//         MoIp::_CreateMo(&ipP, 1, E_IP_MODE_STATIC, IpAddress(ipStr.AsStr( )));
// #endif  // #ifdef UKRAINE

//         ipP->_valueVlan.SetUint32(0);
//     }

//     MoQosCommon::CreateAll( );
//     MoEgressQos::CreateAll( );
//     MoD6clientOpts::CreateAll( );
//     MoIeee1588::CreateAll( );
//     MoCertificate::CreateAll( );
//     MoPipeMode::CreateAll( );

//     if ( firstTime ) {
//         MoClassifierCos::CreateAllDefault( );
//         MoIngressQos::CreateAllDefault( );
//         MoAccessList::CreateAllDefault( );
//     }
// }

//////////////////////////////////////////////////////////

CmdType          Mo::_cmdType;
int              Mo::_numOfReadersId;
CliReaderIdBase* Mo::_readersId[10];

//////////////////////////////////////////////////////////

// If there is no case for some 'moType',
// Mo::BuildNew() returns NULL and does NOT create MO.
// This is the way to avoid MO registration - don't create it.
Mo* Mo::BuildNew(T_MoType moType, void* extraP)
{
    (void)extraP;

    PlaceGlobal place;

    switch ( moType ) {
        // case MO_SYSTEM: return new (place) MoSystem( );
        // case MO_ETHERNET: return new (place) MoEthernet( );
        // case MO_PORT_DIAG: return new (place) MoPortDiag( );
        // case MO_FDB_ENTRY: return new (place) MoFdbEntry( );
        // case MO_IP: return new (place) MoIp( );
        // case MO_IPV6: return new (place) MoIpV6( );
        // case MO_IPV6_DNS: return new (place) MoIpV6Dns( );
        // case MO_EMS: return new (place) MoEms( );
        // case MO_DHCLIENT6_OPTIONS: return new (place) MoD6clientOpts( );
        // case MO_RF: return new (place) MoRf( );
        // case MO_MODULATION: return new (place) MoModulation( );
        // case MO_RF_DEBUG: return new (place) MoRfDebug( );
        // case MO_BRIDGE_PORT: return Sys( )->IsModelRules( ) ? new (place) MoRulesBridgePort( ) : nullptr;
        // case MO_ARP: return new (place) MoArp( );
        // case MO_DEBUG_LOG: return new (place) MoDebugLog( );
        // case MO_SW: return new (place) MoSw( );
        // case MO_SFS: return new (place) MoSfs( );
        // case MO_SCRIPT: return new (place) MoScript( );
        // case MO_ROLLBACK: return new (place) MoRollback( );
        // case MO_NPU: return new (place) MoNpu( );
        // case MO_SNMP_MANAGER: return new (place) MoSnmpManager( );
        // case MO_SNMP_AGENT: return new (place) MoSnmpAgent( );
        // case MO_ALARM: return new (place) MoAlarm( );
        // case MO_USER: return new (place) MoUser( );
        // case MO_INVENTORY: return new (place) MoInventory( );
        // case MO_CFM_MD: return new (place) MoCfmMd( );
        // case MO_INGRESS_QOS: return new (place) MoIngressQos( );
        // case MO_EGRESS_QOS: return new (place) MoEgressQos( );
        // case MO_LICENSE: return new (place) MoLicense( );
        // case MO_ERP: return new (place) MoErp( );
        // case MO_QOS_COMMON: return new (place) MoQosCommon( );
        // case MO_ENCRYPTION: return Sys( )->IsEncryptRfSupport( ) ? (new (place) MoEncryption( )) : nullptr;
        // case MO_NTP: return new (place) MoNtp( );
        // case MO_ROUTE: return new (place) MoRoute( );
        // case MO_ROUTE6: return new (place) MoRoute6( );
        // case MO_SNMP_USER: return new (place) MoSnmpUser( );
        // case MO_LINK_OAM: return new (place) MoLinkOam( );
        // case MO_LINK_OAM_LOOP: return new (place) MoLinkOamLoop( );
        // case MO_LINK_OAM_PEER: return new (place) MoLinkOamPeer( );
        // case MO_LINK_BACKUP: return new (place) MoLinkBackup( );
        // case MO_CLASSIFIER_EVC: return new (place) MoClassifierEvc( );
        // case MO_CLASSIFIER_COS: return new (place) MoClassifierCos( );
        // case MO_METER: return new (place) MoMeter( );
        // case MO_EVENT_CONFIG: return new (place) MoEventConfig( );
        // case MO_LLDP: return new (place) MoLldp( );
        // case MO_LLDP_COMMON: return new (place) MoLldpCommon( );
        // case MO_LLDP_REMOTE: return new (place) MoLldpRemote( );
        // case MO_ACCESS_LIST: return new (place) MoAccessList( );
        // case MO_PASSWORD_STRENGTH: return new (place) MoPasswordStrength( );
        // case MO_BRIDGE_COMMON: return new (place) MoBridgeCommon( );
        // case MO_WRED: return new (place) MoWred( );
        // case MO_AAA_SERVER: return new (place) MoAaaServer( );
        // case MO_DHCP_V4: return new (place) MoDhcpV4( );
        // case MO_DHCP_V6: return new (place) MoDhcpV6( );
        // case MO_PCP_WRITE_PROFILE: return new (place) MoPcpWriteProfile( );
        // case MO_PFC: return new (place) MoPfc( );
        // case MO_SYSLOG: return new (place) MoSyslog( );
        // case MO_ACM_OAM: return new (place) MoAcmOam( );
        // case MO_IEEE1588: return Sys( )->IsSynceIeee1588Supported( ) ? (new (place) MoIeee1588( )) : nullptr;
        // case MO_MOM: return new (place) MoMom( );
        // case MO_CERTIFICATE: return new (place) MoCertificate( );
        // case MO_AAA: return new (place) MoAaa( );
        // case MO_PSE: return Pse( )->IsSupported( ) ? (new (place) MoPse( )) : nullptr;
        // case MO_LAG: return new (place) MoLag( );

//        case MO_PIPE_MODE: return new (place) MoPipeMode( );
        case MO_SYNCE: return new (place) MoSyncE( );
        default: return nullptr;
    };
}

//////////////////////////////////////////////////////////

// void BuildAllScript (MyOutStream* outP)
// {
//     char* keepP = outP->GetNameValSeparator( );
//     outP->SetNameValSeparator(" ");

//     if ( isVisibleMo(MO_LICENSE) )
//         MoLicense::BuildScript(outP);
//     if ( isVisibleMo(MO_SYSLOG) )
//         MoSyslog::BuildScript(outP);
//     if ( isVisibleMo(MO_DHCP_V4) )
//         MoDhcpV4::BuildScript(outP);
//     if ( isVisibleMo(MO_DHCP_V6) )
//         MoDhcpV6::BuildScript(outP);
//     if ( isVisibleMo(MO_PASSWORD_STRENGTH) )
//         MoPasswordStrength::BuildScript(outP);
//     if ( isVisibleMo(MO_PFC) )
//         MoPfc::BuildScript(outP);
//     if ( isVisibleMo(MO_MODULATION) )
//         MoModulation::BuildScript(outP);
//     if ( isVisibleMo(MO_PCP_WRITE_PROFILE) )
//         MoPcpWriteProfile::BuildScript(outP);
//     if ( isVisibleMo(MO_RF) )
//         MoRf::BuildScript(outP);
//     if ( isVisibleMo(MO_ETHERNET) )
//         MoEthernet::BuildScript(outP);
//     if ( isVisibleMo(MO_LAG) )
//         MoLag::BuildScript(outP);
//     if ( isVisibleMo(MO_WRED) )
//         MoWred::BuildScript(outP);
//     if ( isVisibleMo(MO_BRIDGE_COMMON) )
//         MoBridgeCommon::BuildScript(outP);
//     if ( isVisibleMo(MO_BRIDGE_PORT) )
//         MoRulesBridgePort::BuildScript(outP);
//     if ( isVisibleMo(MO_DHCLIENT6_OPTIONS) )
//         MoD6clientOpts::BuildScript(outP);
//     if ( isVisibleMo(MO_IP) )
//         MoIp::BuildScript(outP);
//     if ( isVisibleMo(MO_IPV6) )
//         MoIpV6::BuildScript(outP);
//     if ( isVisibleMo(MO_ROUTE) )
//         MoRoute::BuildScript(outP);
//     if ( isVisibleMo(MO_ROUTE6) )
//         MoRoute6::BuildScript(outP);
//     if ( isVisibleMo(MO_SYSTEM) )
//         MoSystem::BuildScript(outP);
//     if ( isVisibleMo(MO_FDB_ENTRY) )
//         MoFdbEntry::BuildScript(outP);
//     if ( isVisibleMo(MO_ARP) )
//         MoArp::BuildScript(outP);
//     if ( isVisibleMo(MO_SNMP_AGENT) )
//         MoSnmpAgent::BuildScript(outP);
//     if ( isVisibleMo(MO_SNMP_MANAGER) )
//         MoSnmpManager::BuildScript(outP);
//     if ( isVisibleMo(MO_ENCRYPTION) )
//         MoEncryption::BuildScript(outP);
//     if ( isVisibleMo(MO_NTP) )
//         MoNtp::BuildScript(outP);
//     if ( isVisibleMo(MO_AAA_SERVER) )
//         MoAaaServer::BuildScript(outP);
//     if ( isVisibleMo(MO_CFM_MD) )
//         MoCfmMd::BuildScript(outP);
//     {
//         if ( isVisibleMo(MO_QOS_COMMON) )
//             MoQosCommon::BuildScript(outP);
//     }
//     if ( isVisibleMo(MO_CLASSIFIER_EVC) )
//         MoClassifierEvc::BuildScript(outP);
//     {
//         if ( isVisibleMo(MO_CLASSIFIER_COS) )
//             MoClassifierCos::BuildScript(outP);
//     }
//     if ( isVisibleMo(MO_METER) )
//         MoMeter::BuildScript(outP);
//     if ( isVisibleMo(MO_INGRESS_QOS) )
//         MoIngressQos::BuildScript(outP);
//     {
//         if ( isVisibleMo(MO_EGRESS_QOS) )
//             MoEgressQos::BuildScript(outP);
//     }
//     if ( isVisibleMo(MO_LLDP) )
//         MoLldp::BuildScript(outP);
//     if ( isVisibleMo(MO_ERP) )
//         MoErp::BuildScript(outP);
//     if ( isVisibleMo(MO_LINK_OAM) )
//         MoLinkOam::BuildScript(outP);
//     if ( isVisibleMo(MO_LINK_OAM_LOOP) )
//         MoLinkOamLoop::BuildScript(outP);
//     if ( isVisibleMo(MO_LINK_BACKUP) )
//         MoLinkBackup::BuildScript(outP);
//     if ( isVisibleMo(MO_EVENT_CONFIG) )
//         MoEventConfig::BuildScript(outP);
//     if ( isVisibleMo(MO_ACCESS_LIST) )
//         MoAccessList::BuildScript(outP);
//     if ( isVisibleMo(MO_ACM_OAM) )
//         MoAcmOam::BuildScript(outP);
//     if ( isVisibleMo(MO_IEEE1588) )
//         MoIeee1588::BuildScript(outP);
//     if ( isVisibleMo(MO_AAA) )
//         MoAaa::BuildScript(outP);
//     if ( isVisibleMo(MO_PSE) )
//         MoPse::BuildScript(outP);
//     if ( isVisibleMo(MO_PIPE_MODE) )
//         MoPipeMode::BuildScript(outP);

//     *outP << "\n";

//     outP->SetNameValSeparator(keepP);
// }

//////////////////////////////////////////////////////////

// void BuildAllDebugScript (MyOutStream* outP)
// {
//     char* keepP = outP->GetNameValSeparator( );
//     outP->SetNameValSeparator(" ");

//     *outP << "debug on\n";

//     MoDebugLog::BuildScript(outP);
//     MoRfDebug::BuildScript(outP);

//     *outP << "\ndebug off\n";
//     *outP << "\n";

//     outP->SetNameValSeparator(keepP);
// }

//////////////////////////////////////////////////////////

void Mo::RegisterCreatable( )
{
    _properties._asBits.isCreatable = true;  // used in sample only
}

bool Mo::IsCreatable( )
{
    return SampleMo(Type( ))->_properties._asBits.isCreatable;
}

void Mo::DoHandleClear(ActionType cmdType)
{
    (void)cmdType;
    ocli << "clear operation is not supported for " << TypeName( ) << "\n";
}

void Mo::DoHandleSet(ActionType cmdType)
{
    (void)cmdType;
    ocli << "set operation is not supported for " << TypeName( ) << "\n";
}

int Mo::NumOf(T_MoType moType)
{
    int     numOfTotal = 0;
    ValueMo valueMo(moType);
    for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) )
        numOfTotal++;
    return numOfTotal;
}

bool Mo::Has(T_MoType moType)
{
    ValueMo valueMo(moType);
    valueMo.SetFirst( );
    return valueMo.IsValid( );
}

bool Mo::CopySampleTo(Mo* dstP, AttrListSet* attrListP)
{
    if ( cliP->IsError( ) || dstP == nullptr )
        return false;

    Mo* sampleP = SampleMo(dstP->Type( ));

    for ( UINT32 i = 0; i < attrListP->_numOf; i++ ) {
        if ( !attrListP->IsUsed(i) )
            continue;

        AttrId attrId = attrListP->GetAttr(i);
        if ( sampleP->FindAttr(attrId)->GetNameP( ) == nullptr ) {
            cliP->ReturnErrorAndPrint(E_RetStatus::Parsing);
            return false;
        }

        dstP->Touch( );

        // if ( dstP->Type( ) == MO_SYSTEM && attrId == ATTR_TIME )
        //     ((MoSystem*)dstP)->_valueTime._configValue = ((MoSystem*)sampleP)->_valueTime._configValue;
        // else if ( dstP->Type( ) == MO_SYSTEM && attrId == ATTR_DATE )
        //     ((MoSystem*)dstP)->_valueDate._configValue = ((MoSystem*)sampleP)->_valueDate._configValue;
        // else 
        {
            MyStringT<MAX_CLI_INPUT_LEN> strVal;
            strVal << FOCUS(sampleP) << VAL(attrId);
            Value*    valueP = dstP->AttrValue(attrId);
            RetStatus result = valueP->SetString(strVal.AsStr( ));
            if ( !result.ok( ) ) {
                cliP->ReturnErrorAndPrint(result);
                return false;
            }
        }
    }

    RetStatus result = dstP->DoPerform(CONFIRM_CHANGE, nullptr);
    if ( result.ok( ) )
        ocli << "\nSet done: " << dstP;
    else
        cliP->ReturnErrorAndPrint(result);
    return result.ok( );
}

Mo::Mo( )
{
    _properties._asInt           = 0;
    _transaction._asInt          = 0;
    _transaction._asBits.created = true;

    // ValueTimeSecs valueTime;
    // valueTime.UpdateBaseTime( );
    // _elapsedBaseTimeSecs = valueTime.GetBaseTime( );
}

T_MoType Mo::Type( ) const
{
    T_MoType moType;
    const_cast<Mo*>(this)->DoPerform(GET_MO_TYPE, &moType);
    return moType;
}

char* Mo::TypeName( )
{
    static MyStringT<32> names[MO_MAX];
    MyString*            nameP = &names[Type( )];
    if ( nameP->GetLen( ) == 0 )
        DoPerform(PRINT_TYPE_NAME, nameP);
    return nameP->AsStr( );
}

void Mo::MarkToDelete( )
{
    _transaction._asBits.deleted = true;
}

void Mo::MarkToDeleteForced( )
{
    _transaction._asBits.forced = true;
}

T_MoActionStatus Mo::Action( )
{
    if ( _transaction._asBits.deleted )
        return ACTION_DELETED;
    if ( _transaction._asBits.created )
        return ACTION_CREATED;

    if ( _transaction._asBits.resetStatistics )
        return ACTION_MODIFIED;

    if ( _properties._asBits.supportModified )
        return _transaction._asBits.modified ? ACTION_MODIFIED : ACTION_NONE;

    for ( Mo::IteratorAttr it = BeginAttr( ); it.IsValid( ); it++ ) {
        Value* valueP = it->ValueP(this);
        if ( valueP->IsDataChanged( ) )
            return ACTION_MODIFIED;
    }

    return ACTION_NONE;
}

MoId Mo::GetNumber( )
{
    MoId moNumber;
    DoPerform(GET_MO_NUMBER, &moNumber);
    return moNumber;
}

Value* Mo::AttrValue(AttrId attrId) const
{
    IteratorAttr it = FindAttr(attrId);

    if ( !it.IsValid( ) ) {
        ocli << "\n!!! ERROR Mo::AttrValue() - " << this << " attrType " << attrId._attrType << "\n";
    }

    return it->ValueP(this);
}

Value* Mo::AttrDescriptor::GetInvalidValue( )
{
    static ValueLong stub;
    return &stub;
}

UINT32 Mo::AttrUint32(AttrId attrId) const
{
    return AttrValue(attrId)->GetUint32( );
}

bool Mo::AttrWasChanged(const AttrId& attrId)
{
    Value* pVal = AttrValue(attrId);
    return pVal ? pVal->IsDataChanged( ) : false;
}

bool Mo::AttrWasChanged(const std::vector<AttrId>& attrId)
{
    return std::any_of(attrId.begin( ), attrId.end( ), [this] (const AttrId& id) {
        return AttrWasChanged(id);
    });
}

bool Mo::WasStatsClear( )
{
    return _transaction._asBits.resetStatistics;
}

void Mo::UpdateDynamic( )
{
    DoPerform(UPDATE_DYNAMIC, nullptr);
}

// void Mo::ResetStatistics( )
// {
//     CountersLine* lineP = GetCountersLine( );
//     if ( lineP == nullptr )
//         return;

//     Touch( );

//     _transaction._asBits.resetStatistics = true;

//     ValueTimeSecs valueTime;
//     valueTime.UpdateBaseTime( );
//     _elapsedBaseTimeSecs = valueTime.GetBaseTime( );

//     CountersBucket* bucketP = GetCountersBucket( );
//     if ( bucketP != nullptr )
//         bucketP->ResetStatistics(this);
// }

void Mo::KeepAllData( )
{
    for ( Mo::IteratorAttr it = BeginAttr( ); it.IsValid( ); it++ ) {
        it->ValueP(this)->DoDataControl(DATA_KEEP);
    }
}

void Mo::TransactionEndRestore( )
{
    bool wasModified = true;
    if ( _properties._asBits.supportModified )
        wasModified = _transaction._asBits.modified;

    if ( wasModified ) {
        for ( Mo::IteratorAttr it = BeginAttr( ); it.IsValid( ); it++ ) {
            it->ValueP(this)->DoDataControl(DATA_RESTORE);
        }
    }
    _transaction._asInt = 0;
}

void Mo::TransactionEndConfig( )
{
    bool wasModified = true;
    if ( _properties._asBits.supportModified )
        wasModified = (_transaction._asBits.modified || _transaction._asBits.created);

    if ( wasModified )
        KeepAllData( );
    _transaction._asInt = 0;
}

MyOutStream& operator<< (MyOutStream& dstR, const Mo* srcP)
{
    ((Mo*)srcP)->DoPerform(PRINT_FULL_NAME, &dstR);
    return dstR;
}

//////////////////////////////////////////////////////////

MyOutStream& operator<< (MyOutStream& dstR, FOCUS aFocus)
{
    dstR.SetMoFocus(aFocus._moFocusP);
    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, NAME aName)
{
    Mo*         moP   = dstR.GetMoFocus( );
    const char* nameP = moP->FindAttr(aName._attrId)->GetNameP( );

    if ( aName._width < 0 )
        dstR.Print(nameP);
    else {
        MyStringT<16> nameFormat;
        nameFormat << "%%-" << (aName._width + 2) << "s";
        dstR.Print(nameFormat.AsStr( ), nameP);
    }

    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, VAL aValue)
{
    Mo*      moP    = dstR.GetMoFocus( );
    T_MoType moType = moP->Type( );
    (void)moType;

    Value* valueP = moP->AttrValue(aValue._attrId);

    if ( aValue._width < 0 )
        dstR << valueP;
    else {
        MyStringT<16> valueFormat;
        valueFormat << "%%-" << (aValue._width + 2) << "s";

        MyStringT<128> valueBody;
        valueBody << valueP;
        dstR.Print(valueFormat.AsStr( ), valueBody.AsStr( ));
    }
    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, FIELD aField)
{
    switch ( aField._dataType ) {
        case FIELD::DATA_STRING:
            if ( aField._width < 0 )
                dstR << aField._data._stringP;
            else {
                MyStringT<16> valueFormat;
                valueFormat << "%%-" << (aField._width + 2) << "s";
                dstR.Print(valueFormat.AsStr( ), aField._data._stringP);
            }
            break;
        case FIELD::DATA_INT:
            if ( aField._width < 0 )
                dstR << aField._data._uint64;
            else {
                MyStringT<16> valueFormat;
                valueFormat << "%%-" << (aField._width + 2) << "lld";
                dstR.Print(valueFormat.AsStr( ), aField._data._uint64);
            }
            break;
        case FIELD::DATA_VALUE:
            if ( aField._width < 0 )
                dstR << aField._data._valueP;
            else {
                MyStringT<16> valueFormat;
                valueFormat << "%%-" << (aField._width + 2) << "s";

                MyStringT<128> valueBody;
                valueBody << aField._data._valueP;
                dstR.Print(valueFormat.AsStr( ), valueBody.AsStr( ));
            }
            break;
        default: dstR << "??????????????"; break;
    };

    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, NAME_VAL aNameValue)
{
    dstR << NAME(aNameValue._attrId, aNameValue._width) << dstR.GetNameValSeparator( ) << VAL(aNameValue._attrId);
    return dstR;
}

MyOutStream& operator<< (MyOutStream& dstR, NAME_VAL_FILTERED aNameValue)
{
    Mo* moP = dstR.GetMoFocus( );
    if ( moP->FindAttr(aNameValue._attrId)->IsSettable( ) )
        dstR << NAME(aNameValue._attrId, aNameValue._width) << dstR.GetNameValSeparator( ) << VAL(aNameValue._attrId);
    return dstR;
}

////////////////////////////////////////////////////

static MyStringT<64> Tag (MyStringT<64> helpName)
{
    MyStringT<64> helpNameWithTag;
    helpNameWithTag << "<" << helpName << ">";
    return helpNameWithTag;
}

void Mo::Cli_PrintStandardHelpShow(CliModifierShow* m)
{
    ocli << "show " << TypeName( ) << " ";

    for ( int i = 0; i < _numOfReadersId; i++ )
        ocli << "[" << Tag(_readersId[i]->HelpName(Type( ))) << " ";
    ocli << "[<modifier>]";
    for ( int i = 0; i < _numOfReadersId; i++ )
        ocli << "]";

    for ( int i = 0; i < _numOfReadersId; i++ ) {
        ocli << "\n    " << FIELD(Tag(_readersId[i]->HelpName(Type( ))).AsStr( ), 20) << " : ";
        _readersId[i]->PrintHelp( );
    }
    ocli << "\n    " << FIELD("<modifier>", 20) << " : ";
    m->PrintHelp( );

    if ( (GetCountersLine( ) != nullptr) && m->_hasHistory )
        ocli << "\nstatistics have optional history range parameters: <min> [<max>], where <min>,<max> are 0..95";
}

// clear
void Mo::Cli_PrintStandardHelpClear( )
{
    ocli << "clear " << TypeName( ) << " ";

    for ( int i = 0; i < _numOfReadersId; i++ )
        ocli << Tag(_readersId[i]->HelpName(Type( ))) << " ";
    if ( SampleMo(Type( ))->GetCountersLine( ) )
        ocli << " statistics";

    for ( int i = 0; i < _numOfReadersId; i++ ) {
        ocli << "\n    " << FIELD(Tag(_readersId[i]->HelpName(Type( ))).AsStr( ), 20) << " : ";
        _readersId[i]->PrintHelp( );
    }
}

// set
void Mo::Cli_PrintStandardHelpSet(AttrListSet* attrListP)
{
    ocli << "set " << TypeName( ) << " ";

    for ( int i = 0; i < _numOfReadersId; i++ )
        ocli << Tag(_readersId[i]->HelpName(Type( ))) << " ";

    // print attributes list
    for ( int i = 0; i < (int)attrListP->_numOf; i++ ) {
        AttrId attrId = attrListP->GetAttr(i);
        if ( attrListP->IsMandat(i) )
            ocli << " " << SampleMo(Type( ))->FindAttr(attrId)->GetNameP( ) << " "
                 << "<value>";
        else
            ocli << " [" << SampleMo(Type( ))->FindAttr(attrId)->GetNameP( ) << " "
                 << "<value>"
                 << "]";
    }

    for ( int i = 0; i < _numOfReadersId; i++ ) {
        ocli << "\n    " << FIELD((_readersId[i]->HelpName(Type( ))).AsStr( ), 20) << " : ";
        _readersId[i]->PrintHelp( );
    }
}

void Mo::Cli_PrintStandardShow(CliModifierShow* modifierP, AttrArray* infoTableAttrsP)
{
    LOG("entry");
    // if ( modifierP->IsStats( ) || modifierP->IsStatsSummary( ) )
    //     Cli_PrintStandardShowStatistics(modifierP);
    // else 
    if ( infoTableAttrsP != nullptr )
        Cli_PrintStandardShowInfoAsTable(infoTableAttrsP);
    else
        Cli_PrintStandardShowInfoAsList(modifierP);
    LOG("exit");
}

void Mo::Cli_PrintStandardShowInfoAsList(CliModifierShow* modifierP)
{
    LOG("entry");
    ValueMoMatchCli valueMo(Type( ));

    for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
        LOG("loop-1");
        Mo* moP = valueMo.MoP( );
        moP->UpdateDynamic( );
        ocli << FOCUS(moP);
        ocli << "\n";

        for ( UINT32 i = 0; i < modifierP->_info._numOf; i++ ) {
            LOG("loop-2");
            AttrId attrId = modifierP->_info._array[i]._attrId;
            if ( modifierP->IsInfo( ) || (modifierP->GetAttr( ) == attrId) ) {
                ocli << moP << " " << NAME(attrId, CliNameColumnLen) << ocli.GetNameValSeparator( ) << VAL(attrId) << "\n";
            }
        }
    }
    LOG("exit");
}

void Mo::Cli_PrintStandardShowInfoAsTable(AttrArray* infoTableAttrsP)
{
    LOG("entry");
    ocli << FOCUS(SampleMo(Type( ))) << "\n";
    for ( UINT32 i = 0; i < infoTableAttrsP->_numOf; i++ ) {
        if ( infoTableAttrsP->_array[i]._length == 0 ) {
            int len                            = strlen(SampleMo(Type( ))->FindAttr(infoTableAttrsP->_array[i]._attrId)->GetNameP( ));
            infoTableAttrsP->_array[i]._length = len;
        }
        ocli << NAME(infoTableAttrsP->_array[i]._attrId, infoTableAttrsP->_array[i]._length);
    }
    ocli << "\n";

    ValueMoMatchCli valueMo(Type( ));

    for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
        Mo* moP = valueMo.MoP( );
        moP->UpdateDynamic( );
        ocli << FOCUS(moP);

        for ( UINT32 i = 0; i < infoTableAttrsP->_numOf; i++ ) {
            AttrId attrId = infoTableAttrsP->_array[i]._attrId;
            int    width  = infoTableAttrsP->_array[i]._length;

            Value* valueP = moP->AttrValue(attrId);

            if ( width < 0 )
                ocli << valueP;
            else {
                MyStringT<16> valueFormat;
                valueFormat << "%%-" << (width + 2) << "s";

                MyStringT<128> valueBody;
                valueBody << valueP;
                ocli.Print(valueFormat.AsStr( ), valueBody.AsStr( ));
            }
        }
        ocli << "\n";
    }
    LOG("exit");
}

// void Mo::Cli_PrintStandardShowStatistics(CliModifierShow* modifierP)
// {
//     bool isHistory = ((modifierP->_minHistory != 0) || (modifierP->_maxHistory != 0));

//     if ( modifierP->IsStats( ) )
//         SampleMo(Type( ))->GetCountersLine( )->PrintTableHead(isHistory);
//     else
//         SampleMo(Type( ))->GetCountersLine( )->PrintSummaryHead( );

//     ValueMoMatchCli valueMo(Type( ));

//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         Mo* moP = valueMo.MoP( );
//         moP->UpdateDynamic( );
//         ocli << FOCUS(moP);

//         moP->GetCountersLine( )->Update(moP, modifierP->_historyGiven);  // if history is given, update using interval not global (for min-max statistics)

//         for ( UINT32 i = modifierP->_minHistory; i <= modifierP->_maxHistory; i++ ) {
//             CountersLine* lineP = nullptr;

//             if ( i == 0 ) {
//                 lineP = moP->GetCountersLine( );
//             } else if ( moP->GetCountersBucket( ) != nullptr ) {
//                 lineP = moP->GetCountersBucket( )->GetLine(i);
//             }

//             if ( lineP == nullptr )
//                 continue;

//             if ( modifierP->IsStats( ) ) {
//                 lineP->PrintLine(i, isHistory, &ocli, moP);
//             } else {
//                 lineP->PrintSummaryLine(i, isHistory, moP);
//             }
//         }
//     }
// }

////////////////////////////////////////////////////

// #include "MyDump.h"

// void Mo::DoSampleXmlShow(Xml* xmlP, MyString* instancesP)
// {
//     (void)instancesP;

//     if ( xmlP->_action == XML_SHOW_HELP_MO ) {
//         xmlP->TagMo(TypeName( ), "sample");  // of sample

//         for ( Mo::IteratorAttr it = BeginAttr( ); it.IsValid( ); it++ ) {
//             xmlP->Print("<attr name=\"%s\">", FindAttr(it->__Id( ))->GetNameP( ));
//             Value* valueP = it->ValueP(this);
//             valueP->DoGetXmlHelp(xmlP);
//             xmlP->Print("</attr>\n");
//         }
//         xmlP->TagMoEnd( );
//         return;
//     }

//     ValueMo valueMo(Type( ));
//     for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
//         Mo* moP = valueMo.MoP( );

//         if ( moP->_properties._asBits.internalOnly )
//             continue;

//         if ( instancesP->GetLen( ) != 0 ) {
//             MyStringT<128> current;
//             current << moP;
//             if ( *instancesP != current )
//                 continue;
//         }

//         xmlP->TagMo(moP);

//         if ( xmlP->_action == XML_SHOW_INFO )
//             moP->UpdateDynamic( );

//         if ( (xmlP->_action == XML_SHOW_INFO) || (xmlP->_action == XML_SHOW_STATS_HISTORY) ) {
//             for ( Mo::IteratorAttr it = BeginAttr( ); it.IsValid( ); it++ )
//                 xmlP->TagAttr(moP, it->__Id( )._attrType);

//             CountersLine* lineP = moP->GetCountersLine( );
//             if ( lineP != nullptr ) {
//                 lineP->BuildXmlHead(xmlP->_outP);
//                 lineP->BuildXmlLine(xmlP->_outP);
//             }
//             if ( xmlP->_action == XML_SHOW_STATS_HISTORY ) {
//                 CountersBucket* bucketP = moP->GetCountersBucket( );
//                 if ( bucketP != nullptr ) {
//                     for ( UINT32 i = 1; i <= 95; i++ ) {
//                         CountersLine* lineP = bucketP->GetLine(i);
//                         if ( lineP != nullptr )
//                             lineP->BuildXmlHistLine(xmlP->_outP);
//                     }
//                 }
//             }
//         }

//         xmlP->TagMoEnd( );
//     }
// }

////////////////////////////////////////////////////
