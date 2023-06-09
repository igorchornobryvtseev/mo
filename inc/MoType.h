/*
 *
 */
#ifndef _MOTYPE_H_INCLUDED
#define _MOTYPE_H_INCLUDED

enum T_AttrType {
    ATTR_INVALID = -1,

    ATTR_IF_TYPE = 1,

    ATTR_HARD_RESET,
    ATTR_RESET,

    ATTR_INDEX,

    ATTR_DESCRIPTION,
    ATTR_NAME,

    ATTR_DATE,
    ATTR_TIME,

    ATTR_ETH_LOOP_TIMEOUT,

    ATTR_ETH_MIRRORING_MODE,
    // ATTR_ETH_MIRRORING_PORT,
    ATTR_ETH_MIRRORING_TARGET,

    ATTR_RF_LOOP,
    ATTR_RF_LOOP_TIMEOUT,
    ATTR_RF_LOCAL_LOOP_MAC,

    ATTR_SYSTEM_SNMP_OID,
    ATTR_SYSTEM_UP_TIME,
    ATTR_SYSTEM_CONTACT,
    ATTR_SYSTEM_LOCATION,
    ATTR_SYSTEM_OID,
    ATTR_VOLTAGE,
    ATTR_TEMPERATURE,
    ATTR_TEMPERATURE_TX,
    ATTR_TEMPERATURE_RX,
    ATTR_CLI_TIMEOUT,

    ATTR_ROW_POINTER,

    ATTR_MTU_SIZE,
    ATTR_SPEED,
    ATTR_LOCAL_MAC,
    ATTR_ADMIN_STATE,
    ATTR_LAG_PORTS,
    ATTR_LAG_PORT_1_STATUS,
    ATTR_LAG_PORT_2_STATUS,
    ATTR_LAG_PORT_1_LACP_STATUS,
    ATTR_LAG_PORT_2_LACP_STATUS,
    ATTR_LAG_PORT_1_LAST_LACPDU_TIME,
    ATTR_LAG_PORT_2_LAST_LACPDU_TIME,
    ATTR_LAG_PORT1,
    ATTR_LAG_PORT2,
    ATTR_OPERATIONAL_STATE,
    ATTR_LAST_CHANGE_TIME,
    ATTR_TRAP_ENABLE,
    ATTR_PROMISCUOUS_MODE,
    ATTR_CONNECTOR,
    ATTR_ALIAS,

    ATTR_MAC_ADDR,
    ATTR_TYPE,
    ATTR_AGING,
    ATTR_NUM_OF_PORTS,

    ATTR_VERSION,
    ATTR_MAX_NUM,

    ATTR_ETH_PORT,
    ATTR_BRIDGE_PORT,
    ATTR_SERVICE_PORT,
    ATTR_STATUS,

    ATTR_ACCOUNTING_PORT,

    ATTR_IP_INDEX,
    ATTR_IP_PREFIX_LEN,
    ATTR_IP_ROUTE,
    ATTR_IP_DESTINATION,
    ATTR_IP_NEXT_HOP,

    ATTR_CINR,
    ATTR_ALIGNMENT_STATUS,
    ATTR_ALIGNMENT_MAX_RSSI,
    ATTR_RESET_ALIGNMENT_MAX_RSSI,
    ATTR_NUM_OF_CHANNELS,
    ATTR_ACCESS_CONTROL,
    ATTR_FREQUENCY_RX,
    ATTR_ROLE,
    ATTR_ROLE_STATUS,
    ATTR_RF_SPECTRUM_MODE,
    ATTR_RF_MODE,
    ATTR_RF_REMOTE_MODE,
    ATTR_RF_TX_MUTE,
    ATTR_TX_MUTE_TIMEOUT,
    ATTR_RF_LOWEST_MODULATION,
    ATTR_RF_ASYMMETRY,
    ATTR_RF_ENCRYPTION,
    ATTR_RF_STATIC_KEY,
    ATTR_RF_AUTHENTICATION_STRING,
    ATTR_RF_AIR_CAPACITY,

    ATTR_RF_TX_STATE,
    ATTR_RF_RX_STATE,
    ATTR_BER_TEST,
    ATTR_MAIN_LINK_STATE,
    ATTR_BACKUP_LINK_STATE,
    ATTR_RX_LINK_ID,
    ATTR_TX_LINK_ID,
    ATTR_CINR_LOW,
    ATTR_CINR_HIGH,
    ATTR_BACKOFF,
    ATTR_AIR_DELAY,
    ATTR_AIR_DISTANCE,
    ATTR_AIR_DELAY_CORRECTION,
    ATTR_PTP1588_ENCAP_TYPE,
    ATTR_DEBUG_TEST,

