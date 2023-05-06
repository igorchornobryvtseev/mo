//***************************************************************
// File:	 CaSharedRegion.h
// Copyright: Siklu Communications , 2010
// SVN: $Revision$
// SVN: $Author$
// SVN: $Date$
// ***************************************************************

#pragma once

// #include <pthread.h>
// #include <resolv.h>
// #include <semaphore.h>
// #include <unistd.h>

// #include "CaExternal.h"
// #include "erp.h"
// #include "event.h"
// #include "IpAddress.h"
// #include "IpV6Address.h"
// #include "MyOid.h"
// #include "MyString.h"
// #include "PlaceBuffer.h"
// #include "sk_msg.h"

#define DHCPV6_MAX_DNS_SERVERS            MAXNS  // from <resolv.h>

#define MAX_NUM_OF_SNMP_STATIC_MANAGERS   (5)                                                                  /* 5 static SNMP Managers configured by User */
#define MAX_NUM_OF_SNMP_DYNAMIC_MANAGERS  (5)                                                                  /* 5 dynamic SNMP Managers for one EMS */
#define MAX_NUM_OF_SNMP_MANAGERS          (MAX_NUM_OF_SNMP_STATIC_MANAGERS + MAX_NUM_OF_SNMP_DYNAMIC_MANAGERS) /* 5 static and 5 dynamic for one EMS */

#define MAX_NUM_OF_SYSLOG_STATIC_SERVERS  (5)                                                                    /* 5 static Syslog Servers configured by User */
#define MAX_NUM_OF_SYSLOG_DYNAMIC_SERVERS (5)                                                                    /* 5 dynamic Syslog Servers for one EMS */
#define MAX_NUM_OF_SYSLOG_SERVERS         (MAX_NUM_OF_SYSLOG_STATIC_SERVERS + MAX_NUM_OF_SYSLOG_DYNAMIC_SERVERS) /* 5 static and 5 dynamic for one EMS */

#define MAX_COMMUNITY_LEN                 16
#define MAX_SNMP_ENGINE_ID_HEX_LEN        40
#define MAX_SNMP_PASSWORD_LEN             32

// #define PID_NOT_EXIST                     0x7FFFFFFF

// #define REGION_NAME                       "/shared_region"

// #define EVENT_LOG_FILE_PATH               "/var/log/sw_events.log"
// #define AUDIT_LOG_FILE_PATH               "/var/log/config_audit.log"

// class SnmpColumn;

// ////////////////////////////////////////////////////////////

// long mygettime( );

////////////////////////////////////////////////////////////

// enum CadMsgType : int32_t {
//     WDOG_TO_CA_REQUEST = SkMessage::messsageIdBaseForQ(SkMessage::E_QueueId::MSG_CA_IN_CONFIG_Q) + 1,
//     CLI_REQUEST_ENTER,
//     CLI_REQUEST_TAB,
//     CLI_REQUEST_HELLO,
//     SNMP_REQUEST_INIT,
//     SNMP_REQUEST_AGENT_DEFS,
//     SNMP_REQUEST,
//     AUTONOMOUS_PROCESS_REQUEST,
//     LOCAL_WEB_REQUEST,
//     REMOTE_WEB_REQUEST,
//     CLI_DHCP_REQUEST_ENTER,
//     CLI_HTTP_REQUEST_ENTER,
//     ASYNC_MSG_CERT_CREATE_FINISHED,
//     PROCESS_STARTED,
// };

// constexpr const char* to_string (CadMsgType cmt, const char* defValue = nullptr)
// {
//     switch ( cmt ) {
//         case CadMsgType::WDOG_TO_CA_REQUEST: return "WDOG_TO_CA_REQUEST";
//         case CadMsgType::CLI_REQUEST_ENTER: return "CLI_REQUEST_ENTER";
//         case CadMsgType::CLI_REQUEST_TAB: return "CLI_REQUEST_TAB";
//         case CadMsgType::CLI_REQUEST_HELLO: return "CLI_REQUEST_HELLO";
//         case CadMsgType::SNMP_REQUEST_INIT: return "SNMP_REQUEST_INIT";
//         case CadMsgType::SNMP_REQUEST_AGENT_DEFS: return "SNMP_REQUEST_AGENT_DEFS";
//         case CadMsgType::SNMP_REQUEST: return "SNMP_REQUEST";
//         case CadMsgType::AUTONOMOUS_PROCESS_REQUEST: return "AUTONOMOUS_PROCESS_REQUEST";
//         case CadMsgType::LOCAL_WEB_REQUEST: return "LOCAL_WEB_REQUEST";
//         case CadMsgType::REMOTE_WEB_REQUEST: return "REMOTE_WEB_REQUEST";
//         case CadMsgType::CLI_DHCP_REQUEST_ENTER: return "CLI_DHCP_REQUEST_ENTER";
//         case CadMsgType::CLI_HTTP_REQUEST_ENTER: return "CLI_HTTP_REQUEST_ENTER";
//         case CadMsgType::ASYNC_MSG_CERT_CREATE_FINISHED: return "ASYNC_MSG_CERT_CREATE_FINISHED";
//         case CadMsgType::PROCESS_STARTED: return "PROCESS_STARTED";
//     };
//     return defValue;
// }

