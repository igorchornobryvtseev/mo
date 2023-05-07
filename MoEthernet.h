#pragma once

#include "Mo.h"
#include "MoType.h"
#include "Value.h"

class MoEthernet;
class MoLag;

///////////////////////////////////////////////////////
#if 0
struct ValueEtherTypeConfigDualMedia : public Value {
    DICTIONARY_7(EtherTypes_copper, E_EthernetType::Type100hd, "100hd", E_EthernetType::Type100fd, "100fd", E_EthernetType::Type1000hd, "1000hd", E_EthernetType::Type1000fd, "1000fd",
        E_EthernetType::Type2500fd, "2500fd", E_EthernetType::Type5000fd, "5000fd", E_EthernetType::Type10000fd, "10000fd");
    ValueDictT<EtherTypes_copper, static_cast<uint32_t>(E_EthernetType::Type10000fd)> _valueCopper;

    DICTIONARY_2(EtherTypes_sfp, E_EthernetType::Type1000xfd, "1000xfd", E_EthernetType::Type10000xfd, "10000xfd");
    ValueDictT<EtherTypes_sfp, static_cast<uint32_t>(E_EthernetType::Type10000xfd)> _valueSfp;

    UINT32 DoDataControl(T_DataControl dataControl) override;

    void DoGetCli (MyOutStream* outP) override
    {
        *outP << "rj45 " << _valueCopper << " , "
              << " sfp " << _valueSfp;
    }

    void DoSetCli(WordReader* inP) override;

    bool IsWordInCliPartOfList ( ) override { return true; }

    E_EthernetType _GetType (UINT32 mauIndex)
    {
        if ( mauIndex == 0 )
            return (E_EthernetType)_valueCopper.GetUint32( );
        if ( mauIndex == 1 )
            return (E_EthernetType)_valueSfp.GetUint32( );
        return E_EthernetType::Illegal;
    }

    bool _SetSnmpType (UINT32 mauIndex, UINT32 value)
    {
        if ( mauIndex == 0 )
            return _valueCopper.SetUint32(value);
        if ( mauIndex == 1 )
            return _valueSfp.SetUint32(value);
        return false;
    }

    void DoGetCliHelp ( ) override
    {
        ocli << "[rj45 <value>] , [sfp <value>]\n";
        ocli << "rj45 : ";
        _valueCopper.DoGetCliHelp( );
        ocli << "\n";
        ocli << "sfp    : ";
        _valueSfp.DoGetCliHelp( );
    }
};
#endif
///////////////////////////////////////////////////////
#if 0
struct ValueEtherActualType : public ValueDictionary {
    E_EthernetType GetType ( ) { return (E_EthernetType)GetUint32( ); }

    Dictionary* GetDictionary ( ) override
    {
        static DictionaryT<20> dictAll;
        if ( dictAll.IsEmpty( ) ) {
            dictAll.Add(1, "none");
            dictAll.Add(E_EthernetType::Type10hd, "10hd");
            dictAll.Add(E_EthernetType::Type10fd, "10fd");
            dictAll.Add(E_EthernetType::Type100hd, "100hd");
            dictAll.Add(E_EthernetType::Type100fd, "100fd");
            dictAll.Add(E_EthernetType::Type1000hd, "1000hd");
            dictAll.Add(E_EthernetType::Type1000fd, "1000fd");
            dictAll.Add(E_EthernetType::Type1000xfd, "1000xfd");
            dictAll.Add(E_EthernetType::Type10000xfd, "10000xfd");
            dictAll.Add(E_EthernetType::Type2500fd, "2500fd");
            dictAll.Add(E_EthernetType::Type5000fd, "5000fd");
            dictAll.Add(E_EthernetType::Type10000fd, "10000fd");
        };

        return &dictAll;
    }
};
#endif
///////////////////////////////////////////////////////

struct DictionaryEthernet : public Dictionary {
    enum Controls { SKIP_HOST = 0x01, SKIP_ETH0 = 0x02, SKIP_LAG = 0x04, USE_LAG = 0x08, USE_NONE = 0x10 };

    DictionaryEthernet(UINT32 controls = 0) : _controls(controls) { }

    Entry ___GetEntry(UINT32 context) override;
    void  ___FirstEntry(UINT32* contextP) override;
    void  ___NextEntry(UINT32* contextP) override;

protected:
    bool IsMatch(EthNum ethNum);