    ATTR_COUNTER_IN_BER_TOTAL,
    ATTR_COUNTER_IN_BER_ERROR,
    ATTR_COUNTER_IN_BER_RATIO,
    ATTR_COUNTER_AVRG_RSSI,
    ATTR_COUNTER_AVRG_CINR,

    ATTR_COUNTER_REMOTE_CINR,
    ATTR_COUNTER_REMOTE_RSSI,

    ATTR_LOG_LEVEL,

    ATTR_VLAN_ID,
    // ATTR_PB_COMPONENT_ID,
    ATTR_CVID,

    ATTR_FDB_ID,
    ATTR_QUEUE_NUM,

    ATTR_ETHER_TYPE,

    ATTR_IS_LEARNED,

    //    ATTR_PACKET_ERROR_RATE,

    ATTR_TIMEOUT,

    ATTR_NPU_BYPASS,
    ATTR_NPU_FAST_PATH_CLEAR_COUNTER,
    ATTR_NPU_DUMP_COUNTER,
    ATTR_NPU_DUMP_FILE,

    ATTR_UDP_PORT,

    ATTR_TPC,
    ATTR_P1DB_CORRECTION,
    ATTR_DELTA_TPC,
    ATTR_VTUNE,
    ATTR_VTUNE_80,
    ATTR_SUB_BAND,
    ATTR_SUB_BAND_80,
    ATTR_LO_CORRECTION_I,
    ATTR_LO_CORRECTION_Q,
    ATTR_RX_IQ_IMBALANCE_GAIN,
    ATTR_RX_IQ_IMBALANCE_PHASE,
    ATTR_TX_IQ_IMBALANCE_GAIN,
    ATTR_TX_IQ_IMBALANCE_PHASE,
    ATTR_FREQUENCY_OFFSET,
    ATTR_PHASE_NOISE,
    ATTR_LO_AMP_LEVEL,

    ATTR_READ_COMMUNITY,
    ATTR_WRITE_COMMUNITY,
    ATTR_SECURITY_NAME,
    ATTR_ENGINE_ID,
    ATTR_SNMP_VERSION,

    ATTR_INVENT_DESCR,
    ATTR_INVENT_PARENT_ID,
    ATTR_INVENT_PHY_CLASS,
    ATTR_INVENT_RELATIVE_POS,
    ATTR_INVENT_PHY_NAME,
    ATTR_INVENT_HW_REV,
    ATTR_INVENT_FW_REV,
    ATTR_INVENT_SW_REV,
    ATTR_INVENT_SERIAL_NUM,
    ATTR_INVENT_MANUFACT_NAME,
    ATTR_INVENT_MODEL_NAME,
    ATTR_INVENT_REPLACEABLE,

    ATTR_REF_CLOCK_PRIORITY,
    ATTR_REF_CLOCK_QL_ACTUAL,
    ATTR_REF_CLOCK_QL_CONFIG,
    ATTR_REF_CLOCK_QL_MODE,

    ATTR_USER_NAME,
    ATTR_USER_PASSWORD,
    ATTR_USER_LEVEL,

    ATTR_CFM_MD_INDEX,
    ATTR_CFM_MA_INDEX,
    ATTR_CFM_MEP_ID,
    ATTR_CFM_PEER_MEP_ID,
    ATTR_CFM_FORMAT_AND_NAME,
    //    ATTR_CFM_NAME,
    //    ATTR_CFM_FORMAT,
    ATTR_CFM_LEVEL,
    ATTR_CFM_MHF_CREATION,
    ATTR_CFM_MHF_PERMISSION,

    ATTR_INTERVAL,
    ATTR_CALIB,
    ATTR_LUA_TELNET,
    ATTR_RSSI_DISABLE,
    ATTR_PPM_CORRECTION_DISABLE,
    ATTR_PROBE_ALIGNMENT_DISABLE,