// enum SnmpTrapEventMsgType : int32_t { SNMP_EVENT_TRAP_DATA = SkMessage::messsageIdBaseForQ(SkMessage::E_QueueId::MSG_SNMP_IN_EVENT_Q) };

// enum T_SnmpConfigMsg : int32_t { E_SNMP_MSG_SET_CONFIG = SkMessage::messsageIdBaseForQ(SkMessage::E_QueueId::MSG_SNMP_IN_CONFIG_Q) };

// void sendConfigMsgToCad(CadMsgType requestType, uint32_t clientId);
// void MsgSendToCadAndWaitAck(uint32_t clientId, CadMsgType msgId);

// struct ClientArea;
// ClientArea* Client(uint32_t clientId);

// const char* _AttachToCa( );
// const char* AttachSnmpToCa( );
// const char* AttachCliToCa(uint32_t* clientIdP);

// extern MyStringT<256> msgLog;

// enum class SnmpConfigType {
//     Unknown,
//     CreateUser,
//     DeleteUser,
//     ShowUser,
//     Reconfig,
// };

// constexpr const char* to_string (SnmpConfigType e, const char* defValue = nullptr)
// {
//     switch ( e ) {
//         case SnmpConfigType::Unknown: return "SnmpConfigType::Unknown";
//         case SnmpConfigType::CreateUser: return "SnmpConfigType::CreateUser";
//         case SnmpConfigType::DeleteUser: return "SnmpConfigType::DeleteUser";
//         case SnmpConfigType::ShowUser: return "SnmpConfigType::ShowUser";
//         case SnmpConfigType::Reconfig: return "SnmpConfigType::Reconfig";
//     }
//     return defValue;
// }

enum MyAuthType { MY_AUTH_NONE, MY_AUTH_MD5, MY_AUTH_SHA };

enum MyPrivType { MY_PRIV_NONE, MY_PRIV_DES, MY_PRIV_AES };

// struct DataConfigToSnmp {
//     SnmpConfigType _configType {SnmpConfigType::Unknown};
//     char           _resultFileName[200] {0};

//     char _engineId[MAX_SNMP_ENGINE_ID_HEX_LEN + 1] {0};
//     char _userName[MAX_LEN_SNMP_USER_NAME + 1] {0};

//     MyAuthType _authType {MY_AUTH_NONE};
//     char       _authPassPhrase[MAX_SNMP_PASSWORD_LEN + 1] {0};

//     MyPrivType _privType {MY_PRIV_NONE};
//     char       _privPassPhrase[MAX_SNMP_PASSWORD_LEN + 1] {0};
// };

// void                            MsgSendConfigToSnmp(DataConfigToSnmp& srcP);
// std::optional<DataConfigToSnmp> MsgReadConfigToSnmp( );

////////////////////////////////////////////////////////////

struct MsgCliHello {
    char       prompt[MAX_CLI_INPUT_LEN + 1];
    //E_UserType defaultUserType;
};

////////////////////////////////////////////////////////////

typedef enum {
    CA_REMOTE_CMD_ALLOW_DEFAULT = 'D',  // defined by group
    CA_REMOTE_CMD_DENIED        = 'N',  // override default permission
    CA_REMOTE_CMD_ALLOW         = 'Y',  // override default permission
} SKL_REMOTE_CMD_ALLOW_E;

struct MsgCliEnter {
    char requestLine[MAX_CLI_INPUT_LEN + 1];

    bool replyError;  // reply, used for WEB only
    char replyPrompt[MAX_CLI_INPUT_LEN + 1];
    char replyPrintBuffer[CLI_REPLY_PRINT_BUFFER + 1];
};

////////////////////////////////////////////////////////////

struct MsgCliTab {
    char _cliLine[MAX_CLI_INPUT_LEN + 1];
    int  _cliCurrentPos;

    char _prompt[MAX_CLI_INPUT_LEN + 1];

    char _help[1000];