    UINT32 _controls;
};

template<UINT32 DICTIONARY_CONTROLS = 0>
struct ValueEthernetPortT : public ValueDictionary {
    ValueEthernetPortT( ) = default;

    ValueEthernetPortT(EthNum ethNum) { SetEthNum(ethNum); }

    bool SetEthNum (EthNum ethNum) { return SetUint32(ethNum.AsEnum( )); }

    EthNum GetEthNum ( ) { return EthNum {(T_EthernetNum)GetUint32( )}; }

    EthNum GetOldEthNum ( ) { return EthNum {(T_EthernetNum)GetOldUint32( )}; }

    Dictionary* GetDictionary ( ) override
    {
        static DictionaryEthernet dictionary(DICTIONARY_CONTROLS);
        return &dictionary;
    }
};

using ValueEthernetPort = ValueEthernetPortT<>;
#if 0
template<UINT32 DICTIONARY_CONTROLS = 0>
struct ValueEthernetPortsListT : public ValueListOfDictionaryT<ValueEthernetPortT<DICTIONARY_CONTROLS>, 0, 31> {
    void AddEth (EthNum ethNum) { ValueListOfDictionaryT<ValueEthernetPortT<DICTIONARY_CONTROLS>, 0, 31>::SetBit(ethNum.AsEnum( ), true); }

    void DelEth (EthNum ethNum) { ValueListOfDictionaryT<ValueEthernetPortT<DICTIONARY_CONTROLS>, 0, 31>::SetBit(ethNum.AsEnum( ), false); }

    EthNum::Table<bool> GetPorts ( )
    {
        EthNum::Table<bool> ports;
        for ( auto it = BeginEthNumAnyExists( ); it.IsValid( ); it++ )
            ports[*it] = false;

        IteratorValueListOf it = ValueListOfDictionaryT<ValueEthernetPortT<DICTIONARY_CONTROLS>, 0, 31>::BeginList( );
        for ( ; it.IsValid( ); it++ ) {
            EthNum current = EthNum {(T_EthernetNum)*it};
            ports[current] = true;
        }
        return ports;
    }
};

struct ValueEthernetPortsList : public ValueEthernetPortsListT<> {
};
#endif
///////////////////////////////////////////////////////
#if 0
MoEthernet* FindEthernet(EthNum ethNum);

struct ValueRemEthPort : public ValueEthernetPortT<DictionaryEthernet::SKIP_HOST | DictionaryEthernet::SKIP_ETH0> {
};

class MoEthernet : public Mo
{
public:
    static const T_MoType MO_TYPE = MO_ETHERNET;

    static void BuildScript(MyOutStream* dstP);

    static void DownloadConfigNxp( );

    static void DownloadConfig ( ) { DownloadConfigNxp( ); }

    static void DownloadConfigAdmin( );

    static MoEthernet* Create(EthNum ethNum);
    static MoEthernet* CreateRf(uint);

    static Mac GetHostSelfMac( );

    static void SendEthernetEvents( );

    static void                               ConfigAlarmPropagation( );
    std::optional<T_MO_ETHERNET_DYNAMIC_DATA> DynamicData( );
    void                                      UploadDynamicData(T_MO_ETHERNET_DYNAMIC_DATA* dstP);

    MoEthernet( );

    EthNum GetEthNum ( ) { return _valuePortNum.GetEthNum( ); }

    bool IsRfWigig( );  // wigig rf dynamic or static

    bool IsRfGuest( );

    bool IsDoubleMauPort ( ) { return Eth( )->IsDoublePort(GetEthNum( )); }

    RetStatus DoPerform(ActionType performType, void* dataP) override;
    void      DoSnmpPerform(ActionType performType) override;
    void      DoPostConfig(T_MoActionStatus action) override;

    void SnmpPerformMau(ActionType performType);
    void SnmpPerformSfpDdm(ActionType performType);

    void DoHandleShow(ActionType cmdType) override;
    void DoHandleSet(ActionType cmdType) override;
    void DoHandleClear(ActionType cmdType) override;

    void SingleShowInfo(CliModifierShow* modifierP);

    // info
    ValueEthernetPort    _valuePortNum;
    ValueString32        _valueDescription;
    ValueLong            _valueMtuSize;
    ValueMac             _valueLocalMac;
    T_ETH_ALARM_SM_STATE _alarm_state;