    ATTR_CFM_MEP_DIRECTION,
    ATTR_CFM_MEP_VLAN,
    ATTR_CFM_MEP_CCI,
    ATTR_CFM_MEP_MSG_PRIO,
    ATTR_CFM_MEP_LOW_DEFECT,
    ATTR_CFM_MEP_ALARM_TIME,
    ATTR_CFM_MEP_RESET_TIME,
    ATTR_CFM_MEP_LBM_DST_MAC,
    ATTR_CFM_MEP_LBM_DST_MEPID,
    ATTR_CFM_MEP_LBM_DST_TYPE,
    ATTR_CFM_MEP_LBM_TX_NUM,
    ATTR_CFM_MEP_LBM_TX_DATA_LEN,
    ATTR_CFM_MEP_LBM_TX_PRIO,
    ATTR_CFM_MEP_LBM_TX_DROP,
    ATTR_CFM_MEP_LTM_DST_MAC,
    ATTR_CFM_MEP_LTM_DST_MEPID,
    ATTR_CFM_MEP_LTM_DST_TYPE,
    ATTR_CFM_MEP_LTM_TX_TTL,
    ATTR_CFM_MEP_LBM_TX_STATUS,
    ATTR_CFM_MEP_LTM_TX_STATUS,

    ATTR_CFM_MEP_DYNAMIC_FNG_STATE,
    ATTR_CFM_MEP_DYNAMIC_MAC,
    ATTR_CFM_MEP_DYNAMIC_HIGH_DEFECT,
    ATTR_CFM_MEP_DYNAMIC_DEFECTS,
    ATTR_CFM_MEP_DYNAMIC_CCM_SEQ_ERRORS,
    ATTR_CFM_MEP_DYNAMIC_CCM_TX,
    ATTR_CFM_MEP_DYNAMIC_LBM_TX_RESULT,
    ATTR_CFM_MEP_DYNAMIC_LBM_TX_SN,
    ATTR_CFM_MEP_DYNAMIC_LBM_NEXT_SN,
    ATTR_CFM_MEP_DYNAMIC_LBR_IN_ORDER,
    ATTR_CFM_MEP_DYNAMIC_LBR_OUT_OF_ORDER,
    ATTR_CFM_MEP_DYNAMIC_LBR_TX,
    ATTR_CFM_MEP_DYNAMIC_LTM_NEXT_SN,
    ATTR_CFM_MEP_DYNAMIC_LTR_UNEXPECTED,
    ATTR_CFM_MEP_DYNAMIC_LTM_TX_RESULT,
    ATTR_CFM_MEP_DYNAMIC_LTM_TX_SN,

    ATTR_CFM_PEER_MEP_STATE,
    ATTR_CFM_PEER_MEP_FAIL_OK_TIME,
    ATTR_CFM_PEER_MEP_MAC,
    ATTR_CFM_PEER_MEP_RDI,
    ATTR_CFM_PEER_MEP_PORT_STATUS,
    ATTR_CFM_PEER_MEP_IF_STATUS,
    ATTR_CFM_PEER_MEP_CHASSIS_ID_SUBTYPE,
    ATTR_CFM_PEER_MEP_CHASSIS_ID,
    ATTR_CFM_PEER_MEP_MNG_ADDR_DOMAIN,
    ATTR_CFM_PEER_MEP_MNG_ADDR,

    //    ATTR_ALARM_PROPAGATION,
    ATTR_ALARM_PROPAGATION_EH,
    ATTR_REMOTE_ETH_PORT,
    ATTR_LOCAL_PAUSE_FRAMES,

    ATTR_CLASSIFIER_ID,

    ATTR_EVC_ID,

    ATTR_CLASSIFIER_DATA,

    ATTR_LM_INTERVAL,
    ATTR_DM_INTERVAL,
    ATTR_LM_ENABLE,
    ATTR_DM_ENABLE,

    ATTR_AIS_GENERATE,
    ATTR_AIS_PERIOD,
    ATTR_AIS_LEVEL,
    ATTR_AIS_SUPPRESS,
    ATTR_AIS_DEFECTS,

    ATTR_ERP_INDEX,
    ATTR_ERP_ID,
    ATTR_ERP_CW_PORT,
    ATTR_ERP_ACW_PORT,
    ATTR_REVERTIVE,

    ATTR_HOLD_OFF_TIMER,
    ATTR_GUARD_TIMER,
    ATTR_WTB_TIMER,
    ATTR_WTR_TIMER,

    ATTR_ACTION,
    ATTR_ACTION_CHANGE_COUNTER,

    ATTR_TIME_CHANGE_COUNTER,
    ATTR_DATE_CHANGE_COUNTER,