    bool _isAsk;  // request
    bool _error;  // reply
    bool _wasFullWordCompleted;
};

////////////////////////////////////////////////////////////

// struct MsgSnmpInit {
//     int _count;

//     SnmpColumn* _snmpColumnP;  // do not use it in sikluPlugin !!!
//     MyOid       _snmpColumnOid;
//     char        _tableName[64];
//     char        _columnName[64];
// };

////////////////////////////////////////////////////////////

// struct MsgSnmpAgentDefs {
//     uint32_t _udpPort;
//     char     _readCommunity[MAX_COMMUNITY_LEN + 1];
//     char     _writeCommunity[MAX_COMMUNITY_LEN + 1];
//     bool     _isSystemSnmpVer3;

//     bool _isColdTrapEnable;

//     // trap destinations
//     struct TrapDest {
//         bool        _isIpV6;
//         IpAddress   _ipV4;
//         IpV6Address _ipV6;

//         IpAddress _sourceIp;  // used for generic alarm trap source IP field
//         uint32_t  _udp;
//         bool      _isSnmpVer3;
//         char      _securityName[MAX_COMMUNITY_LEN + 1];
//         char      _engineId[MAX_SNMP_ENGINE_ID_HEX_LEN + 2];
//     };

//     TrapDest _trapDestTable[MAX_NUM_OF_SNMP_MANAGERS];
//     uint32_t _numOfTrapDest;
// };

////////////////////////////////////////////////////////////

// struct SnmpVariable {
//     MyOid  _oid;
//     u_char _asnType;
//     u_char _valueBuffer[600];
//     int    _valueLen;

//     void*       _originalP;  // (netsnmp_variable_list *)
//     SnmpColumn* _columnP;
//     int         _errCode;

//     void operator<< (UINT64 value);
//     void operator<< (const MyOid& val);
//     void operator<< (const Mac& val);
//     void operator<< (const IpV6Address& val);
//     void operator<< (IpAddress val);
//     void operator<< (const MyString& valR);
//     void operator<< (std::string_view val);

//     template<typename T>
//     void setValue (const T& val)
//     {
//         setValueArray(&val, 1);
//     }

//     template<typename T>
//     void setValueArray (const T valP[], size_t count)
//     {
//         if ( count * sizeof(T) <= sizeof(_valueBuffer) ) {
//             _valueLen = count * sizeof(T);
//             memcpy(_valueBuffer, valP, _valueLen);
//         }
//     }

//     bool      AsString(char*& dstP, size_t& dstLen);
//     bool      AsBitmap(BitMapDynamic* dstP);
//     bool      AsString(MyString& dst);
//     uint32_t  AsUint32( );
//     IpAddress AsIp( );
//     MyOid     AsOid( );
//     Mac       AsMac( );
// };

// struct MsgSnmpPdu {
//     bool _isNext;
//     bool _isSet;
//     bool _isV1;

//     enum { MAX_NUM_OF_VARS = 200 };

//     SnmpVariable _varsTable[MAX_NUM_OF_VARS];
//     int          _numOfVars;
// };

////////////////////////////////////////////////////////////

// class RepeatStack;

// // Sleep - client (CLI) :
// //              1. prints '.'
// //              2. performs a 1 sec sleep
// //              3. performs repeat call
// // NoSleep - client (CLI) :
// //              1. does not print '.'
// //              2. does not perform a 1 sec sleep
// //              3. prints an ocli stream
// //              4. performs repeat call

// typedef enum { NO_SLEEP, SLEEP, SLEEP_TILL_CTRL_C, ASK_USER_ACK } SleepType;

// struct CadRepeater {
//     SleepType    sleepType;
//     bool         wasStopRequest {false};
//     bool         printWhenSleep {false};
//     RepeatStack* stackP {nullptr};

//     struct UserAck {
//         void Start (const char* questionP)
//         {
//             Clean( );
//             question = questionP;
//         }

//         void Clean ( ) { *this = UserAck( ); }

//         void Print ( ) const { 
//             //fmt::print("\n{}\npress ctrl/c to terminate, Y/y to confirm, N/n to reject: ", question); 
//         }

//         std::string question;
//         bool        wasPrint {false};
//         bool        isAck {false};
//     };

//     UserAck userAck;

//     explicit CadRepeater(SleepType sleepType) : sleepType(sleepType) { }

//     virtual ~CadRepeater( ) = default;

//     virtual void CallMe( ) = 0;

//     virtual bool IsScript ( ) const { return false; }

//     inline void EndMe( );
// };

// class RepeatStack
// {
// public:
//     [[nodiscard]] bool isEmpty ( ) const { return itemsCount == 0; }