    DICTIONARY_2(AdminStateTable, E_AdminState::Enable, "up", E_AdminState::Disable, "down");

    using ValueEthernetAdminState = ValueDictT<AdminStateTable>;

    ValueEthernetAdminState _valueAdminState;

    DICTIONARY_2(OperationalStateTable, E_OperationalState::Up, "up", E_OperationalState::Down, "down");
    ValueDictT<OperationalStateTable> _valueOperationalState;

    struct ValueBlockedByExtendMMState : public Value {
        ValueBlockedByExtendMMState(MoEthernet* ownerP) : _ownerP(ownerP) { }

        bool IsWordInCliPartOfList ( ) override { return false; }

        void DoSetCli (WordReader* inP) override { (void)inP; }

        void DoGetCli (MyOutStream* outP) override
        {
            static const char* _blockedStateStr[] = {"unknown", "yes", "no"};
            outP->Print("%s", _blockedStateStr[GetBlockedState( )]);
        }

    private:
        enum BlockedState {
            UNKNOWN,
            BLOCKED,
            UNBLOCKED,
        };

        BlockedState GetBlockedState( );
        MoEthernet*  _ownerP;
    };

    ValueBlockedByExtendMMState _valueBlockedByExtendMMState;

    ValueTimeSecs _valueLastChangeTime;
    ValueString32 _valueName;
    ValueString32 _valueAlias;

    class ValueLoopMode : public ValueDictionary
    {
    public:
        ValueLoopMode( ) { SetUint32(E_ETH_LOOP_DISABLED); }

        Dictionary* GetDictionary ( ) override
        {
            static DictionaryT<10> dict;
            if ( dict.IsEmpty( ) ) {
                dict.Add(E_ETH_LOOP_DISABLED, "disabled");
                dict.Add(E_ETH_LOOP_INTERNAL_MAC_SWAP, "internal-mac-swap");
            }
            return &dict;
        }
    };

    ValueLoopMode _valueLoop;

    ValueMac _valueLoopMac;

    ValueLongRangeT<0, 86400, 60> _valueLoopTimeout;

    DICTIONARY_4(MirroringModeTable, E_MIRORRING_DISABLED, "disabled", E_MIRORRING_INGRESS, "ingress", E_MIRORRING_EGRESS, "egress", E_MIRORRING_BOTH, "both");
    ValueDictT<MirroringModeTable, E_MIRORRING_DISABLED> _valueMirroringMode;

    struct ValueEthMirrorPort : public ValueEthernetPortT<DictionaryEthernet::SKIP_HOST | DictionaryEthernet::SKIP_ETH0 | DictionaryEthernet::USE_NONE> {
    };

    ValueEthMirrorPort _valueMirroringTarget;
    /*
        struct  ValueAlarmPropagation : public ValueDictionary
        {
            ValueAlarmPropagation();
            virtual Dictionary * GetDictionary();
        };
        ValueAlarmPropagation  _valueAlarmPropagation;
    */
    DICTIONARY_2(AlarmPropagationEHTable, E_AlarmPropagation::Disable, "disabled", E_AlarmPropagation::Backward, "backward");

    using ValueAlarmPropagationEH = ValueDictT<AlarmPropagationEHTable>;

    ValueAlarmPropagationEH _valueAlarmPropagation_eh;

    DICTIONARY_2(PauseFramesTable, E_PauseFrames::Disabled, to_string(E_PauseFrames::Disabled), E_PauseFrames::Enabled, to_string(E_PauseFrames::Enabled));
    ValueDictT<PauseFramesTable, static_cast<uint32_t>(E_PauseFrames::Disabled)> _valuePauseFrames;

    ValueRemEthPort _valueRemEthPort;

    struct ValuePromiscMode : public ValueSnmpOnly {
        void DoGetSnmp (SnmpVariable* outP) override { *outP << _mode; }

        int _mode;
    };

    ValuePromiscMode _valuePromiscMode;

    struct ValueConnectorPresent : public ValueSnmpOnly {
        void DoGetSnmp (SnmpVariable* outP) override { *outP << _present; }

        int _present;
    };

    ValueConnectorPresent _valueConnectorPresent;

    struct ValueIfType : public ValueSnmpOnly {
        void DoGetSnmp (SnmpVariable* outP) override { *outP << _ifType; }