    ATTR_CW_STATUS_DATA,
    ATTR_ACW_STATUS_DATA,
    ATTR_CW_STATUS_RAPS,
    ATTR_ACW_STATUS_RAPS,
    ATTR_PROTOCOL_STATE,
    ATTR_LAST_STATE_TIME,
    ATTR_IDLE_TIME_PERCENTAGE,
    ATTR_PROTECTION_TIME_PERCENTAGE,
    ATTR_MS_TIME_PERCENTAGE,
    ATTR_FS_TIME_PERCENTAGE,
    ATTR_PENDING_TIME_PERCENTAGE,
    ATTR_CW_NODE_ID,
    ATTR_CW_BPR,
    ATTR_ACW_NODE_ID,
    ATTR_ACW_BPR,
    ATTR_CW_MEP,
    ATTR_ACW_MEP,

    ATTR_TX_CONTROL,

    ATTR_PDU_SIZE,
    ATTR_REVISION,
    ATTR_FUNCTION_SUPPORTED,
    ATTR_VENDOR_OUI,
    ATTR_VENDOR_INFO,

    ATTR_METER_ID,
    ATTR_CBS,
    ATTR_EIR,
    ATTR_EBS,

    ATTR_MARKING,

    ATTR_LOOP_CMD,
    ATTR_PEER_REQUEST,

    ATTR_ID,
    ATTR_RTX_LIMIT,

    ATTR_CLOCK_MODE,

    ATTR_RF_RSSI_PTX_CORRECTION,

    ATTR_RF_ATPC,

    ATTR_RF_DISABLE_UPDATE_FN_PLL,
    ATTR_RF_AGC_LINE,
    ATTR_RF_AUTO_RESET,
    ATTR_RF_CALIB_FINISHED,
    ATTR_PARENT,

    ATTR_CHASSIS_ID_SUBTYPE,
    ATTR_CHASSIS_ID,
    ATTR_PORT_ID_SUBTYPE,
    ATTR_PORT_ID,

    ATTR_PORT_DESCR,
    ATTR_SYS_NAME,
    ATTR_SYS_DESCR,

    ATTR_DEFAULT_GATEWAY,

    ATTR_INTERNAL_INDEX,

    ATTR_COPY_SESSION_STATUS,
    ATTR_COPY_SESSION_ACTION,
    ATTR_COPY_SESSION_LOCAL_FILE,
    ATTR_COPY_SESSION_PROGRESS_STATE,
    ATTR_COPY_SESSION_RESULT,

    ATTR_PASSWORD_MIN_LENGTH,
    ATTR_PASSWORD_MIN_DIFFERENCE,

    ATTR_N_FACTOR,
    ATTR_MIN_THRESHOLD,
    ATTR_MAX_THRESHOLD,
    ATTR_PROBABILITY,
    ATTR_MIN_THRESHOLD_YELLOW,
    ATTR_MAX_THRESHOLD_YELLOW,
    ATTR_PROBABILITY_YELLOW,

    ATTR_AAA_MODE,
    ATTR_AAA_SECRET,

    ATTR_FDB_QUOTA_ID,
    ATTR_FDB_QUOTA,
    ATTR_FDB_EVC2QUOTA_ID,
    ATTR_FDB_MAC_LEARNING,
    ATTR_CNTRL_PACKETS_COS_ID,

    ATTR_DHCP_CONFIG,
    ATTR_DHCP_RESTART_ON_ERROR_DELAY,
    ATTR_DHCP_RELAY,

    ATTR_PCP_WRITE_PROFILE_ID,
    ATTR_PCP_WRITE_PROFILE,

    ATTR_PFC_MODE,

    ATTR_DISABLE_ALARM,

    ATTR_MNG_ADDR,

    ATTR_IP_INDEX_LLDP,
    ATTR_ADDR_TYPE_LLDP,

    ATTR_ACM_OAM_RATE_ID,
    ATTR_ACM_OAM_RATE,
    ATTR_ACM_OAM_PERIOD,
    ATTR_ACM_OAM_VLAN,
    ATTR_ACM_OAM_PRIORITY,
    ATTR_ACM_OAM_MD_LEVEL,

    ATTR_EVENT_TRAP_MASKED,
    ATTR_EVENT_ALARM_MASKED,
    ATTR_THRESHOLD_TYPE,
    ATTR_THRESHOLD_H,
    ATTR_THRESHOLD_L,
    ATTR_THRESHOLD,
    ATTR_THRESHOLD_HYSTERESIS,
    ATTR_THRESHOLD_VALID,
    ATTR_EVENT_MASK,

    ATTR_COUNTRY,