//     CadRepeater& top ( ) { return *getItem(itemsCount - 1); }

//     void clean ( )
//     {
//         while ( !isEmpty( ) )
//             pop( );
//     }

//     void pop ( ) { getItem(--itemsCount)->CadRepeater::~CadRepeater( ); }

//     template<class T, class... Types>
//     const char* push (Types&&... args)
// #if __cplusplus >= 202002L
//         requires std::derived_from<T, CadRepeater>
// #endif
//     {
//         /// ToDo: important note: we put virtual classes into shared memory, thus vptr is pointing some address of src
//         ///        process, but can be called from dst process.. We're ok right now just becasue shared memory in
//         ///        both processes is placed with same base address (by mmap) so vptr, taken in one process has valid value in another
//         ///        process. In general -- this is not true, so this is potentially very dangerous place
//         static_assert(sizeof(T) < sizeof(CadRepeaterMemChunk));
//         if ( itemsCount >= _repeatersBuffer.size( ) )
//             return "Cannot start process (nesting).";
//         CadRepeater* p = new (getItem(itemsCount++)) T(std::forward<Types>(args)...);
//         p->stackP      = this;
//         return nullptr;
//     }

//     [[nodiscard]] bool isAnyUnderlyingIsScript ( ) const
//     {
//         for ( ssize_t idx = itemsCount - 2; idx >= 0; idx-- )
//             if ( getItem(idx)->IsScript( ) )
//                 return true;

//         return false;
//     }

// private:
//     CadRepeater* getItem (size_t idx) { return reinterpret_cast<CadRepeater*>(_repeatersBuffer.at(idx)); }

//     const CadRepeater* getItem (size_t idx) const { return reinterpret_cast<const CadRepeater*>(_repeatersBuffer.at(idx)); }

//     using CadRepeaterMemChunk = char[1024];
//     std::array<CadRepeaterMemChunk, 5> _repeatersBuffer;
//     size_t                             itemsCount = 0;
// };

// void CadRepeater::EndMe( )
// {
//     stackP->pop( );
// }

////////////////////////////////////////////////////////////

struct ScriptBuffer {
    enum { MAX_LEN = 2400000 /*1000000*/ };

    pthread_mutex_t _accessMutex;  // = PTHREAD_MUTEX_INITIALIZER;
    int             _startPos;
    int             _freePos;
    char            _body[MAX_LEN];
};

void ScriptBufferInit(ScriptBuffer* bufferP);
void ScriptBufferClean(ScriptBuffer* bufferP);
void ScriptBufferWrite(ScriptBuffer* bufferP, const char* srcP);
int  ScriptBufferRead(ScriptBuffer* bufferP, char* dstP, int maxLen);

////////////////////////////////////////////////////////////

// struct UserAuthInfo {
//     char       _userName[128] {0};
//     E_UserType _userLocalGroup;
//     E_UserType _userRemoteGroup;  // inform supported by remote TACACS/RADIUS server

//     uint32_t total_num_indiv_per;     // total number individual permissions relayed from tacacs/radius server to the system
//     uint32_t last_pos;                // offset of last character in  AccumPermArea string relative to start of  AccumPermArea
//     char     _accumPermArea[0x1000];  // common for all tacacs/radius permission strings

//     struct PermKey {
//         uint32_t crc;
//         uint32_t offset;  // permition's string offset inside  AccumPermArea, relative to start of AccumPermArea
//     };

//     PermKey _permKey[200];

//     E_Status CopyAuthFromEnvironment(uint32_t clientId);
//     bool     CheckPermission(std::string_view operation, std::string_view param) const;
//     bool     CheckHttpPermission(std::string_view query) const;
// };

// struct MyHttpSession {
//     constexpr static size_t maxSessionsCount = 10;

//     void CreateNew(int slot);

//     int    _slot;
//     bool   _isOcuppied;
//     time_t _lastAccessTime;
//     char   _cookie[64];

//     UserAuthInfo _userInfo;
// };

// struct AllHttpSession {
//     void CollectGarbage( );

//     MyHttpSession* CreateNew( );

//     MyHttpSession* FindFree(int* indexP);

//     std::array<MyHttpSession, MyHttpSession::maxSessionsCount> _table;
// };

////////////////////////////////////////////////////////////

// struct ClientArea {
//     SkMessage::E_QueueId _clientQueue;
//     pid_t                _clientPid;
//     UINT64               _startTime;  // currently cli only

//     UserAuthInfo _userInfo;  // remote server AAA info for TACACS+ and RADIUS protocols