        int _ifType;
    };

    ValueIfType _valueIfType;

    UINT32 GetIfIndex ( ) { return GetEthNum( ).AsEnum( ) + 1; }

    ////////////////////////////////////////////

    struct ValueEtherTypeConfig : public ValueProxy {
        bool   _isDoubleMua;
        Value* _valueP;

        DICTIONARY_6(EtherTypes_rj45, E_EthernetType::Type10hd, "10hd", E_EthernetType::Type10fd, "10fd", E_EthernetType::Type100hd, "100hd", E_EthernetType::Type100fd, "100fd",
            E_EthernetType::Type1000hd, "1000hd", E_EthernetType::Type1000fd, "1000fd");
        ValueDictT<EtherTypes_rj45, static_cast<uint32_t>(E_EthernetType::Type1000fd)> _valueSingleMua_rj45;

        DICTIONARY_2(EtherTypes_sfp, E_EthernetType::Type1000xfd, "1000xfd", E_EthernetType::Type10000xfd, "10000xfd");
        ValueDictT<EtherTypes_sfp, static_cast<uint32_t>(E_EthernetType::Type1000xfd)> _valueSingleMua_sfp;

        DICTIONARY_2(EtherTypes_sfpWigig, E_EthernetType::Type1000xfd, "1000xfd", E_EthernetType::Type2500xfd, "2500xfd");
        ValueDictT<EtherTypes_sfpWigig, static_cast<uint32_t>(E_EthernetType::Type1000xfd)> _valueSingleMua_sfpWigig;  //@todo no used? MH leftover ?

        ValueEtherTypeConfigDualMedia _valueDualMedia;

        ValueEtherTypeConfig( ) { AssignConnector(E_ETHER_CONNECTOR_RJ45); }

        E_EthernetType GetType (UINT32 mauIndex) { return _isDoubleMua ? _valueDualMedia._GetType(mauIndex) : (E_EthernetType)_valueP->GetUint32( ); }

        bool SetSnmpType (UINT32 mauIndex, UINT32 value) { return _isDoubleMua ? _valueDualMedia._SetSnmpType(mauIndex, value) : ((ValueDictionary*)_valueP)->SetUint32(value); }

        void AssignConnector (T_MO_ETHERNET_CONNECTOR_TYPE connector)
        {
            _isDoubleMua = (connector == E_ETHER_CONNECTOR_DUAL_MEDIA);

            if ( connector == E_ETHER_CONNECTOR_DUAL_MEDIA )
                _valueP = &_valueDualMedia;
            else if ( connector == E_ETHER_CONNECTOR_RJ45 )
                _valueP = &_valueSingleMua_rj45;
            else if ( connector == E_ETHER_CONNECTOR_SFP ) {
                _valueP = &_valueSingleMua_sfp;
            } else {
                _valueP = &_valueSingleMua_rj45;
            }
        }

        void CopyFrom(MoEthernet* srcP);

        Value* GetOriginalValue ( ) override { return _valueP; }
    };

    ValueEtherTypeConfig _valueEtherTypeConfig;

    ValueEtherActualType _valueActualEtherType;

    ////////////////////////////////////////////

    DICTIONARY_4(EtherConnectorType, E_ETHER_CONNECTOR_NONE, "none", E_ETHER_CONNECTOR_RJ45, "rj45", E_ETHER_CONNECTOR_SFP, "sfp", E_ETHER_CONNECTOR_DUAL_MEDIA, "dual-media");
    ValueDictT<EtherConnectorType> _valueEtherConnectorType;

    DICTIONARY_2(AlarmSuppression, E_AGENT_TRUE, "enable", E_AGENT_FALSE, "disable");
    ValueDictT<AlarmSuppression, E_AGENT_FALSE> _valueAlarmSuppression;

    DICTIONARY_2(AutoNegAdminTable, E_AutoNegState::Disable, "disabled", E_AutoNegState::Enable, "enabled");

    using ValueAutoNegAdmin = ValueDictT<AutoNegAdminTable>;

    ValueAutoNegAdmin _valueAutoNegAdmin;

    struct LoopCadTimer : public CadTimer {
        LoopCadTimer(MoEthernet* ownerP) : _ownerP(ownerP) { Register(this); }

        ~LoopCadTimer( ) override { UnRegister(this); }