    ATTR_CERT_STATUS,
    ATTR_CERT_SUBJECT,
    ATTR_CERT_DOMAIN,
    ATTR_CERT_EMAIL,
    ATTR_CERT_UNIT,
    ATTR_CERT_ORGANIZATION,
    ATTR_CERT_LOCALITY,
    ATTR_CERT_STATE,
    ATTR_CERT_ACTION,
    ATTR_CERT_ACTION_CHANGE_COUNTER,
    ATTR_CERT_ISSUER,
    ATTR_CERT_INVALID_BEFORE,
    ATTR_CERT_INVALID_AFTER,

    ATTR_CURRENT,
    ATTR_CAPACITY,
    ATTR_LEGEND,
    // ATTR_BRIDGE,
    // ATTR_ETH_ISOLATION,
    //    ATTR_MAX_RATE,

    ATTR_PSE_POWER_OUT,

    ATTR_LAG_ALG,
    ATTR_LAG_MEMBER,

    ATTR_LAG_KEY,
    ATTR_LAG_ACTIVITY,

    // ATTR_SELF_MAC,
    // ATTR_CONNECT_STATUS,
    // ATTR_HIDDEN,
    // ATTR_GUEST_CONNECTION,
    // ATTR_SCAN,

    // ATTR_SIGNAL_QUALITY,
    // ATTR_RSSI,
    ATTR_TX_THROUGHPUT,
    ATTR_RX_THROUGHPUT,
    ATTR_TX_RATE,
    // ATTR_UNIT_MODE,
    // ATTR_REAUTH_PERIOD,
    ATTR_CONFIG_SAVED,
    // ATTR_REMOTE_CONFIG,
    // ATTR_LEDS_TURN_OFF,
    // ATTR_RF_RECOVERY_MODE,

    ATTR_SWITCH_MODE,

    ATTR_LINK_BACKUP_LOWEST_MODULATION,

    // ATTR_VRING_TIMING,

    ATTR_START_LOCAL,

    ATTR_MAX = ATTR_START_LOCAL + 50  // keep at end
};

enum T_MoType {
    MO_UNKNOWN = 0,
    MO_SYSTEM,
    MO_ETHERNET,
    MO_FDB_ENTRY,
    MO_IP,
    MO_IPV6,
    MO_IPV6_DNS,
    MO_EMS,
    MO_RF,
    MO_ARP,
    MO_DEBUG_LOG,
    MO_QUEUE,
    MO_IN_QUEUE,
    MO_RF_DEBUG,
    MO_SW,
    MO_SFS,
    MO_SCRIPT,
    MO_MODULATION,
    MO_ROLLBACK,
    MO_NPU,
    MO_PORT_DIAG,
    MO_SNMP_MANAGER,
    MO_SNMP_AGENT,
    MO_ALARM,
    MO_USER,
    MO_INVENTORY,
    MO_CFM_MD,
    MO_CFM_MA,
    MO_CFM_MA_COMP,
    MO_CFM_MEP,
    MO_CFM_PEER_MEP_CREATE,
    MO_CFM_PEER_MEP,
    MO_INGRESS_QOS,
    MO_EGRESS_QOS,
    MO_LICENSE,
    MO_CFM_MP,
    MO_ERP,
    MO_QOS_COMMON,
    MO_ENCRYPTION,
    MO_NTP,
    MO_ROUTE,
    MO_ROUTE6,
    MO_SNMP_USER,
    MO_SYSLOG,
    MO_LINK_OAM,
    MO_LINK_OAM_PEER,
    MO_LINK_OAM_LOOP,
    MO_LINK_BACKUP,
    MO_CLASSIFIER_EVC,
    MO_CLASSIFIER_COS,
    MO_METER,
    MO_EVENT_CONFIG,
    MO_LLDP,
    MO_LLDP_COMMON,
    MO_LLDP_REMOTE,
    MO_ACCESS_LIST,
    MO_PASSWORD_STRENGTH,
    MO_BRIDGE_COMMON,
    MO_WRED,
    MO_AAA,
    MO_AAA_SERVER,
    MO_FDB_QUOTA,
    MO_FDB_EVC_QUOTA,
    MO_DHCP_V4,
    MO_DHCP_V6,
    MO_DHCLIENT6_OPTIONS,
    MO_PCP_WRITE_PROFILE,
    MO_PFC,
    MO_PCP_MAP,
    MO_ACM_OAM,
    MO_IEEE1588,
    MO_MOM,
    MO_CERTIFICATE,
    MO_PSE,
    MO_LAG,
    MO_BRIDGE_PORT,
    MO_BRIDGE_RULE,
    MO_PIPE_MODE,
    MO_SYNCE,

    // all interfaces should be till here

    MO_MAX
};

#endif  // _MOTYPE_H_INCLUDED