//     RepeatStack _repeatStack;

//     template<class T = char>
//     T* getData ( )
//     {
//         return reinterpret_cast<T*>(_data);
//     }

// private:
//     CHAR _data[sizeof(MsgCliEnter) + 200];  // for longest message
// };

#define CLIENT_SNMP                    (SkMessage::MAX_NUM_OF_CLI)
#define CLIENT_AUTONOMOUS_PROCESS      (CLIENT_SNMP + 1)
#define CLIENT_LOCAL_WEB               (CLIENT_AUTONOMOUS_PROCESS + 1)
#define CLIENT_REMOTE_WEB              (CLIENT_AUTONOMOUS_PROCESS + 2)

#define MAX_RFPIPE_TO_K60S_UART_BUFFER 41

// class CaSharedRegion
// {
// public:
//     ClientArea _clientsCli[SkMessage::MAX_NUM_OF_CLI];
//     ClientArea _clientSnmp;
//     ClientArea _clientAutonomousProcess;
//     ClientArea _clientLocalWeb;
//     ClientArea _clientRemoteWeb;

//     WebRemChannelHeader _localToRemoteRequestHeader;
//     WebRemChannelHeader _localFromRemoteReplyHeader;
//     WebRemChannelHeader _remoteFromLocalRequestHeader;
//     WebRemChannelHeader _remoteToLocalReplyHeader;

//     char _localToRemoteRequest[MAX_CLI_INPUT_LEN];
//     char _localFromRemoteReply[CLI_REPLY_PRINT_BUFFER];

//     char _rfpipeToModemdBuf[MAX_RFPIPE_TO_K60S_UART_BUFFER];
//     char _modemdTorfpipeBuf[MAX_RFPIPE_TO_K60S_UART_BUFFER];

//     sem_t _localFromRemoteReplySemaphore;

//     sem_t _rfpipeToModemdTxSemaphore;
//     sem_t _modemdFromRfpipeTxSemaphore;

//     sem_t _rfpipeFromModemdRxSemaphore;
//     sem_t _modemdToRfpipeRxSemaphore;

//     sem_t _acceptConnectionSemaphore;

//     pthread_mutex_t _remoteWebAccessMutex;
//     bool            _remoteWebSessionBusy;  // indicates whether _remoteWebAccessMutex is locked

//     ScriptBuffer _autonomousScriptBuffer;

//     pid_t _cadPid;
//     int   _cliTimeoutSec {15 * 60};

//     pthread_mutex_t _cliAllocationMutex;

//     int _snmpReInitCounter {0};
//     int _netSnmpReConfigRequestCounter {0};

//     AllHttpSession _httpSessions;
// };

// extern CaSharedRegion* regionP;

// ////////////////////////

// struct __attribute__((packed)) TrapData {
//     struct __attribute__((packed)) VarValue {
//         union {
//             uint32_t _asInt;
//             long     _asTime;
//             uint32_t _asIp;
//             uint16_t _asStrOffset;
//         } _body;

//         u_char _format;
//     };

//     struct __attribute__((packed)) VarOid {
//         MyOidNum _table[22];
//         UINT8    _numOf;
//     };

//     VarValue _varValue[12];
//     VarOid   _varOid[12];

//     char   _trapId[32];
//     UINT8  _numVars {0};
//     UCHAR  _buffer[256] {0};
//     UINT16 _offset {1};

//     MyOid GetVarOid (int index)
//     {
//         MyOid oid;
//         for ( UINT8 i = 0; i < _varOid[index]._numOf; i++ ) {
//             MyOidNum cur = _varOid[index]._table[i];
//             oid.AddNum(cur);
//         }
//         return oid;
//     }

//     u_char GetVarFormat (int index) { return _varValue[index]._format; }

//     uint32_t GetValueAsInt (int index) { return _varValue[index]._body._asInt; }

//     long GetValueAsTime (int index) { return _varValue[index]._body._asTime; }

//     IpAddress GetValueAsIp (int index) { return IpAddress(_varValue[index]._body._asIp); }

//     char* GetValueAsStr (int index) { return (char*)_buffer + _varValue[index]._body._asStrOffset; }

//     bool sendEvent( ) const;
// };

// inline E_Status sendAsyncMessage (SkMessage::E_QueueId queueId, int msgId)
// {
//     return SkMessage::SkMessage(SkMessage::E_QueueId::MSG_DUMMY_Q).setTimeout(SkMessage::waitInf).setMode(SkMessage::E_SendMode::Async).send(queueId, msgId);
// }

// CaSharedRegion* CreateCommunicator( );
// void            DestroyCommunicator( );