        void        Action( ) override;
        MoEthernet* _ownerP;
    };

    LoopCadTimer _loopTimer;

    DICTIONARY_4(ClockModeTable, E_CLOCK_MODE_AUTO, "auto", E_CLOCK_MODE_MASTER, "master", E_CLOCK_MODE_SLAVE, "slave", E_CLOCK_MODE_SYNCE, "synce");
    ValueDictT<ClockModeTable, E_CLOCK_MODE_AUTO> _valueClockMode;

    struct ValuePcpIdOrNone : public ValueUnion {
        Value* GetCurrentValueP (UINT8 valueIndex) override
        {
            switch ( valueIndex ) {
                case 0: return &_valueNone;
                case 1: return &_pcpId;
                default: return nullptr;
            };
        }

        DICTIONARY_1(NoneWord, 0, "none");
        ValueDictT<NoneWord>   _valueNone;
        ValuePcpWriteProfileId _pcpId;
    };

    ValuePcpIdOrNone _valuePcpId;

    DICTIONARY_2(PfcModeTable, E_PFC_MODE_RF, "rf", E_PFC_MODE_DISABLE, "disable");
    ValueDictT<PfcModeTable, E_PFC_MODE_DISABLE> _valuePfcMode;

    struct ValueLagRef : public ValueLong {
        ValueLagRef( ) { UnassignLag( ); }

        void AssignLag (UINT32 lagId, bool isMaster) { SetUint32(isMaster ? lagId : (lagId * 10)); }

        void UnassignLag ( ) { SetUint32(0); }

        bool IsLag ( ) { return (GetUint32( ) != 0); }

        bool IsLagSlave ( ) { return (GetUint32( ) >= 10); }

        bool IsLagMaster ( ) { return (GetUint32( ) > 0) && (GetUint32( ) < 10); }

        // MoLag * GetLag()    { return (MoLag*) FindMo( MO_LAG, MoId(LagId())); }

        UINT32 LagId ( )
        {
            if ( (GetUint32( ) == 1) || (GetUint32( ) == 10) )
                return 1;
            if ( (GetUint32( ) == 2) || (GetUint32( ) == 20) )
                return 2;
            return 0;
        }

        char* LagName ( )
        {
            if ( (GetUint32( ) == 1) || (GetUint32( ) == 10) )
                return "lag1";
            if ( (GetUint32( ) == 2) || (GetUint32( ) == 20) )
                return "lag2";
            return "";
        }

        void DoGetCli (MyOutStream* outP) override { outP->Print(LagName( )); }
    };

    ValueLagRef _valueLagRef;

    bool IsLag ( ) { return _valueLagRef.IsLag( ); }

    template<class DICTIONARY_T, UINT32 INIT_VAL_T = (unsigned)-1>
    struct ValueDictDoublePortT : public ValueDictT<DICTIONARY_T, INIT_VAL_T> {
    public:
        void DoGetCli (MyOutStream* outP) override
        {
            if ( _inUse )
                ValueDictT<DICTIONARY_T, INIT_VAL_T>::DoGetCli(outP);
        }

        bool _inUse {true};
    };

    DICTIONARY_3(ActiveConfigTable, ETH_ACTIVE_RJ45, "rj45", ETH_ACTIVE_SFP, "sfp", ETH_ACTIVE_AUTO, "auto");

    struct ValueActiveConfig : public ValueDictDoublePortT<ActiveConfigTable, ETH_ACTIVE_AUTO> {
    };

    ValueActiveConfig _valueActiveConfig;

    DICTIONARY_2(ActiveActualTable, ETH_ACTIVE_RJ45, "rj45", ETH_ACTIVE_SFP, "sfp");

    struct ValueActiveActual : public ValueDictDoublePortT<ActiveActualTable> {
    };

    ValueActiveActual _valueActiveActual;

    ValueCounter64 _valueTxThroughput;
    ValueCounter64 _valueRxThroughput;

    T_MO_ETHERNET_DYNAMIC_DATA _dynamicData;

    // SFP DDM info
    ValueFloatNormal _valueSfpDdmTemperature;
    ValueFloatNormal _valueSfpDdmVcc;
    ValueFloatNormal _valueSfpDdmTxBias;
    ValueFloatNormal _valueSfpDdmTxPower;
    ValueFloatNormal _valueSfpDdmRxPower;

    DICTIONARY_11(SfpDdmStatusTable, ETH_SFPDDM_STATUS_OK, "OK", ETH_SFPDDM_STATUS_ALM_HIGH, "Alarm high", ETH_SFPDDM_STATUS_ALM_LOW, "Alarm low", ETH_SFPDDM_STATUS_WARN_HIGH, "Warning high",
        ETH_SFPDDM_STATUS_WARN_LOW, "Warning low", ETH_SFPDDM_STATUS_ALM_HIGH_WARN_LOW, "Alarm high, Warning low", ETH_SFPDDM_STATUS_ALM_LOW_WARN_HIGH, "Alarm low, Warning high",
        ETH_SFPDDM_STATUS_ALM_HIGH_WARN_HIGH, "Alarm high, Warning high", ETH_SFPDDM_STATUS_ALM_LOW_WARN_LOW, "Alarm low, Warning low", ETH_SFPDDM_STATUS_LINK_DOWN, "Link Down",
        ETH_SFPDDM_STATUS_NO_SFP, "No SFP");

    struct ValueSfpDdmStatus : public ValueDictDoublePortT<SfpDdmStatusTable> {
    };

    ValueSfpDdmStatus _valueSfpDdmTemperatureStatus;
    ValueSfpDdmStatus _valueSfpDdmVccStatus;
    ValueSfpDdmStatus _valueSfpDdmTxBiasStatus;
    ValueSfpDdmStatus _valueSfpDdmTxPowerStatus;
    ValueSfpDdmStatus _valueSfpDdmRxPowerStatus;

    // counters

    struct EthernetCountersLine : CountersLine {
        struct SnmpObjects {
            UINT32      _numOf {0};
            SnmpColumn* _columnP[25];
            long        _offset[25];

            void Add (SnmpColumn* columnP, long offset)
            {
                if ( _numOf >= 25 )
                    return;
                _columnP[_numOf] = columnP;
                _offset[_numOf]  = offset;
                _numOf++;
            }

            MyOid GetOid (Mo* moP, UINT32 snmpCountIndex)
            {
                MyOid fullOid;

                if ( (moP != nullptr) && snmpCountIndex < _numOf )
                    fullOid << _columnP[snmpCountIndex]->ReadColumnOid( ) << ((MoEthernet*)moP)->GetIfIndex( );

                return fullOid;
            }
        };

        static SnmpObjects _snmpObjects;

        UINT64 GetSnmpCount (UINT32 snmpCountIndex)
        {
            if ( (snmpCountIndex) >= _snmpObjects._numOf )
                return 0;
            long    offset = _snmpObjects._offset[snmpCountIndex];
            UINT64* srcP   = (UINT64*)((long)(&_isCurrent) + offset);
            return *srcP;
        }

        //////////////////////////////

        void Update(Mo* moP) override;

        void Update (Mo* moP, bool interval) override
        {
            if ( interval )
                Update(moP);
        }

        void PrintLine(int index, bool isHistory, MyOutStream* outP, Mo* moP) override;

        void BuildCsv (MyOutStream* outP, bool isHead) override
        {
            if ( isHead ) {
                *outP << "time"
                      << ", "
                      << "in-octets"
                      << ", "
                      << "in-ucast-pkts"
                      << ", "
                      << "in-discards"
                      << ", "
                      << "in-errors"
                      << ", "
                      << "out-octets"
                      << ", "
                      << "out-ucast-pkts"
                      << ", "
                      << "out-errors"
                      << ", "
                      << "in-mcast-pkts"
                      << ", "
                      << "in-bcast-pkts"
                      << ", "
                      << "out-mcast-pkts"
                      << ", "
                      << "out-bcast-pkts"
                      << ", "
                      << "out-discards"
                      << ", "
                      << "in-no-rule-discards"
                      << ", "
                      << "in-rate"
                      << ", "
                      << "out-rate"
                      << ", "
                      << "util"
                      << "\n";
                return;
            }

            ValueDateAndTime valueTime(_startAbsTime);
            ValueCounter64   valueInOctets(_data._inOctets);
            ValueCounter64   valueInUcastPkts(_data._inUcastPackets);
            ValueCounter64   valueInDiscards(_data._inDiscardedPackets);
            ValueCounter64   valueInErrors(_data._inErrors);
            ValueCounter64   valueOutOctets(_data._outOctets);
            ValueCounter64   valueUcastPkts(_data._outUcastPackets);
            ValueCounter64   valueOutErrors(_data._outErrors);
            ValueCounter64   valueInMcastPkts(_data._inMcastPackets);
            ValueCounter64   valueInBcastPkts(_data._inBcastPackets);
            ValueCounter64   valueOutMcastPkts(_data._outMcastPackets);
            ValueCounter64   valueOutBcastPkts(_data._outBcastPackets);
            ValueCounter64   valueOutDiscardedPkts(_data._outDiscardedPackets);
            ValueCounter64   valueNoRule(_data._inNoRuleDiscardPackets);
            ValueCounter64   valuenInLagDiscard(_data._inLagDiscardPackets);
            ValueCounter64   valueInRate(_data._inRate);
            ValueCounter64   valueOutRate(_data._outRate);
            ValueCounter64   valueUtil(_data._util);

            *outP << valueTime << ", " << valueInOctets << ", " << valueInUcastPkts << ", " << valueInDiscards << ", " << valueInErrors << ", " << valueOutOctets << ", " << valueUcastPkts << ", "
                  << valueOutErrors << ", " << valueInMcastPkts << ", " << valueInBcastPkts << ", " << valueOutMcastPkts << ", " << valueOutBcastPkts << ", " << valueOutDiscardedPkts << ", "
                  << valueNoRule << ", " << valueInRate << ", " << valueOutRate << ", " << valueUtil << ", " << valuenInLagDiscard << "\n";
        }

        bool HasStatsSummary ( ) override { return true; }

        void PrintSummaryHead( ) override;
        void PrintSummaryLine(int index, bool isHistory, Mo* moP) override;

        struct Data {
            INT64  _inOctets;
            INT64  _inUcastPackets;
            INT64  _inDiscardedPackets;
            INT64  _inErrors;
            INT64  _inPackets;
            INT64  _outOctets;
            INT64  _outUcastPackets;
            INT64  _outErrors;
            INT64  _inMcastPackets;
            INT64  _inBcastPackets;
            INT64  _outMcastPackets;
            INT64  _outBcastPackets;
            INT64  _outDiscardedPackets;
            INT64  _outPackets;
            INT64  _inNoRuleDiscardPackets;
            INT64  _inLagDiscardPackets;
            UINT64 _inRate;
            UINT64 _outRate;
            UINT64 _util;

            Data& operator-= (const Data& other)
            {
                _inOctets -= other._inOctets;
                _inUcastPackets -= other._inUcastPackets;
                _inDiscardedPackets -= other._inDiscardedPackets;
                _inErrors -= other._inErrors;
                _outOctets -= other._outOctets;
                _outUcastPackets -= other._outUcastPackets;
                _outErrors -= other._outErrors;
                _inMcastPackets -= other._inMcastPackets;
                _inBcastPackets -= other._inBcastPackets;
                _outMcastPackets -= other._outMcastPackets;
                _outBcastPackets -= other._outBcastPackets;
                _outDiscardedPackets -= other._outDiscardedPackets;
                _inNoRuleDiscardPackets -= other._inNoRuleDiscardPackets;
                _inLagDiscardPackets -= other._inLagDiscardPackets;
                return *this;
            }
        };

        Data _data;
    };

    struct EthernetCountersBucket : public CountersBucketT<EthernetCountersLine, 96> {
        EthernetCountersLine::Data _oldData;

        EthernetCountersBucket( ) { bzero(CurP( ), sizeof(EthernetCountersLine::Data)); }

        EthernetCountersLine::Data* CurP ( ) { return &((EthernetCountersLine*)GetLine(0))->_data; }

        void UpdateInterval (Mo* moP) override
        {
            GetLine(0)->Update(moP);

            EthernetCountersLine::Data deltaData = *CurP( );
            deltaData -= _oldData;

            _oldData = *CurP( );
            *CurP( ) = deltaData;

            PushLineIndex( );
        }

        void ResetStatistics (Mo* moP) override
        {
            GetLine(0)->Update(moP);
            _oldData -= *CurP( );
        }
    };

    EthernetCountersBucket _counters;

    CountersLine* GetCountersLine ( ) override { return _counters.GetLine(0); }

    CountersBucket* GetCountersBucket ( ) override { return &_counters; }
};

#endif //0
