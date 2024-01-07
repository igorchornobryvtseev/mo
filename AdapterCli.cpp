#include "igorc_common.h"
#include "MyString.h"
#include "RetStatus.h"
#include "CliContext.h"
#include <string>

// #include <dirent.h>
// #include <sys/stat.h>

// #include <cassert>
// #include <fstream>
// #include <iostream>

// #include "bist_monitor_common_defs.h"
// #include "CaExternal.h"
// #include "DebugFlag.h"
// #include "event.h"
// #include "FtpControl.h"
// #include "MoArp.h"
// #include "MoFdbEntry.h"
// #include "MoLicense.h"
// #include "MoLinkOamLoop.h"
// #include "MoNpu.h"
// #include "MoPse.h"
// #include "MoRf.h"
// #include "MoScript.h"
// #include "MoSnmpUser.h"
// #include "MoSw.h"
// #include "MoSystem.h"
// #include "siklu_fmt.h"
// #include "StartupConfig.h"
// #include "string_manipulations.h"
// #include "system_handler_helper.h"
// #include "utils.h"

// fs::path errorFilePath                     = FsLocation( ).startupErrorFilepath( );
bool     disableLicenseConfigMismatchCheck = false;
uint32_t requestClientId;
bool     commandOnStartup = true;

void DoFrequencyXmlShow(Xml* xmlP, MyString* instancesP);
void NxpModem_Get_Fw_Revision(char* fw_rev_rf_70_card, char* fw_rev_rf_80_card);

// const fs::path& GetErrorFilePath ( )
// {
//     return errorFilePath;
// }

uint32_t GetClientId ( )
{
    return requestClientId;
}

void LogUserActivity (std::string_view text)
{
    if ( commandOnStartup )
        return;

    // std::string tmp = fmt::format("User: {} : {}", Client(GetClientId( ))->_userInfo._userName, text);

//#ifdef X86_TARGET
    LOG("'%s'", text.data());
// #else
//     syslog(LOG_LOCAL5, "%s", tmp.c_str( ));
// #endif
}

CliContext* cliP = nullptr;

// static bool IsSpectrumTestRunning ( )
// {
//     MoRf* rfP = FindMoRf( );
//     return rfP && rfP->IsSpectrumRunning( );
// }

struct CliVars {
    struct CliVarEntry {
        void Set (const char* nameP, const char* valueP)
        {
            _name.Clean( );
            _name << nameP;

            _value.Clean( );
            _value << valueP;

            _isUse = true;
        }

        bool          _isUse {false};
        MyStringT<32> _name;
        MyStringT<64> _value;
    };

    CliVarEntry _globals[100];
    CliVarEntry _locals[5];
    UINT32      _numOfLocals {0};

    // RetStatus SetGlobal (const char* nameP, const char* valueP)
    // {
    //     for ( UINT32 i = 0; i < std_size(_globals); i++ ) {
    //         if ( _globals[i]._isUse && (_globals[i]._name == nameP) ) {
    //             _globals[i].Set(nameP, valueP);
    //             return RetStatus {E_RetStatus::Ok};
    //         }
    //     }
    //     for ( UINT32 i = 0; i < sizeof(_globals) / sizeof(_globals[0]); i++ ) {
    //         if ( !_globals[i]._isUse ) {
    //             _globals[i].Set(nameP, valueP);
    //             return RetStatus {E_RetStatus::Ok};
    //         }
    //     }
    //     return RetStatus {"no space"};
    // }

    // RetStatus ClearGlobal (const char* nameP)
    // {
    //     for ( UINT32 i = 0; i < std_size(_globals); i++ ) {
    //         if ( !_globals[i]._isUse )
    //             continue;
    //         if ( _globals[i]._name != nameP )
    //             continue;

    //         _globals[i]._isUse = false;
    //         return RetStatus {E_RetStatus::Ok};
    //     }
    //     return RetStatus {"not found"};
    // }

    // void AddLocals (const char* startP)
    // {
    //     while ( true ) {
    //         MyStringT<128> name;

    //         for ( ; *startP == ' '; startP++ )
    //             ;                                                                       // skip whitespaces
    //         for ( ; (*startP != 0) && (*startP != ' ') && (*startP != '='); startP++ )  // read name
    //             name.Print("%c", *startP);

    //         for ( ; *startP == ' '; startP++ )
    //             ;                  // skip whitespaces
    //         if ( *startP != '=' )  // check =
    //             return;
    //         startP++;

    //         WordReaderString readerValue(startP);
    //         if ( !readerValue.ReadWord( ).ok( ) )
    //             return;
    //         startP += readerValue.GetPos( );

    //         if ( _numOfLocals < std_size(_locals) )
    //             _locals[_numOfLocals++].Set(name.AsStr( ), readerValue._dst.AsStr( ));
    //     }
    // }

    // void ClearLocals ( ) { _numOfLocals = 0; }

    char* Find (const char* nameP)
    {
        LOG("return NULL");
        // UINT32 i;
        // for ( i = 0; i < _numOfLocals; i++ ) {
        //     if ( _locals[i]._name == nameP )
        //         return _locals[i]._value.AsStr( );
        // }

        // for ( i = 0; i < std_size(_globals); i++ ) {
        //     if ( !_globals[i]._isUse )
        //         continue;
        //     if ( _globals[i]._name == nameP )
        //         return _globals[i]._value.AsStr( );
        // }
        return nullptr;
    }

    void ShowAll ( )
    {
        ocli << "Locals:\n";
        for ( UINT32 i = 0; i < 5 /*_numOfLocals*/; i++ )
            ocli << FIELD(_locals[i]._name.AsStr( ), 40) << ": " << FIELD(_locals[i]._value.AsStr( ), 100) << "\n";

        ocli << "Globals:\n";
        for ( UINT32 i = 0; i < 100 /*std_size(_globals)*/; i++ ) {
            if ( _globals[i]._isUse )
                ocli << FIELD(_globals[i]._name.AsStr( ), 40) << ": " << FIELD(_globals[i]._value.AsStr( ), 100) << "\n";
        }
    }
};

CliVars cliVars;

// void AddLocalCliVars (const char* startP)
// {
//     cliVars.AddLocals(startP);
// }

// void ClearLocalCliVars ( )
// {
//     cliVars.ClearLocals( );
// }

bool* DebugModeP ( )
{
    static bool debugMode = false;
    return &debugMode;
}

bool IsDebugMode ( )
{
    return *DebugModeP( );
}

class CliCmd
{
public:
    CliCmd(CmdType cmdType, const char* cmdName, bool isDebug = false) : _cmdType(cmdType), _cmdName(cmdName), _isDebug(isDebug) { }

    virtual ~CliCmd( ) = default;

    virtual void Run(CliContext* contextP) = 0;
    virtual void PrintCmdHelp( )           = 0;

    CmdType         _cmdType;
    const char*     _cmdName;
    bool            _isDebug;
};

void PrintCmdHelp (CliCmd* cmdP)
{
    cmdP->PrintCmdHelp( );
}

//////////////////////////////////////////////////////////

class ValueCmd : public ValueDictionary
{
public:
    static CliCmd* Cmd(UINT32 index);

    Dictionary* GetDictionary ( ) override
    {
        static DictionaryT<200> myDictionary;
        if ( myDictionary.IsEmpty( ) ) {
            for ( UINT32 i = 0; Cmd(i) != nullptr; i++ )
                myDictionary.Add(Cmd(i)->_cmdType, Cmd(i)->_cmdName);
        }

        return &myDictionary;
    }

    CliCmd* Cmd ( ) { return Cmd(_context); }

    bool DoIsMatch ( ) override { return IsDebugMode( ) ? true : (!Cmd( )->_isDebug); }
};

void PrintCliHelperRoot ( )
{
    // build common help for normal or debug mode
    ValueCmd cmd;
    for ( cmd.Start( ); cmd.IsValid( ); cmd.Next( ) ) {
        ocli << "\n";
        cmd.Cmd( )->PrintCmdHelp( );
    }
}

//////////////////////////////////////////////////////////

class CliCmdHelp : public CliCmd
{
public:
    CliCmdHelp( ) : CliCmd(CMD_HELP, "help") { }

    void Run (CliContext* contextP) override
    {
        //fTRACE(1, "entry");
        ValueCmd valueCmd;
        valueCmd.SetCli(contextP);
        if ( contextP->CheckExit(true) )
            return;

        CliContextExe* contextExeP = dynamic_cast<CliContextExe*>(contextP);
        if ( contextExeP != nullptr )
            contextExeP->_isCmdHelp = true;

        cliP->_helper.SetAssignCmd(valueCmd.Cmd( ));
        valueCmd.Cmd( )->Run(contextP);
        //fTRACE(1, "exit");
    }

    void PrintCmdHelp ( ) override
    {
        ValueCmd cmd;
        for ( cmd.Start( ); cmd.IsValid( ); cmd.Next( ) ) {
            if ( cmd.GetEnum<CmdType>( ) == CMD_DEBUG && !IsDebugMode( ) )
                continue;
            ocli << "help " << cmd << ", ";
        }
    }
};

//////////////////////////////////////////////////////////

// class CliCmdReset : public CliCmd
// {
// public:
//     CliCmdReset( ) : CliCmd(CMD_RESET, "reset") { }

//     void Run (CliContext* contextP) override
//     {
//         DictionaryT<10> modifier;
//         modifier.Add(1, "system");
//         modifier.Add(2, "rf");
//         modifier.Add(3, "switch");

//         ValueDictExternal valueModifier(&modifier);

//         valueModifier.SetCli(contextP);
//         if ( contextP->CheckExit(true) )
//             return;

//         // perform operation
//         if ( valueModifier == "system" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             Mo*        systemP     = Mo::FindMo(MO_SYSTEM);
//             ValueLong* valueResetP = (ValueLong*)systemP->AttrValue(ATTR_HARD_RESET);
//             valueResetP->SetUint32(valueResetP->GetUint32( ) + 1);
//             ocli << "\nSystem reset...";
//         } else if ( valueModifier == "rf" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             if ( IsSpectrumTestRunning( ) ) {
//                 cliP->ReturnErrorAndPrint(RetStatus {"Can't reset RF while spectrum analyzer is running. Operation failed."});
//                 return;
//             }
//             Mo*        rfP         = Mo::FindMo(MO_RF);
//             ValueLong* valueResetP = (ValueLong*)rfP->AttrValue(ATTR_RESET);
//             valueResetP->SetUint32(valueResetP->GetUint32( ) + 1);
//             ocli << "\nRf reset done";
//         } else if ( valueModifier == "switch" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             fLOG_EX_DEBUG(false, "send E_NPU_SET_SW_RESET message to npu from CLI command");
//             if ( SkMessage::SkMessage msg(SkMessage::E_QueueId::MSG_CAD_API_Q); msg.send(SkMessage::E_QueueId::MSG_NPU_Q, E_NPU_SET_SW_RESET) == E_Status::Error ) {
//                 fLOG_EX_ERR(false, "Fail to send {} message to {} queue from CLI command", msg.msgName( ), msg.qName( ));
//             }

//             ocli << "\nSwitch reset done";
//         } else if ( valueModifier == "rf-debug" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             Mo*        moP         = Mo::FindMo(MO_RF_DEBUG);
//             ValueLong* valueResetP = (ValueLong*)moP->AttrValue(ATTR_RESET);
//             valueResetP->SetUint32(valueResetP->GetUint32( ) + 1);
//             ocli << "\nrf-debug reset done";
//         } else if ( valueModifier == "pse" ) {
//             ValuePsePort valuePsePort;
//             valuePsePort.SetCli(contextP);
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             if ( !LicenseController::Self( )->GetActual(LICENSE_PSE) )
//                 ocli << "\nPSE license is disabled";
//             else {
//                 Mo*        pseP        = Mo::FindMo(MO_PSE, MoId(valuePsePort.GetUint32( )));
//                 ValueLong* valueResetP = (ValueLong*)pseP->AttrValue(ATTR_RESET);
//                 valueResetP->SetUint32(valueResetP->GetUint32( ) + 1);
//                 ocli << "\nPSE reset done";
//             }
//         } else {  // link-oam-loopback
//             ValueEthernetPortsList valuePortsList;

//             valuePortsList.SetCli(contextP);
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             for ( IteratorValueListOf it = valuePortsList.BeginList( ); it.IsValid( ); it++ ) {
//                 MoLinkOamLoop* loopP = Mo::FindMoT<MoLinkOamLoop>(MoId(*it));
//                 if ( loopP != nullptr ) {
//                     UINT32 count = loopP->_valueResetChangeCounter.GetUint32( ) + 1;
//                     loopP->_valueResetChangeCounter.SetUint32(count);
//                     loopP->_valueStatusCmd.SetUint32(E_LINKOAM_LOOPBACK_STATUS_TERM_LOOPBACK);
//                 }
//             }
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         /*      if( Pse()->IsSupported())
//                     ocli << " | pse  <eth-port>";*/

//         ocli << "reset {system | rf | switch}";
//     }
// };

////////////////////////////////////////////////////////////////////

// static void CopySwFromUrl (CliContext* contextP, bool IsProgramActiveSwBankImage = false)
// {
//     ValueStringT<MAX_CA_URL_LEN> valueUrl;
//     *contextP >> valueUrl;
//     if ( contextP->CheckExit(true) )
//         return;

//     if ( !contextP->IsEolExe( ) ) {
//         DICTIONARY_1(Modifier, 1, "if-version-differs-from") modifier;
//         ValueDictExternal valueModifier(&modifier);

//         ValueStringT<256> valueVersion;
//         *contextP >> valueModifier >> valueVersion >> END( );
//         if ( contextP->CheckExit(true) )
//             return;

//         if ( 0 == strcmp(valueVersion.GetP( ), MoSw::getSwVersionsData( ).activeSwName( )) ) {
//             ocli << "Equal, do nothing\n";
//             return;
//         }
//     }
//     if ( IsProgramActiveSwBankImage )
//         FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_COPY_SW1, valueUrl.view( ), "");
//     else
//         FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_COPY_SW, valueUrl.view( ), "");
// }

// static void CopyLicenceFromUrl (CliContext* contextP)
// {
//     ValueStringT<MAX_CA_URL_LEN> valueUrl;
//     *contextP >> valueUrl >> END( );
//     if ( contextP->CheckExit(true) )
//         return;

//     FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_COPY_LICENSE, valueUrl.view( ), "");
// }

// static void CopyDumpFromNpu (CliContext* contextP)
// {
//     ValueString64 valueFileName;

//     if ( contextP->IsEol( ) ) {
//         valueFileName.SetString("npu.dump");
//         *contextP >> END( );
//     } else {
//         *contextP >> valueFileName >> END( );
//     }
//     if ( contextP->CheckExit(true) )
//         return;

//     if ( MoNpu* npuP = Mo::FindMoT<MoNpu>(MoId(1)) ) {
//         npuP->_valueDumpFileName.SetString(valueFileName.GetP( ));
//         npuP->_valueDumpCounter.SetUint32(npuP->_valueDumpCounter.GetUint32( ) + 1);
//     }

//     ocli << "Done\n";
// }

// class CliCmdCopy : public CliCmd
// {
// public:
//     CliCmdCopy( ) : CliCmd(CMD_COPY, "copy") { }

//     DICTIONARY_6(FromPredefinedDict, 1, "running-configuration", 2, "startup-configuration", 3, "sw", 4, "sw1", 5, "license", 6, "npu");

//     void Run (CliContext* contextP) override
//     {
//         ValueDictT<FromPredefinedDict> valueFromPredefined;
//         ValueStringT<MAX_CA_URL_LEN>   valueFromUrl;

//         ValueUnionExternal valueFrom(&valueFromPredefined, &valueFromUrl);

//         *contextP >> valueFrom;
//         if ( contextP->CheckExit(true) )
//             return;

//         if ( valueFrom == "running-configuration" ) {
//             DICTIONARY_2(ToDict, 1, "startup-configuration", 2, "display") toDict;
//             ValueDictExternal valueTo(&toDict);
//             valueTo.SetCli(contextP);
//             if ( contextP->CheckExit(true) )
//                 return;

//             if ( valueTo == "startup-configuration" ) {
//                 if ( IsSpectrumTestRunning( ) ) {
//                     cliP->ReturnErrorAndPrint(RetStatus {"Can't save configuration while spectrum analyzer is running. Operation failed."});
//                     return;
//                 }
//                 ConfigCreate( );
//                 MoSystem* systemP = (MoSystem*)Mo::FindMo(MO_SYSTEM);
//                 systemP->_valueConfigSaved.SetUint32(E_AGENT_TRUE);

//             } else if ( valueTo == "display" )
//                 ConfigCreate(true);
//         } else if ( valueFrom == "startup-configuration" ) {
//             DICTIONARY_1(ToDict, 2, "display") toDict;
//             ValueDictExternal valueTo(&toDict);
//             valueTo.SetCli(contextP);
//             if ( contextP->CheckExit(true) )
//                 return;

//             ConfigDumpToCli( );
//         } else if ( valueFrom == "sw" ) {
//             CopySwFromUrl(contextP);
//         } else if ( valueFrom == "sw1" ) {
//             CopySwFromUrl(contextP, true);
//         } else if ( valueFrom == "license" ) {
//             CopyLicenceFromUrl(contextP);
//         } else if ( valueFrom == "npu" ) {
//             CopyDumpFromNpu(contextP);
//         } else {
//             // copy file

//             ValueStringT<MAX_CA_URL_LEN> valueToUrl;
//             *contextP >> valueToUrl >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             bool toLocal = (valueToUrl.StartsWith("flash:") || valueToUrl.StartsWith("eprom:"));
//             if ( toLocal && valueToUrl.StartsWith("flash:cert") && valueToUrl.EndsWith(".csr") ) {
//                 cliP->ReturnErrorAndPrint(RetStatus {"Cannot overwrite certificate request"});
//                 return;
//             }

//             if ( toLocal )
//                 FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_COPY_REM2LOC, valueFromUrl.view( ), valueToUrl.view( ));
//             else
//                 FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_COPY_LOC2REM, valueToUrl.view( ), valueFromUrl.view( ));
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "copy running-configuration [startup-configuration | display]\n";
//         ocli << "copy startup-configuration display\n";
//         ocli << "copy sw <from-url> [if-version-differs-from <version>]\n";
//         ocli << "copy license <from-url>\n";
//         ocli << "copy <from-url> <to-url>\n";
//     }
// };

////////////////////////////////////////////////////////////

class ValueScheduled : public Value
{
public:
    void DoGetCli (MyOutStream* outP) override { outP->Print("%02d:%02d", _hour, _min); }

    void DoSetCli (WordReader* inP) override
    {
        if ( inP->HandleAutoComplete( ) )
            return;
        if ( !inP->Read( ).IsOk( ) )
            return;

        int numOf = sscanf(inP->_dst.AsStr( ), "%d:%d", &_hour, &_min);
        if ( !isdigit(inP->_dst.GetLast( )) )
            return inP->SetStatus(RetStatus {E_RetStatus::Parsing});
        if ( numOf != 2 )
            return inP->SetStatus(RetStatus {E_RetStatus::Parsing});
        if ( _hour > 23 )
            return inP->SetStatus(RetStatus {"invalid hour"});
        if ( _min > 59 )
            return inP->SetStatus(RetStatus {"invalid min"});
    }

    bool IsWordInCliPartOfList ( ) override { return false; }

    UINT32 _hour {0};
    UINT32 _min {0};
};

// class CliCmdRun : public CliCmd
// {
// public:
//     CliCmdRun( ) : CliCmd(CMD_RUN, "run") { }

//     void Run (CliContext* contextP) override
//     {
//         struct ValueRunType : public ValueDictInternalT<10> {
//             ValueRunType( )
//             {
//                 _dictionary.Add(1, "sw");
//                 _dictionary.Add(2, "script");
//                 _dictionary.Add(3, "convert-startup");
//             }
//         };

//         ValueRunType valueRunType;

//         *contextP >> valueRunType;
//         if ( contextP->CheckExit(true) )
//             return;

//         if ( valueRunType == "sw" ) {
//             DICTIONARY_3(SwRunMode, E_SwRunMode::Immediate, "immediate", E_SwRunMode::NextReset, "next-rst", E_SwRunMode::Scheduled, "scheduled");
//             SwRunMode         swRunMode;
//             ValueDictExternal valueSwRunMode(&swRunMode);

//             *contextP >> valueSwRunMode;
//             E_SwRunMode runMode = valueSwRunMode.GetEnum<E_SwRunMode>( );

//             if ( contextP->CheckExit(true) )
//                 return;

//             ValueScheduled valueScheduledTime;

//             if ( runMode == E_SwRunMode::Scheduled ) {
//                 *contextP >> valueScheduledTime;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }

//             DICTIONARY_1(NoTimeOut, 1, "no-timeout") noTimeOut;
//             ValueDictExternal valueNoTimeOut(&noTimeOut);

//             ValueLongRangeT<300, 30000> valueTimeoutSec;
//             ValueUnionExternal          valueAcceptTimeout(&valueNoTimeOut, &valueTimeoutSec);

//             if ( runMode != E_SwRunMode::Scheduled ) {
//                 *contextP >> valueAcceptTimeout;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }

//             bool doConfigConversion = false;

//             DictionaryT<5> extraModifiersTable;
//             extraModifiersTable.Add(1, "if-version-differs-from").Add(2, "convert-configuration");

//             while ( (!contextP->IsEolExe( )) && (!extraModifiersTable.IsEmpty( )) ) {
//                 ValueDictExternal valueModifier(&extraModifiersTable);
//                 *contextP >> valueModifier;
//                 if ( contextP->CheckExit(true) )
//                     return;

//                 if ( valueModifier == 1 ) {
//                     ValueStringT<256> valueVersion;
//                     *contextP >> valueVersion;
//                     if ( contextP->CheckExit(true) )
//                         return;
//                     if ( strcmp(valueVersion.GetP( ), MoSw::getSwVersionsData( ).activeSwName( )) == 0 ) {
//                         ocli << "Equal, do nothing\n";
//                         return;
//                     }
//                 } else  // 2
//                 {
//                     doConfigConversion = true;
//                 }
//                 extraModifiersTable.Del(valueModifier.GetUint32( ));
//             }

//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             if ( doConfigConversion ) {
//                 RetStatus convertResult = ConvertConfigurations(ReadActiveVersion( ), ReadPassiveVersion( ));
//                 if ( !convertResult.ok( ) ) {
//                     cliP->ReturnErrorAndPrint(convertResult);
//                     return;
//                 }
//             }

//             MyStringT<128> local;
//             local << valueSwRunMode;
//             if ( runMode == E_SwRunMode::Scheduled )
//                 local << " " << valueScheduledTime;
//             local << " " << valueAcceptTimeout;

//             if ( std::string declainReasoon = Sys( )->checkVersionAllowed(MoSw::getSwVersionsData( ).passiveSwVer( )); !declainReasoon.empty( ) ) {
//                 ocli << declainReasoon.c_str( ) << "\n";
//                 return;
//             }

//             FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_SW_RUN, "", local);
//         } else if ( valueRunType == "script" ) {
//             MoScript::ReloadScriptsList( );

//             ValueScriptName valueScriptName;

//             *contextP >> valueScriptName;
//             if ( contextP->CheckExit(true) )
//                 return;

//             MyStringT<MAX_CA_URL_LEN> localPath;
//             localPath << valueScriptName;

//             char* scriptParams = nullptr;
//             if ( contextP->IsExeMode( ) ) {
//                 if ( !contextP->IsEol( ) ) {
//                     scriptParams = contextP->_originalLine.AsStr( ) + contextP->GetPos( );
//                     localPath << " " << scriptParams;
//                 }
//             } else {
//                 *contextP >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }

//             FtpControl::Self( )->StartActionRunScript(FtpControl::SOURCE_CLI, localPath.AsStr( ));
//         } else if ( valueRunType == "convert-startup" ) {
//             RetStatus status = ConvertConfigurations(ReadActiveVersion( ), ReadPassiveVersion( ));
//             ocli << (status.ok( ) ? "Converted." : status.message( )) << "\n";
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "run sw {immediate | next-rst | {scheduled <HH:MM>}} [{<accept-timeout-sec> | no-timeout}] [if-version-differs-from <version>] [convert-configuration]\n";
//         ocli << "\tcannot use accept-timeout for scheduled\n";
//         ocli << "run script <script-name> [<arguments>]  \n\twhere <arguments> - optional arguments in format \'name=value  name=value ...\'\n";
//         ocli << "run convert-startup\n";
//     }
// };

////////////////////////////////////////////////////////////

class CliCmdDir : public CliCmd
{
public:
    CliCmdDir( ) : CliCmd(CMD_DIR, "dir") { }

    void Run (CliContext* contextP) override
    {
        ValueStringT<128> valueUrl;

        *contextP >> valueUrl >> END( );
        if ( contextP->CheckExit(true) )
            return;

        // FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_SFS_DIR, "", valueUrl.view( ));
    }

    void PrintCmdHelp ( ) override { ocli << "dir <device:>\n"; }
};

////////////////////////////////////////////////////////////

// T_Ret NpuUpdateHostMaxPacketsPerSecond(UINT32 val, UINT32 destIp, UINT16 destPort);

// class CliCmdIperf : public CliCmd
// {
// public:
//     CliCmdIperf( ) : CliCmd(CMD_IPERF, "iperf") { }

//     void Run (CliContext* contextP) override
//     {
//         bool                               isClient = false;
//         bool                               isServer = false;
//         bool                               isUdp    = false;
//         ValueIp                            valueHost;
//         ValueLongRangeT<1024, 49151, 5201> valuePort;
//         ValueLongRangeT<1, 86400, 60>      valueTime;

//         if ( contextP->HandleAutoComplete( ) )
//             return;

//         while ( !contextP->IsEnter( ) ) {
//             if ( contextP->TestNext("-c") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp;
//                 isClient = true;
//             } else if ( contextP->TestNext("-s") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp;
//                 isServer = true;
//             } else if ( contextP->TestNext("-t") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueTime;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( contextP->TestNext("-u") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp;
//                 isUdp = true;
//             } else if ( contextP->TestNext("-p") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valuePort;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else {
//                 *contextP >> valueHost;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }
//         }

//         if ( (!isClient && !isServer) || (isClient && isServer) || (isClient && valueHost.GetUint32( ) == 0) ) {
//             *contextP >> valueHost;
//             if ( contextP->CheckExit(true) )
//                 return;
//         }

//         MyStringT<128> linuxCmd;

//         linuxCmd << "/usr/bin/iperf";

//         //-i seconds between periodic bandwidth reports
//         if ( isClient ) {
//             linuxCmd << " -c " << valueHost << " -i 1 "
//                      << " -t " << valueTime << " ";

//         } else
//             linuxCmd << " -s -i 1 ";

//         //-l length of buffer to read or write
//         //-b for UDP, bandwidth to send at in bits/sec
//         //-P number of parallel client threads to run
//         if ( isClient ) {
//             if ( isUdp ) {
//                 linuxCmd << " -u -l 4K ";
//                 linuxCmd << "-b 2000M ";
//             }

//         } else {
//             if ( isUdp ) {
//                 linuxCmd << " -u -l 4K ";
//             }
//         }

//         if ( valuePort != 5201 )
//             linuxCmd << " -p " << valuePort;

//         linuxCmd << " > ";

//         // change host max pps and suspend top cpu consumers 1)rssi.lua 2)home/sw/bin/rfd"
//         T_Ret status = NpuUpdateHostMaxPacketsPerSecond(1, valueHost.GetUint32( ), valuePort.GetUint32( ));

//         if ( status == RET_OK ) {
//             pid_t iperfPid = StartIperfCliProcess(linuxCmd.AsStr( ));

//             if ( iperfPid == -1 ) {
//                 cliP->SetStatus(RetStatus {"Connect failed"});
//                 cliP->CheckExit(true);
//                 NpuUpdateHostMaxPacketsPerSecond(0, 0, 0);
//             } else {
//                 const char* errorP = Client(GetClientId( ))->_repeatStack.push<LinuxCliIperfProcessRepeater>(iperfPid);
//                 if ( errorP != nullptr ) {
//                     cliP->SetStatus(RetStatus {"Cannot run process"});
//                     cliP->CheckExit(true);
//                     NpuUpdateHostMaxPacketsPerSecond(0, 0, 0);
//                 }
//             }
//         } else {
//             cliP->ReturnErrorAndPrint(RetStatus {"Iperf is already running. Operation failed."});
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "iperf <mode> [-t <time>] [-u] [-p <port>]\n\n"
//                 "<mode> : -s | -c <host> , where -s is for server mode and -c <host> is for client mode, connecting to <host>\n"
//                 "-u     : use UDP rather than TCP\n"
//                 "port   : server port to listen on/connect to. Default is 5201\n"
//                 "time   : for client only 1..86400 (sec). Default is 60";
//     }
// };

////////////////////////////////////////////////////////////

// class CliCmdPing6 : public CliCmd
// {
// public:
//     CliCmdPing6( ) : CliCmd(CMD_PING6, "ping6") { }

//     void Run (CliContext* contextP) override
//     {
//         ValueLongRangeT<1, 32000, 5> valueNumOf;
//         bool                         numOfForever = false;
//         ValueLongRangeT<0, 5000, 56> valueLength;
//         ValueIpV6                    valueHost;

//         if ( contextP->HandleAutoComplete( ) )
//             return;

//         while ( !contextP->IsEnter( ) ) {
//             if ( contextP->TestNext("-c") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueNumOf;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( contextP->TestNext("-t") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp;
//                 numOfForever = true;
//             } else if ( contextP->TestNext("-l") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueLength;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else {
//                 *contextP >> valueHost;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }
//         }

//         if ( valueHost.GetIpV6( ) == IpV6Address::ZERO ) {
//             *contextP >> valueHost;
//             if ( contextP->CheckExit(true) )
//                 return;
//         }

//         MyStringT<128> linuxCmd;
//         linuxCmd << "/bin/ping6";
//         if ( !numOfForever )
//             linuxCmd << " -c " << valueNumOf;
//         linuxCmd << " -s " << valueLength << " " << valueHost;

//         pid_t pingPid = StartLinuxCliProcess(linuxCmd.AsStr( ));

//         const char* errorP = Client(GetClientId( ))->_repeatStack.push<LinuxCliProcessRepeater>(pingPid);
//         if ( errorP != nullptr ) {
//             cliP->SetStatus(RetStatus {"Cannot run process"});
//             cliP->CheckExit(true);
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "ping6 [-c <num-packets 1..32000>] [-t] [-l <packet-length 0..5000>] <host>\n"
//                 "      -t - ping until stopped by ctrl/c\n";
//     }
// };

// class CliCmdPing : public CliCmd
// {
// public:
//     CliCmdPing( ) : CliCmd(CMD_PING, "ping") { }

//     void Run (CliContext* contextP) override
//     {
//         ValueLongRangeT<1, 32000, 5> valueNumOf;
//         bool                         numOfForever = false, flood = false;
//         ValueLongRangeT<0, 5000, 56> valueLength;
//         ValueIp                      valueHost;

//         if ( contextP->HandleAutoComplete( ) )
//             return;

//         while ( !contextP->IsEnter( ) ) {
//             if ( contextP->TestNext("-c") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueNumOf;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( contextP->TestNext("-t") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp;
//                 numOfForever = true;
//             } else if ( contextP->TestNext("-l") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueLength;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( contextP->TestNext("-f") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp;
//                 flood = true;
//             } else {
//                 *contextP >> valueHost;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }
//         }

//         if ( valueHost.GetUint32( ) == 0 ) {
//             *contextP >> valueHost;
//             if ( contextP->CheckExit(true) )
//                 return;
//         }

//         MyStringT<128> linuxCmd;
//         linuxCmd << "/bin/ping";
//         if ( !numOfForever )
//             linuxCmd << " -c " << valueNumOf;
//         if ( flood )
//             linuxCmd << " -f ";
//         linuxCmd << " -s " << valueLength << " " << valueHost;

//         pid_t pingPid = StartLinuxCliProcess(linuxCmd.AsStr( ));

//         const char* errorP = Client(GetClientId( ))->_repeatStack.push<LinuxCliProcessRepeater>(pingPid);
//         if ( errorP != nullptr ) {
//             cliP->SetStatus(RetStatus {"Cannot run process"});
//             cliP->CheckExit(true);
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "ping [-c <num-packets 1..32000>] [-t] [-f] [-l <packet-length 0..5000>] <host>\n"
//                 "     -t - ping until stopped by ctrl/c\n";
//     }
// };

////////////////////////////////////////////////////////////

// class CliCmdSpectrum : public CliCmd
// {
// public:
//     CliCmdSpectrum( ) : CliCmd(CMD_SPECTRUM, "spectrum-analyzer") { }

//     void Run (CliContext* contextP) override
//     {
//         DICTIONARY_3(RunType, 1, "start", 2, "abort", 3, "results") runType;
//         ValueDictExternal valueRunType(&runType);
//         *contextP >> valueRunType;
//         if ( contextP->CheckExit(true) )
//             return;
//         MoRf* rfP = (MoRf*)Mo::FindMo(MO_RF);
//         if ( valueRunType == "start" ) {
//             struct ValueWidthOrFreq : public ValueDictInternalT<100> {
//                 char _allWidthName[10][20];

//                 ValueWidthOrFreq( )
//                 {
//                     DictionaryT<10> widthDict = GetChannelWidthDictionary( );
//                     UINT32          context;
//                     MoRf*           rfP = (MoRf*)Mo::FindMo(MO_RF);
//                     for ( widthDict.___FirstEntry(&context); widthDict.___GetEntry(context).IsValid( ); widthDict.___NextEntry(&context) ) {
//                         if ( (rfP != nullptr) && (rfP->GetCountry( ) == COUNTRY_JP) && (MyStringT<10>(widthDict.___GetEntry(context)._nameP) == MyStringT<10>("1250")) )
//                             continue;
//                         sprintf(_allWidthName[context], "all-%sMhz", widthDict.___GetEntry(context)._nameP);
//                         _dictionary.Add(widthDict.___GetEntry(context)._value, _allWidthName[context]);
//                     }

//                     DictionaryT<100> freqDict = GetFrequencyDictionary(FindMoRf( )->GetWidth( ));
//                     _dictionary.Add(&freqDict);
//                 }
//             };

//             ValueWidthOrFreq valueWidthOrFreq;
//             contextP->_helper.HelpAssignValue(&valueWidthOrFreq);

//             *contextP >> valueWidthOrFreq >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             RetStatus status = rfP->StartSpectrum(valueWidthOrFreq.GetUint32( ));
//             if ( !status.ok( ) ) {
//                 cliP->SetStatus(status);
//                 cliP->CheckExit(true);
//                 return;
//             }

//             ocli << "Start analyze.\nDo not change or save configuration till process termination !!!\n";
//         } else if ( valueRunType == "results" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             rfP->ShowSpectrum(contextP);
//         } else if ( valueRunType == "abort" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             rfP->StopSpectrum( );
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "spectrum-analyzer start <frequencies>\n";
//         ocli << "spectrum-analyzer abort\n";
//         ocli << "spectrum-analyzer results\n";
//     }
// };

////////////////////////////////////////////////////////////

// class CliCmdTracert6 : public CliCmd
// {
// public:
//     CliCmdTracert6( ) : CliCmd(CMD_TRACERT6, "tracert6") { }

//     void Run (CliContext* contextP) override
//     {
//         ValueLongRangeT<2, 255, 30>  valueMaxHops;     // In IPv6 min value is 2
//         ValueLongRangeT<2, 86400, 3> valueTimeoutSec;  // In IPv6 min value is 2
//         ValueIpV6                    valueHost;

//         while ( !contextP->IsEnter( ) ) {
//             if ( contextP->TestNext("-h") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueMaxHops;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( contextP->TestNext("-w") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueTimeoutSec;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else {
//                 *contextP >> valueHost;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }
//         }

//         if ( valueHost.GetIpV6( ) == IpV6Address::ZERO ) {
//             *contextP >> valueHost;
//             if ( contextP->CheckExit(true) )
//                 return;
//         }

//         MyStringT<128> linuxCmd;
//         linuxCmd << "/bin/traceroute6";
//         linuxCmd << " -m " << valueMaxHops << " -w " << valueTimeoutSec << " " << valueHost;

//         pid_t tracertPid = StartLinuxCliProcess(linuxCmd.AsStr( ));

//         const char* errorP = Client(GetClientId( ))->_repeatStack.push<LinuxCliProcessRepeater>(tracertPid);
//         if ( errorP != nullptr ) {
//             cliP->SetStatus(RetStatus {"Cannot run process"});
//             cliP->CheckExit(true);
//         }
//     }

//     void PrintCmdHelp ( ) override { ocli << "tracert6 [-h <maximum-hops 2..255>] [-w <timeout, sec, 2..86400>] <host>\n"; }
// };

// class CliCmdTracert : public CliCmd
// {
// public:
//     CliCmdTracert( ) : CliCmd(CMD_TRACERT, "tracert") { }

//     void Run (CliContext* contextP) override
//     {
//         ValueLongRangeT<1, 255, 30>  valueMaxHops;
//         ValueLongRangeT<1, 86400, 3> valueTimeoutSec;
//         ValueIp                      valueHost;

//         while ( !contextP->IsEnter( ) ) {
//             if ( contextP->TestNext("-h") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueMaxHops;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( contextP->TestNext("-w") ) {
//                 ValueStringT<128> valueTmp;
//                 *contextP >> valueTmp >> valueTimeoutSec;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else {
//                 *contextP >> valueHost;
//                 if ( contextP->CheckExit(true) )
//                     return;
//             }
//         }

//         if ( valueHost.GetUint32( ) == 0 ) {
//             *contextP >> valueHost;
//             if ( contextP->CheckExit(true) )
//                 return;
//         }

//         MyStringT<128> linuxCmd;
//         linuxCmd << "/usr/bin/traceroute";
//         linuxCmd << " -m " << valueMaxHops << " -w " << valueTimeoutSec << " " << valueHost;

//         pid_t tracertPid = StartLinuxCliProcess(linuxCmd.AsStr( ));

//         const char* errorP = Client(GetClientId( ))->_repeatStack.push<LinuxCliProcessRepeater>(tracertPid);
//         if ( errorP != nullptr ) {
//             cliP->SetStatus(RetStatus {"Cannot run process"});
//             cliP->CheckExit(true);
//         }
//     }

//     void PrintCmdHelp ( ) override { ocli << "tracert [-h <maximum-hops 1..255>] [-w <timeout, sec, 1..86400>] <host>\n"; }
// };

////////////////////////////////////////////////////////////

class CliCmdAccept : public CliCmd
{
public:
    CliCmdAccept( ) : CliCmd(CMD_ACCEPT, "accept") { }

    void Run (CliContext* contextP) override
    {
        DICTIONARY_1(AcceptType, 1, "sw") acceptType;
        ValueDictExternal valueAcceptType(&acceptType);

        *contextP >> valueAcceptType >> END( );
        if ( contextP->CheckExit(true) )
            return;

        // FtpControl::Self( )->StartAction(FtpControl::SOURCE_CLI, CMD_SW_ACCEPT, "", "");

        ocli << "Done\n";
    }

    void PrintCmdHelp ( ) override { ocli << "accept sw\n"; }
};

///////////////////////////////////////////////////////////

class CliCmdDebug : public CliCmd
{
public:
    CliCmdDebug( ) : CliCmd(CMD_DEBUG, "debug") { }

    void Run (CliContext* contextP) override
    {
        DICTIONARY_2(Modifier, 1, "on", 2, "off") modifier;
        ValueDictExternal valueModifier(&modifier);
        valueModifier.SetCli(contextP);
        if ( contextP->CheckExit(true) )
            return;
        *DebugModeP( ) = (valueModifier == "on");
    }

    void PrintCmdHelp ( ) override
    {
        if ( IsDebugMode( ) )
            ocli << "debug [on | off]";
    }
};

// class CliCmdNpu : public CliCmd
// {
// public:
//     CliCmdNpu( ) : CliCmd(CMD_NPU, "npu", true) { }

//     void Run (CliContext* contextP) override
//     {
//         ValueLong valueParam_1;
//         valueParam_1.SetCli(contextP);
//         if ( contextP->CheckExit(true) )
//             return;

//         UINT32 param_1 = valueParam_1.GetUint32( );
//         UINT32 param_2 = 0;

//         if ( !contextP->IsEol( ) ) {
//             ValueLong valueParam_2;
//             valueParam_2.SetCli(contextP);
//             if ( contextP->CheckExit(true) )
//                 return;

//             param_2 = valueParam_2.GetUint32( );
//         }

//         *contextP >> END( );
//         if ( contextP->CheckExit(true) )
//             return;

//         (void)param_1;
//         (void)param_2;
//     }

//     void PrintCmdHelp ( ) override
//     {
//         if ( IsDebugMode( ) )
//             ocli << "npu <number> [<number>]";
//     }
// };

// class CliCmdCfm : public CliCmd
// {
// public:
//     CliCmdCfm( ) : CliCmd(CMD_CFM, "cfm", true) { }

//     void Run (CliContext* contextP) override
//     {
//         ValueLong v_1;
//         ValueLong v_2;
//         ValueLong v_3;
//         ValueLong v_4;
//         ValueLong v_5;

//         *contextP >> v_1 >> v_2 >> v_3 >> v_4 >> v_5 >> END( );
//         if ( contextP->CheckExit(true) )
//             return;

//         void CfmApi_CfmDebug(UINT32 command, UINT32 data, UINT32 md_index, UINT32 ma_index, UINT32 mep_id);

//         CfmApi_CfmDebug(v_1.GetUint32( ), v_2.GetUint32( ), v_3.GetUint32( ), v_4.GetUint32( ), v_5.GetUint32( ));
//     }

//     void PrintCmdHelp ( ) override
//     {
//         if ( IsDebugMode( ) )
//             ocli << "cfm <number_1> <number_2> <number_3> <number_4> <number_5>";
//     }
// };

// class CliCmdDebugFlag : public CliCmd
// {
// public:
//     CliCmdDebugFlag( ) : CliCmd(CMD_DEBUG_FLAG, "debug-flag", true) { }

//     DICTIONARY_3(ModifierDict, 1, "save", 2, "show", 3, "set");

//     void Run (CliContext* contextP) override
//     {
//         ValueDictT<ModifierDict> valueModifier;
//         valueModifier.SetCli(contextP);
//         if ( contextP->CheckExit(true) )
//             return;

//         if ( valueModifier == "save" ) {
//             FileWrapper file(FILE_LOG_CONFIG, "w");
//             if ( file.is_open( ) ) {
//                 DebugRegion* debugRegion = DebugRegion::Self( );

//                 for ( UINT32 i = 0; i < sizeof(debugRegion->debugFlags) / sizeof(debugRegion->debugFlags[0]); i++ ) {
//                     if ( debugRegion->debugFlags[i].isExist ) {
//                         MyStringT<64> tmp;
//                         fmt::print(file, "{} {}\n", debugRegion->debugFlags[i].name, debugRegion->debugFlags[i].isActive ? 1 : 0);
//                         ocli << "\nSave " << debugRegion->debugFlags[i].name << " " << (debugRegion->debugFlags[i].isActive ? "enabled" : "disabled");
//                     }
//                 }
//                 ::sync( );
//                 ocli << "\nDone";
//             } else
//                 ocli << "\nError";
//             ocli << "\n";
//         } else if ( valueModifier == "show" ) {
//             ocli << "\n";
//             for ( int i = 0; i < DEBUG_FLAG_MAX; i++ ) {
//                 DebugRegion* debugRegion = DebugRegion::Self( );

//                 char* nameP     = debugRegion->debugFlags[i].name;
//                 char* isActiveP = debugRegion->debugFlags[i].isActive ? (char*)"enabled" : (char*)"disabled";
//                 char* descrP    = debugRegion->debugFlags[i].descr;
//                 ocli << FIELD(nameP, 40) << " = " << FIELD(isActiveP, 10) << "(" << descrP << ")\n";
//             }
//             ocli << "\n";
//         } else if ( valueModifier == "set" ) {
//             DebugRegion* debugRegion = DebugRegion::Self( );

//             DictionaryT<DEBUG_FLAG_MAX> nameDict;
//             for ( int i = 0; i < DEBUG_FLAG_MAX; i++ )
//                 nameDict.Add(i, debugRegion->debugFlags[i].name);

//             ValueDictExternal valueName(&nameDict);
//             valueName.SetCli(contextP);
//             if ( contextP->CheckExit(true) )
//                 return;

//             DICTIONARY_2(Enabled, 1, "enabled", 2, "disabled") enabled;
//             ValueDictExternal valueEnabled(&enabled);
//             valueEnabled.SetCli(contextP);
//             if ( contextP->CheckExit(true) )
//                 return;

//             int id                               = valueName.GetUint32( );
//             debugRegion->debugFlags[id].isActive = valueEnabled == "enabled";
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "debug-flag save"
//              << "\n"
//              << "debug-flag show"
//              << "\n"
//              << "debug-flag set <flagName> [enabled | disabled]";
//     }
// };

// class CliCmdNpuDebug : public CliCmd
// {
// public:
//     CliCmdNpuDebug( ) : CliCmd(CMD_DEBUG_NPU, "debug-npu", true) { }

//     DICTIONARY_3(ModifierDict, 1, "on", 0, "off", 2, "show");

//     void Run (CliContext* contextP) override
//     {
//         ValueDictT<ModifierDict> valueModifier;
//         valueModifier.SetCli(contextP);
//         if ( contextP->CheckExit(true) )
//             return;

//         std::ignore = SkMessage::SkMessage(SkMessage::E_QueueId::MSG_CAD_API_Q).send(SkMessage::E_QueueId::MSG_NPU_Q, E_NPU_DEBUG, static_cast<int>(valueModifier.GetUint32( )));
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "debug-npu on"
//              << "\n"
//              << "debug-npu off"
//              << "\n"
//              << "debug-npu show"
//              << "\n";
//     }
// };

template<int T>
void AddMoTypes (DictionaryT<T>* dstP)
{
    for ( int i = 0; i < MO_MAX; i++ ) {
        Mo* moP = SampleMo((T_MoType)i);
        if ( moP == nullptr )
            continue;

        if ( i == MO_SFS )
            continue;

        if ( !isVisibleMo((T_MoType)i) )
            continue;
        dstP->Add(moP->Type( ), moP->TypeName( ));
    }
}

// extern void SnmpDump(CliOutStream* dstP);
// extern void SnmpTest(CliOutStream* dstP);

// class CliCmdDebugAction : public CliCmd
// {
// public:
//     CliCmdDebugAction( ) : CliCmd(CMD_DEBUG_ACTION, "debug-action", true) { }

//     void Run (CliContext* contextP) override
//     {
//         DICTIONARY_7(Modifier, 1, "mo", 2, "internal-fdb-table", 4, "event", 5, "snmp-dump", 6, "snmp-test", 7, "pipe-dump", 8, "set-license-remain-time-sec") modifier;
//         ValueDictExternal valueModifier(&modifier);
//         valueModifier.SetCli(contextP);
//         if ( contextP->CheckExit(true) )
//             return;

//         if ( valueModifier == "set-license-remain-time-sec" ) {
//             struct ValueLicenseType : public ValueDictionary {
//                 Dictionary* GetDictionary ( ) override
//                 {
//                     static DictionaryT<15> dict;
//                     if ( dict.IsEmpty( ) ) {
//                         for ( LicenseController::Iterator it = LicenseController::Self( )->Begin( ); it.IsValid( ); it++ )
//                             dict.Add(it->GetType( ), it->GetName( ));
//                     }
//                     return &dict;
//                 }
//             };

//             ValueLicenseType valueLicenseType;
//             ValueLong        valueLicenseRemainTimeSec;
//             *contextP >> valueLicenseType >> valueLicenseRemainTimeSec;
//             if ( contextP->CheckExit(true) )
//                 return;

//             LicenseController::Self( )->_licenseTable[valueLicenseType.GetUint32( )]->_trialTime.SetRemainTime(valueLicenseRemainTimeSec.GetUint32( ));
//             LicenseController::Self( )->WriteTrialFile( );
//             return;
//         }
//         if ( valueModifier == "internal-fdb-table" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             //            MoFdbEntry::PrintInternal( & ocli);
//             return;
//         }
//         if ( valueModifier == "snmp-dump" ) {
//             SnmpDump(&ocli);
//             return;
//         }
//         if ( valueModifier == "snmp-test" ) {
//             SnmpTest(&ocli);
//             return;
//         }
//         if ( valueModifier == "event" ) {
//             ValueDictInternalT<20> valueEvent;
//             valueEvent._dictionary.Add(1, "cold-start");
//             valueEvent._dictionary.Add(2, "link");
//             valueEvent._dictionary.Add(4, "temperature");
//             valueEvent._dictionary.Add(5, "sfp");
//             valueEvent._dictionary.Add(6, "ref-clock");
//             valueEvent._dictionary.Add(7, "cfm-fault");
//             valueEvent._dictionary.Add(8, "cfm-mip");
//             valueEvent._dictionary.Add(9, "rx-ql-eec1");
//             valueEvent._dictionary.Add(10, "dynamic-rem-station");
//             valueEvent._dictionary.Add(11, "static-rem-station");

//             *contextP >> valueEvent;
//             if ( contextP->CheckExit(true) )
//                 return;

//             if ( valueEvent == "cold-start" ) {
//                 *contextP >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;
//             } else if ( valueEvent == "link" ) {
//                 ValueEthernetPort port;

//                 DICTIONARY_2(OperatState, E_OperationalState::Up, "up", E_OperationalState::Down, "down") operatState;
//                 ValueDictExternal valueOperatState(&operatState);

//                 *contextP >> port >> valueOperatState >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;
//                 SendEventLinkStateEthernet(port.GetEthNum( ), valueOperatState.GetEnum<E_OperationalState>( ) == E_OperationalState::Up ? E_ETH_ALARM_EVENT_LINK_UP : E_ETH_ALARM_EVENT_LINK_DOWN);
//             } else if ( valueEvent == "temperature" ) {
//                 DICTIONARY_2(Temper, E_FALSE, "normal", E_TRUE, "high") temper;
//                 ValueDictExternal valueTemper(&temper);
//                 *contextP >> valueTemper >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;

//                 SendEventTemperature((T_BOOL)valueTemper.GetUint32( ));
//             } else if ( valueEvent == "sfp" ) {
//                 ValueEthernetPort port;

//                 DICTIONARY_2(Inserted, E_FALSE, "out", E_TRUE, "in") inserted;
//                 ValueDictExternal valueInserted(&inserted);

//                 *contextP >> port >> valueInserted >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;

//                 SendEventSfp(port.GetEthNum( ), (T_BOOL)valueInserted.GetUint32( ));
//             }
//             return;
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         if ( IsDebugMode( ) ) {
//             ocli << "debug-action snmp-dump\n";
//             ocli << "debug-action pipe-dump\n";
//             ocli << "debug-action snmp-test\n";
//             ocli << "debug-action internal-fdb-table\n";
//             ocli << "debug-action set-license-remain-time-sec <license-type> <remain-time-sec>\n";

//             ocli << "\n"
//                  << "debug-action event cold-start\n";
//             ocli << "debug-action event link {host | eth0 | eth1 | eth2} {up | down}\n";
//             ocli << "debug-action event modulation {qpsk | qam16 | qam64} {1 | 2 | 3 | 4} {1 | 2 | 4} {0.5 | 0.67 | 0.8}\n";
//             ocli << "debug-action event temperature {high | normal}\n";
//             ocli << "debug-action event sfp {host | eth0 | eth1 | eth2} {in | out}\n";
//             ocli << "debug-action event ref-clock {host | eth0 | eth1 | eth2}\n";
//             ocli << "debug-action event synthesizer {lock | unlock}\n";
//             ocli << "debug-action event poe-low {host | eth0 | eth1 | eth2} {normal | low}\n";
//             ocli << "debug-action event poe-incompatible {host | eth0 | eth1 | eth2} {normal | incompatible}\n";
//         }
//     }
// };

///////////////////////////////////////////////////////////////////////////

static int CompareHelpEntries (const void* firstP, const void* secondP)
{
    return ((MyStringT<32>*)firstP)->Compare(*((MyStringT<32>*)secondP));
}

void PrintSortedHelp (MyStringT<32>* namesP, int numOf)
{
    qsort(namesP, numOf, sizeof(namesP[0]), CompareHelpEntries);

    int maxLen = 0;
    for ( int i = 0; i < numOf; i++ ) {
        int len = namesP[i].GetLen( );
        if ( len > maxLen )
            maxLen = len;
    }
    MyStringT<16> myFormat;
    myFormat << "%%-" << (maxLen + 2) << "s";

    for ( int i = 0; i < numOf; i++ ) {
        if ( ((i % 4) == 0) && (i != 0) )
            ocli << "\n";
        MyStringT<32> tmp;
        tmp << namesP[i];
        if ( i != numOf - 1 )
            tmp << ", ";
        ocli.Print(myFormat.AsStr( ), tmp.AsStr( ));
    }
    ocli << "\n";
}

// struct PrintLogRepeater : public CadRepeater {
//     explicit PrintLogRepeater(bool isAudit) : CadRepeater(NO_SLEEP), _isAudit(isAudit) { }

//     bool _isAudit;
//     int  _fileIndex {20};

//     void CallMe ( ) override
//     {
//         if ( _fileIndex < 0 ) {
//             fLOG_EX_DEBUG(false, "file index is negative - exit");
//             EndMe( );
//             return;
//         }

//         MyStringT<32> name;
//         struct stat   buffer;

//         if ( _fileIndex == 20 ) {
//             for ( _fileIndex = 20; _fileIndex >= 0; _fileIndex-- ) {
//                 name << (_isAudit ? AUDIT_LOG_FILE_PATH : EVENT_LOG_FILE_PATH);

//                 if ( _fileIndex != 0 ) {
//                     name << "." << _fileIndex;
//                 }

//                 if ( stat(name.AsStr( ), &buffer) == 0 )  // If File exist the break from loop
//                     break;
//                 name.Clean( );
//             }
//         } else {
//             name << (_isAudit ? AUDIT_LOG_FILE_PATH : EVENT_LOG_FILE_PATH);

//             if ( _fileIndex != 0 )
//                 name << "." << _fileIndex;
//         }

//         if ( FileWrapper fp(name.AsStr( ), "r"); fp.is_open( ) ) {
//             fLOG_EX_DEBUG(false, "Using log file {}", fp);
//             char tmp[200];
//             while ( fgets(tmp, sizeof(tmp) - 1, fp) )
//                 ocli << tmp;
//         }
//         _fileIndex--;
//     }
// };

/* Read CPU temperature. return E_Status::Ok on successes, else failure  */
// E_Status show_cpu_temperature ( )
// {
//     std::ifstream thermalFile("/sys/class/thermal/thermal_zone0/temp");

//     if ( !thermalFile.is_open( ) )
//         return E_Status::Error;

//     float temperature_val;
//     thermalFile >> temperature_val;

//     ocli.Print("%.3f celsius\n", temperature_val / 1000);

//     return E_Status::Ok;
// }

class CliCmdShow : public CliCmd
{
public:
    CliCmdShow( ) : CliCmd(CMD_SHOW, "show") { }

    void Run (CliContext* contextP) override
    {
        ValueDictInternalT<MO_MAX + 5> valueMoType;

        AddMoTypes(&valueMoType._dictionary);
        valueMoType._dictionary.Add(MO_MAX + 1, "loginsession");
        valueMoType._dictionary.Add(MO_MAX + 2, "log");
        valueMoType._dictionary.Add(MO_MAX + 3, "user-activity-log");
        valueMoType._dictionary.Add(MO_MAX + 5, "var");
        valueMoType._dictionary.Add(MO_MAX + 6, "frequency");
        valueMoType._dictionary.Add(MO_MAX + 7, "cpu-temperature");

        *contextP >> valueMoType;
        if ( contextP->CheckExit(true) )
            return;

        if ( valueMoType.GetUint32( ) < MO_MAX ) {
            T_MoType moType = (T_MoType)valueMoType.GetUint32( );
            cliP->_helper.HelpAssignMo(_cmdType, moType);

            Mo* sampleP         = SampleMo(moType);
            Mo::_cmdType        = CMD_SHOW;
            Mo::_numOfReadersId = 0;
            LOG("CMD_SHOW before");
            sampleP->DoHandleShow(ACTION_CLI_CMD);
            LOG("CMD_SHOW after");
        }
        // else if ( valueMoType == "loginsession" ) {
        //     MyStringT<64> tmp;
        //     tmp << "show loginsession [my | all]"
        //         << "\ndefault is my";
        //     cliP->_helper.HelpAssignFreeText(tmp.AsStr( ));

        //     enum ModifierType { SHOW_MY, SHOW_ALL } modifier = SHOW_MY;

        //     if ( !contextP->IsEol( ) || contextP->NowAutoComplete( ) ) {
        //         DICTIONARY_2(ModifierTable, SHOW_MY, "my", SHOW_ALL, "all") modifierTable;
        //         ValueDictExternal valueModifier(&modifierTable);
        //         *contextP >> valueModifier >> END( );
        //         if ( contextP->CheckExit(true) )
        //             return;
        //         modifier = (ModifierType)valueModifier.GetUint32( );
        //     }

        //     ocli << FIELD("Session ID", 15) << "Session Time"
        //          << "\n";

        //     for ( UINT32 i = 0; i < sizeof(regionP->_clientsCli) / sizeof(regionP->_clientsCli[0]); i++ ) {
        //         if ( regionP->_clientsCli[i]._clientPid == PID_NOT_EXIST )
        //             continue;
        //         if ( kill(regionP->_clientsCli[i]._clientPid, 0) != 0 )  // skip garbadge - just sends signal to process to check
        //             continue;

        //         if ( modifier == SHOW_MY && i != GetClientId( ) )
        //             continue;
        //         MyStringT<64> tmp;
        //         tmp << "0" << i;
        //         ocli << FIELD(tmp.AsStr( ), 15);

        //         UINT64 ticks = mygettime( ) - regionP->_clientsCli[i]._startTime;
        //         UINT32 days  = ticks / (60 * 60 * 24);
        //         ticks        = ticks % (60 * 60 * 24);

        //         UINT32 hrs = ticks / (60 * 60);
        //         ticks      = ticks % (60 * 60);

        //         UINT32 min = ticks / 60;
        //         ticks      = ticks % 60;

        //         UINT32 sec = ticks;

        //         tmp.Clean( );
        //         tmp.Print("%04d:%02d:%02d:%02d", days, hrs, min, sec);
        //         ocli << tmp.AsStr( );

        //         ocli << "\n";
        //     }
        // } else if ( valueMoType == "log" || valueMoType == "user-activity-log" ) {
        //     *contextP >> END( );
        //     if ( contextP->CheckExit(true) )
        //         return;

        //     //            ocli << "\n";

        //     bool isAudit = valueMoType != "log";

        //     UINT32 clientId = GetClientId( );

        //     std::ignore = Client(clientId)->_repeatStack.push<PrintLogRepeater>(isAudit);

        //     //            ocli << "\n";
        // } else if ( valueMoType == "var" ) {
        //     cliVars.ShowAll( );
        // } else if ( valueMoType == "frequency" ) {
        //     ocli << "\n";
        //     EnumCountry eepromCountry = CountryCode(Eeprom( )->SEEPROMGetCountryCode( ));
        //     for ( UINT32 i = 0; FreqEntry::GetEntry(i); i++ ) {
        //         FreqEntry* entry = FreqEntry::GetEntry(i);

        //         if ( eepromCountry != COUNTRY_NONE && eepromCountry != COUNTRY_WW && !entry->HasCountry(eepromCountry) )
        //             continue;

        //         ocli << fmt::format("frequency {} channel-width {:>{}}", entry->FreqAsName( ), entry->WidthAsName( ), 4 /*8*/).c_str( );

        //         if ( eepromCountry == COUNTRY_WW || eepromCountry == COUNTRY_NONE ) {
        //             forEachAvailableCountryCode([entry] (EnumCountry country) {
        //                 ocli << "  " << FIELD(entry->HasCountry(country) ? CountryName(country) : " ", strlen(CountryName(country)));
        //             });
        //         }
        //         ocli << "\n";
        //     }
        // } else if ( valueMoType == "cpu-temperature" && show_cpu_temperature( ) == E_Status::Error ) {
        //     fLOG_EX_ERR(false, "Fail to read CPU temperature");
        // }
    }

    void PrintCmdHelp ( ) override
    {
        MyStringT<32> names[MO_MAX + 20];
        int           numOf = 0;

        for ( int i = 0; i < MO_MAX; i++ ) {
            if ( i == MO_SFS )
                continue;
            if ( !isVisibleMo((T_MoType)i) )
                continue;
            Mo* moP = SampleMo((T_MoType)i);
            if ( moP != nullptr )
                names[numOf++] << moP->TypeName( );
        }
        names[numOf++] << "loginsession";
        names[numOf++] << "log";
        names[numOf++] << "user-activity-log";
        names[numOf++] << "var";
        names[numOf++] << "cpu-temperature";

        ocli << "show\n";
        PrintSortedHelp(names, numOf);
    }
};

////////////////////////////////////////////////

class CliCmdSet : public CliCmd
{
public:
    CliCmdSet( ) : CliCmd(CMD_SET, "set") { }

    void Run (CliContext* contextP) override
    {
        ValueDictInternalT<MO_MAX> valueMoType;
        AddMoTypes(&valueMoType._dictionary);
        valueMoType._dictionary.Add(MO_MAX + 1, "var");
        // set command not supported for FdbEntry because of ticket DEV-814 in JIRA
        valueMoType._dictionary.Del(MO_FDB_ENTRY);
        *contextP >> valueMoType;
        if ( contextP->CheckExit(true) )
            return;

        if ( valueMoType.GetUint32( ) < MO_MAX ) {
            T_MoType moType = (T_MoType)valueMoType.GetUint32( );
            cliP->_helper.HelpAssignMo(_cmdType, moType);

            Mo* sampleP         = SampleMo(moType);
            Mo::_cmdType        = CMD_SET;
            Mo::_numOfReadersId = 0;
            sampleP->DoHandleSet(ACTION_CLI_CMD);
        }
        // else if ( valueMoType == "var" ) {
        //     ValueStringT<128> valueName;
        //     ValueStringT<128> valueValue;

        //     *contextP >> valueName >> valueValue >> END( );
        //     if ( contextP->CheckExit(true) )
        //         return;

        //     RetStatus ret = cliVars.SetGlobal(valueName.GetP( ), valueValue.GetP( ));
        //     if ( !ret.ok( ) )
        //         cliP->ReturnErrorAndPrint(ret);
        // }
    }

    void PrintCmdHelp ( ) override
    {
        MyStringT<32> names[MO_MAX + 20];
        int           numOf = 0;

        for ( int i = 0; i < MO_MAX; i++ ) {
            if ( (i == MO_SFS) || (i == MO_SW) )
                continue;

            if ( !isVisibleMo((T_MoType)i) )
                continue;
            Mo* moP = SampleMo((T_MoType)i);
            if ( moP != nullptr ) {
                bool hasSet = false;
                for ( Mo::IteratorAttr it = moP->BeginAttr( ); it.IsValid( ); it++ ) {
                    if ( it->IsSettable( ) ) {
                        hasSet = true;
                        break;
                    }
                }

                if ( (moP->IsCreatable( ) || hasSet) )
                    names[numOf++] << moP->TypeName( );
            }
        }
        names[numOf++] << "var";

        ocli << "set\n";
        PrintSortedHelp(names, numOf);
    }
};

////////////////////////////////////////////////

// class CliCmdClear : public CliCmd
// {
// public:
//     CliCmdClear( ) : CliCmd(CMD_CLEAR, "clear") { }

//     DICTIONARY_1(sw_scheduled, 1, "scheduled");

//     void PrintCmdHelp ( ) override
//     {
//         MyStringT<32> names[MO_MAX + 20];
//         int           numOf = 0;

//         for ( int i = 0; i < MO_MAX; i++ ) {
//             if ( i == MO_SFS )
//                 continue;

//             if ( !isVisibleMo((T_MoType)i) )
//                 continue;
//             Mo* moP = SampleMo((T_MoType)i);
//             if ( (moP != nullptr) && (moP->IsCreatable( ) || moP->GetCountersLine( )) )
//                 names[numOf++] << moP->TypeName( );
//         }
//         names[numOf++] << "startup-configuration";
//         names[numOf++] << "log";
//         names[numOf++] << "user-activity-log";
//         names[numOf++] << "var";
//         names[numOf++] << "sw scheduled";

//         ocli << "clear\n";
//         PrintSortedHelp(names, numOf);
//     }

//     void Run (CliContext* contextP) override
//     {
//         ValueDictInternalT<MO_MAX + 5> valueMoType;
//         for ( int i = 0; i < MO_MAX; i++ ) {
//             if ( (!IsDebugMode( )) && (i == MO_PORT_DIAG) )
//                 continue;
//             if ( !isVisibleMo((T_MoType)i) )
//                 continue;
//             Mo* moP = SampleMo((T_MoType)i);
//             if ( (moP != nullptr) && (moP->IsCreatable( ) || moP->GetCountersLine( ) || (i == MO_SCRIPT)) )
//                 valueMoType._dictionary.Add(moP->Type( ), moP->TypeName( ));
//         }
//         valueMoType._dictionary.Add(MO_MAX + 1, "startup-configuration");
//         valueMoType._dictionary.Add(MO_MAX + 2, "log");
//         valueMoType._dictionary.Add(MO_MAX + 3, "user-activity-log");
//         valueMoType._dictionary.Add(MO_MAX + 4, "var");
//         valueMoType._dictionary.Add(MO_MAX + 5, "sw");

//         valueMoType.SetCli(contextP);
//         if ( contextP->CheckExit(true) )
//             return;

//         if ( valueMoType.GetUint32( ) < MO_MAX ) {
//             T_MoType moType = (T_MoType)valueMoType.GetUint32( );
//             cliP->_helper.HelpAssignMo(_cmdType, moType);

//             Mo* sampleP         = SampleMo(moType);
//             Mo::_cmdType        = CMD_CLEAR;
//             Mo::_numOfReadersId = 0;

//             sampleP->DoHandleClear(ACTION_CLI_CMD);
//         } else if ( valueMoType == "startup-configuration" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             ConfigDelete( );
//             ocli << "startup-configuration is removed\n";
//         } else if ( (valueMoType == "log") || (valueMoType == "user-activity-log") ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             MyStringT<128> path;
//             if ( valueMoType == "log" )
//                 path << EVENT_LOG_FILE_PATH;
//             else
//                 path << AUDIT_LOG_FILE_PATH;

//             truncate(path.AsStr( ), 0);

//             for ( int i = 1; i <= 20; i++ ) {
//                 MyStringT<32> name;
//                 name << path.AsStr( ) << "." << i;
//                 remove(name.AsStr( ));
//             }

//             ocli << "log is cleared\n";
//         } else if ( valueMoType == "var" ) {
//             ValueStringT<128> valueName;

//             *contextP >> valueName >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             RetStatus ret = cliVars.ClearGlobal(valueName.GetP( ));
//             if ( !ret.ok( ) )
//                 cliP->ReturnErrorAndPrint(ret);
//         } else if ( valueMoType == "sw" ) {
//             ValueDictT<sw_scheduled, 1> _value_sw_scheduled;

//             *contextP >> _value_sw_scheduled >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             unlink(SwRunHandler::ScheduledFileName( ));
//             ocli << "Done";
//         }
//     }
// };

////////////////////////////////////////////////

// #define PIC_NUM_FILES_SUPPORTED 4
// #define PIC_CRC_STR_LEN         (sizeof("0xffff"))
// #define PIC_HEX_FILE_NAME_LEN   256  // length of dirent.d_name
// #define PIC_APPS_PATH           "/etc/pic_apps/"
// #define PIC_UPDATER_PATH        "/home/sw/bin/pic_updater"

// // file format: host_app_bin_0x????.hex
// #define PIC_IMAGE_NAME_START "host_app_bin_"
// #define PIC_IMAGE_NAME_END   ".hex"

// class CliCmdPic : public CliCmd
// {
// public:
//     CliCmdPic( ) : CliCmd(CMD_PIC, "pic") { }

//     struct ValueCrcHex : public ValueDictInternalT<PIC_NUM_FILES_SUPPORTED> {
//         char  _allCrcName[PIC_NUM_FILES_SUPPORTED][PIC_CRC_STR_LEN];
//         char  _allCrcFullFileName[PIC_NUM_FILES_SUPPORTED][PIC_HEX_FILE_NAME_LEN];
//         UINT8 files_number = 0;

//         ValueCrcHex( )
//         {
//             DIR*                             picAppsDir = opendir(PIC_APPS_PATH);
//             MyStringT<PIC_HEX_FILE_NAME_LEN> fileNameNoCrc(PIC_IMAGE_NAME_START PIC_IMAGE_NAME_END);
//             int                              index = 0;
//             for ( dirent* hexFile = readdir(picAppsDir); hexFile != nullptr; hexFile = readdir(picAppsDir) ) {
//                 if ( index == PIC_NUM_FILES_SUPPORTED ) {
//                     fLOG_EX_ERR(false, "number of pic images found exceeded limit");
//                     ocli << "number of pic images found exceeded limit, showing only the first " << PIC_NUM_FILES_SUPPORTED << " alphabetically\n";
//                     break;
//                 }
//                 if ( hexFile->d_name[0] == '.' ) {
//                     continue;  // skip hidden files
//                 }
//                 MyStringT<PIC_HEX_FILE_NAME_LEN> fileName(hexFile->d_name);
//                 fileName.RemoveSubstr(sizeof(PIC_IMAGE_NAME_START) - 1, PIC_CRC_STR_LEN - 1);
//                 if ( fileName == fileNameNoCrc ) {
//                     sprintf(_allCrcFullFileName[index], hexFile->d_name);
//                     snprintf(_allCrcName[index], PIC_CRC_STR_LEN, hexFile->d_name + sizeof(PIC_IMAGE_NAME_START) - 1);
//                     _dictionary.Add(index, _allCrcName[index]);
//                 }
//                 index++;
//             }
//             files_number = index;
//         }

//         char* getFullName (int index)
//         {
//             assert(index < PIC_NUM_FILES_SUPPORTED);
//             return &_allCrcFullFileName[index][0];
//         }

//         char* getCrcName (int index)
//         {
//             assert(index < PIC_NUM_FILES_SUPPORTED);
//             return &_allCrcName[index][0];
//         }
//     };

//     void Run (CliContext* contextP) override
//     {
//         DICTIONARY_4(RunType, 1, "burn", 2, "validate", 3, "display", 4, "list-available-images") runType;
//         ValueDictExternal valueRunType(&runType);
//         *contextP >> valueRunType;
//         if ( contextP->CheckExit(true) )
//             return;

//         ValueCrcHex valueCrcHex;
//         if ( valueRunType == "list-available-images" ) {
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//             for ( int i = 0; i < valueCrcHex.files_number; i++ ) {
//                 ocli << valueCrcHex.getCrcName(i) << "\n";
//             }
//             ocli << "\n";
//             return;
//         }
//         MyStringT<128> linuxPicCmd(PIC_UPDATER_PATH);
//         if ( valueRunType == "display" ) {
//             linuxPicCmd << " -i";
//             *contextP >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;
//         } else {
//             contextP->_helper.HelpAssignValue(&valueCrcHex);
//             *contextP >> valueCrcHex >> END( );
//             if ( contextP->CheckExit(true) )
//                 return;

//             if ( valueRunType == "validate" ) {
//                 *contextP >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;
//                 linuxPicCmd << " -v " << PIC_APPS_PATH << valueCrcHex.getFullName(valueCrcHex.GetUint32( ));
//             } else if ( valueRunType == "burn" ) {
//                 *contextP >> END( );
//                 if ( contextP->CheckExit(true) )
//                     return;
//                 ocli << "Starting burning process.\nDo not turn off the device until finished!!!\n";
//                 linuxPicCmd << " -b " << PIC_APPS_PATH << valueCrcHex.getFullName(valueCrcHex.GetUint32( ));
//             } else {
//                 return;
//             }
//         }
//         pid_t       picPid = StartLinuxCliProcess(linuxPicCmd.AsStr( ));
//         const char* errorP = Client(GetClientId( ))->_repeatStack.push<LinuxCliProcessRepeater>(picPid);
//         if ( errorP != nullptr ) {
//             cliP->SetStatus(RetStatus {"Cannot run pic updater process"});
//             cliP->CheckExit(true);
//         }
//     }

//     void PrintCmdHelp ( ) override
//     {
//         ocli << "pic display - display PIC FW version and CRC value\n";
//         ocli << "pic list-available-images - list CRCs of available images\n";
//         ocli << "pic validate <CRC> - validate image with the selected CRC\n";
//         ocli << "pic burn <CRC> - burns image with the selected CRC\n";
//     }
// };

//////////////////////////////////////////////////////////

bool IsBistMode ( )
{
//#ifdef X86_TARGET
    return false;
//#endif  // #ifdef X86_TARGET

    // bool        rc;
    // E_BIST_MODE mode = Sys( )->GetBistMode( );

    // rc = mode != E_BIST_MODE_NONE;

    // return rc;
}

// bool BistExec (char* cmdP, void* contextP, UINT32 maxContextLen)
// {
//     (void)maxContextLen;
//     bool ret_val = true;

//     struct BistContext {
//         UINT32 counter;
//     };

//     BistContext* dataP = (BistContext*)contextP;

//     if ( cmdP != nullptr ) {
//         dataP->counter = 1;
//         // cli_print( "BIST_CALL: cmd=<%s>, counter=<%d>", cmdP, dataP->counter);
//         const int max_num_tokens = 20;  // TBD
//         char      tokens[max_num_tokens][TOKEN_MAX_SIZE];
//         int       count = 0;

//         memset(tokens, 0, sizeof(tokens));
//         // at least 1 token with word "bist" presents
//         char* token = strtok(cmdP, " ");
//         while ( token && (count < max_num_tokens) ) {
//             strncpy(tokens[count], token, TOKEN_MAX_SIZE - 1);
//             // cli_print("token: %s %s\n", token, tokens[count]);
//             count++;
//             token = strtok(nullptr, " ");
//         }

//         SYSTEM_BIST_TEST_E rc = SystemBistExec(count - 1, &tokens[1]);  // skip over first word/token "bist"
//         switch ( rc ) {
//             case SYSTEM_BIST_TEST_COMPLETED_OK:
//             case SYSTEM_BIST_TEST_COMPLETED_ERR: ret_val = true; break;
//             case SYSTEM_BIST_TEST_INPROGRESS: ret_val = false; break;
//         }
//     } else {
//         dataP->counter++;
//         cli_print("BIST_CALL: counter=<%d>", dataP->counter);
//     }

//     // example: return (dataP->counter > 5) ? true : false;
//     return ret_val;
// }

// struct BistRepeater : public CadRepeater {
//     BistRepeater(char* cmdP) : CadRepeater(SLEEP)
//     {
//         _originalCmd.Clean( );
//         _originalCmd << cmdP;
//     }

//     void CallMe ( ) override
//     {
//         bool isEnd = BistExec(_isFirst ? _originalCmd.AsStr( ) : nullptr, _context, sizeof(_context));
//         _isFirst   = false;

//         printWhenSleep = true;

//         if ( isEnd )
//             EndMe( );
//     }

//     MyStringT<256> _originalCmd;
//     bool           _isFirst {true};
//     char           _context[256];
// };

// class CliCmdBist : public CliCmd
// {
// public:
//     CliCmdBist( ) : CliCmd(CMD_BIST, "bist") { }

//     void Run (CliContext* contextP) override
//     {
//         const char* errorP = Client(GetClientId( ))->_repeatStack.push<BistRepeater>(contextP->_originalLine.AsStr( ));
//         if ( errorP != nullptr ) {
//             cliP->SetStatus(RetStatus {"Cannot run process"});
//             cliP->CheckExit(true);
//         }
//     }

//     void PrintCmdHelp ( ) override { ocli << "bist <parameters>\n"; }
// };

//////////////////////////////////////////////////////////

PlaceGlobal placeGlobal;

CliCmd* ValueCmd::Cmd(UINT32 index)
{
    static UINT32  numOf = 0;
    static CliCmd* cmdTable[100];
    if ( numOf == 0 ) {
        cmdTable[numOf++] = new (placeGlobal) CliCmdHelp( );
        cmdTable[numOf++] = new (placeGlobal) CliCmdShow( );
        cmdTable[numOf++] = new (placeGlobal) CliCmdSet( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdClear( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdReset( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdCopy( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdDebug( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdDebugFlag( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdNpuDebug( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdSpectrum( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdPic( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdNpu( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdCfm( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdRun( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdAccept( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdDir( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdDebugAction( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdPing( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdPing6( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdTracert( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdIperf( );
        // cmdTable[numOf++] = new (placeGlobal) CliCmdTracert6( );

        // if ( IsBistMode( ) )
        //     cmdTable[numOf++] = new (placeGlobal) CliCmdBist( );
    }

    return (index < numOf) ? cmdTable[index] : nullptr;
}

void PrintErrorToFile (const CliContext& context)
{
    // if ( context.IsError( ) && !GetErrorFilePath( ).empty( ) ) {
    //     if ( std::fstream file(GetErrorFilePath( ), std::fstream::app); file.is_open( ) ) {
    //         file << "\n" << context._originalLine.view( ) << "\n" << ocli.AsStr( ) << "\n";
    //         if ( !file.good( ) )
    //             fLOG_EX_ERR(false, "fail to write to {}", GetErrorFilePath( ));
    //         file.close( );
    //         ::sync( );
    //     }
    // }
}

void AdapterCli (CliContext& context)
{
    cliP = &context;

    LOG("processing cli command %s\n", context._originalLine.AsStr());
    // if ( special_character_exist(context._originalLine.view( ), E_ProhibitedCharactersSet::Cli) ) {
    //     cliP->ReturnErrorAndPrint(RetStatus {"Input contains not allowed symbols. Operation failed."});
    //     PrintErrorToFile(context);

    //     return;
    // }

    cliP->_helper.HelpAssignRoot( );

    // if ( (GetDebugFlag(DEBUG_FLAG_CONFIGURING) || GetDebugFlag(DEBUG_FLAG_CONFIGURING_DETAILS)) && context.IsExeMode( ) )
    //    ocli << "\n***************** START DUMP ******************\n";

    // detect and substitute cli variables
    if ( context.IsExeMode( ) ) {
        bool hasCliVar = false;

        while ( true ) {
            char* startVarP = strchr(context._originalLine.AsStr( ), '$');
            if ( startVarP == nullptr )
                break;
            char*          endVarP = strchrnul(startVarP, ' ');
            MyStringT<128> name;
            name.Print(endVarP - startVarP, startVarP);

            hasCliVar = true;

            char* cliVarValue = cliVars.Find(name.AsStr( ) + 1);  // do not use starting $

            if ( cliVarValue == nullptr ) {
                LOG("Cannot find '%s'. Operation failed.\n", name.AsStr());
                // cliP->ReturnErrorAndPrint(RetStatus {"Cannot find {}. Operation failed.", name});
                PrintErrorToFile(context);
                return;
            }

            context._originalLine.RemoveSubstr(startVarP - context._originalLine.AsStr( ), name.GetLen( ));
            context._originalLine.InsertSubstr(startVarP - context._originalLine.AsStr( ), cliVarValue);
        }

        if ( hasCliVar )
            ocli << "Substituted: " << context._originalLine << "\n";

        if ( !context._ignorePermissions ) {
            std::string first;
            std::string second;

            WordReaderString reader(context._originalLine.AsStr( ));

            if ( reader.ReadWord( ).ok( ) ) {
                first = reader._dst.AsStr( );

                if ( reader.ReadWord( ).ok( ) )
                    second = reader._dst.AsStr( );
            }

            // SkString::strlower(first);
            // SkString::strlower(second);
            // fTRACE(false, "check permission for {} {} {}", Client(GetClientId( ))->_userInfo._userName, first, second);
            // if ( !Client(GetClientId( ))->_userInfo.CheckPermission(first, second) ) {
            //     cliP->ReturnErrorAndPrint(RetStatus {"No permission for current user"});
            //     return;
            // }
        }
    }

    // read command
    ValueCmd valueCmd;
    valueCmd.SetCli(&context);
    if ( context.CheckExit(true) ) {
        PrintErrorToFile(context);
        return;
    }

    // build per-command help (singletone)
    cliP->_helper.SetAssignCmd(valueCmd.Cmd( ));

    bool wasConfiguring = false;

    // perform the command
    valueCmd.Cmd( )->Run(&context);

    // terminate transaction: normal or restoration
    switch ( valueCmd.GetUint32( ) ) {
        case CMD_SET:
        case CMD_CLEAR:
        case CMD_RESET:
        case CMD_COPY:
        case CMD_RUN:
        case CMD_ACCEPT:
        case CMD_DIR:
        case CMD_SPECTRUM:
        case CMD_BIST: wasConfiguring = true; break;
        default: break;
    }

    // if ( wasConfiguring && context.IsExeMode( ) && !context.IsError( ) && !disableLicenseConfigMismatchCheck ) {
    //     RetStatus result = LicenseController::Self( )->CheckAllConfigMismatch( );
    //     if ( !result.ok( ) )
    //         cliP->ReturnErrorAndPrint(result);
    // }

    if ( context.IsExeMode( ) && !context.IsError( ) ) {
        // logging
        if ( (valueCmd == CMD_SHOW) || (valueCmd == CMD_DIR) || (valueCmd == CMD_PING) || (valueCmd == CMD_TRACERT) || (valueCmd == CMD_PING6) || (valueCmd == CMD_TRACERT6) || (valueCmd == CMD_IPERF)
             || (valueCmd == CMD_BIST) )
            ;  // do not log
        else if ( valueCmd == CMD_SET ) {
            MyStringT<MAX_CLI_INPUT_LEN> str;
            str.Print(context._originalLine.AsStr( ));

            WordReaderString reader(str.AsStr( ));
            reader.Read( );  // read first word - command
            reader.Read( );  // read second word MO type

            LOG("reader._dst=%s", reader._dst);
            // if ( reader._dst == SampleMo(MO_SNMP_USER)->TypeName( ) )
            //     ;  // do not log
            // else if ( reader._dst == SampleMo(MO_USER)->TypeName( ) ) {
            //     reader.Read( );                   // read user name
            //     while ( reader.Read( ).IsOk( ) )  // read attr name
            //     {
            //         if ( reader._dst == "passw" ) {
            //             while ( reader.Cur( ) == ' ' )
            //                 reader.NextChar( );  // skip whitespaces
            //             int passwPos = reader.GetPos( );
            //             str.RemoveWord(passwPos);
            //             str.InsertSubstr(passwPos, "***");
            //         }
            //         reader.Read( );  // read attr value
            //     }
            //     LogUserActivity(str.AsStr( ));
            // } else
            //     LogUserActivity(context._originalLine.AsStr( ));
        } else if ( valueCmd == CMD_SPECTRUM ) {
            MyStringT<MAX_CLI_INPUT_LEN> str;
            str.Print(context._originalLine.AsStr( ));

            WordReaderString reader(str.AsStr( ));
            reader.Read( );  // read first word - spectrum-analyzer
            reader.Read( );  // read second word - start/abort/results

            if ( reader.IsOk( ) && reader._dst == "results" )
                ;  // do not log
            else
                LogUserActivity(context._originalLine.AsStr( ));

        } else if ( valueCmd == CMD_PIC ) {
            MyStringT<MAX_CLI_INPUT_LEN> str;
            str.Print(context._originalLine.AsStr( ));

            WordReaderString reader(str.AsStr( ));
            reader.Read( );  // read first word - pic
            reader.Read( );  // read second word - display/burn/...

            if ( reader.IsOk( ) && reader._dst == "list-available-images" )
                ;  // do not log
            else if ( reader.IsOk( ) && reader._dst == "display" )
                ;  // do not log
            else if ( reader.IsOk( ) && reader._dst == "validate" )
                ;  // do not log
            else
                LogUserActivity(context._originalLine.AsStr( ));

        } else
            LogUserActivity(context._originalLine.AsStr( ));
    }

    if ( !wasConfiguring )
        TransactionEnd(TRANSACTION_END_NORMAL);
    else if ( context.IsExeMode( ) && (!context.IsError( )) ) {
        TransactionEnd(TRANSACTION_END_CONFIG);

        // if ( (GetDebugFlag(DEBUG_FLAG_CONFIGURING) || GetDebugFlag(DEBUG_FLAG_CONFIGURING_DETAILS)) && context.IsExeMode( ) )
            ocli << "\n***************** END DUMP ********************\n";
    } else
        TransactionEndRestore( );

    PrintErrorToFile(context);
}

// void AdapterHttp (CliContext* contextP)
// {
//     cliP = contextP;
//     char fw_rev_rf_70_card[20], fw_rev_rf_80_card[20];

//     //  MoFdb::ReloadDynamicFdbObjects();
//     //  MoArp::ReloadDynamicObjects();

//     Xml xml(&ocli);

//     WordReaderString reader(contextP->_originalLine.AsStr( ));  // query
//     if ( !reader.ReadWord( ).ok( ) ) {
//         ocli << "<error>"
//              << "missing command"
//              << "</error>\n";
//         return;
//     }

//     if ( reader._dst == "help" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         ocli << "<command>"
//              << "system-defs"
//              << "</command>\n";
//         ocli << "<command>"
//              << "mo-info"
//              << "</command>\n";
//         ocli << "<command>"
//              << "mo-stats-history"
//              << "</command>\n";
//         ocli << "<command>"
//              << "mo-stats-days"
//              << "</command>\n";
//         ocli << "<command>"
//              << "show-log"
//              << "</command>\n";
//         ocli << "<command>"
//              << "show-user-activity-log"
//              << "</command>\n";
//         ocli << "<command>"
//              << "simple-command"
//              << "</command>\n";
//         ocli << "<command>"
//              << "start-ping"
//              << "</command>\n";
//         ocli << "<command>"
//              << "start-tracert"
//              << "</command>\n";
//         ocli << "<command>"
//              << "linux-process-status"
//              << "</command>\n";
//         ocli << "<command>"
//              << "stop-linux-process"
//              << "</command>\n";
//         ocli << "<command>"
//              << "process-status"
//              << "</command>\n";
//         ocli << "<command>"
//              << "snmp-user-process-status"
//              << "</command>\n";
//         ocli << "<command>"
//              << "start-process  [copy-sw | copy-license | copy-rem2loc | sw-accept | sw-run | script-run]"
//              << "</command>\n";
//         ocli << "<command>"
//              << "script-process-status"
//              << "</command>\n";
//         ocli << "<command>"
//              << "snmp-user"
//              << "</command>\n";
//         ocli << "<command>"
//              << "start-iperf"
//              << "</command>\n";
//     } else if ( reader._dst == "snmp-user" ) {
//         MyStringT<256> cmdBody;
//         if ( !reader.IsEol( ) )
//             cmdBody << (reader._srcP + reader.GetPos( ));

//         RetStatus retStatus = SnmpUserRepeater::StartHttpAction(cmdBody.AsStr( ));

//         if ( !retStatus.ok( ) )
//             xml.TagEndCode("error").TagText(retStatus.message( ).c_str( ));
//         else
//             xml.TagEndCode("ok");
//         return;
//     } else if ( reader._dst == "system-defs" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         ocli << "<global-identity>" << Sys( )->GetGlobalIdentityStr( ) << "</global-identity>";
//         ocli << "<is-fdd>" << 1 << "</is-fdd>";
//         ocli << "<pse-supported>" << (Pse( )->IsSupported( ) ? 1 : 0) << "</pse-supported>";
//         ocli << "<pse-has-status>" << (Pse( )->HasStatus( ) ? 1 : 0) << "</pse-has-status>";
//         ocli << "<pse-has-two-ports>" << (MoPse::HasTwoOutPorts( ) ? 1 : 0) << "</pse-has-two-ports>";
//         ocli << "<country-supported>" << 1 << "</country-supported>";
//         ocli << "<synce-1588-supported>" << (Sys( )->IsSynceIeee1588Supported( ) ? 1 : 0) << "</synce-1588-supported>";
//         ocli << "<encryption-supported>" << (Sys( )->IsEncryptRfSupport( ) ? 1 : 0) << "</encryption-supported>";
//         ocli << "<spectrum-supported>" << (MoRf::HasSpectrum( ) ? 1 : 0) << "</spectrum-supported>";
//         ocli << "<atpc-supported>" << (MoRf::HasAtpc( ) ? 1 : 0) << "</atpc-supported>";
//         ocli << "<in-queue-statistics-supported>0</in-queue-statistics-supported>";
//         ocli << "<qos-supported>" << 1 << "</qos-supported>";
//         NxpModem_Get_Fw_Revision(fw_rev_rf_70_card, fw_rev_rf_80_card);
//         ocli << "<rf-boards>"
//              << "<rf-70>" << fw_rev_rf_70_card << "</rf-70>"
//              << "<rf-80>" << fw_rev_rf_80_card << "</rf-80>"
//              << "</rf-boards>";

//         ocli << "<channel-width-list>";
//         DictionaryT<10> dictChannelWidth = GetChannelWidthDictionary( );
//         UINT32          context;
//         for ( dictChannelWidth.___FirstEntry(&context); dictChannelWidth.___GetEntry(context).IsValid( ); dictChannelWidth.___NextEntry(&context) )
//             ocli << "<channel-width>" << dictChannelWidth.___GetEntry(context)._nameP << "</channel-width>";
//         ocli << "</channel-width-list>";

//         ocli << "<frequency-list>";
//         for ( UINT32 i = 0; FreqEntry::GetEntry(i) != nullptr; i++ ) {
//             ocli << "<frequency-entry>";
//             ocli << "<channel-width>" << FreqEntry::GetEntry(i)->WidthAsName( ) << "</channel-width>";
//             ocli << "<frequency>" << FreqEntry::GetEntry(i)->FreqAsName( ) << "</frequency>";

//             forEachAvailableCountryCode([i] (EnumCountry country) {
//                 if ( FreqEntry::GetEntry(i)->HasCountry(country) )
//                     ocli << "<country>" << CountryName(country) << "</country>";
//             });
//             ocli << "</frequency-entry>";
//         }
//         ocli << "</frequency-list>";
//     } else if ( (reader._dst == "start-ping") || (reader._dst == "start-ping6") ) {
//         bool isSix = (reader._dst == "start-ping6");

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  host");
//             return;
//         }
//         MyStringT<128> host;
//         host << reader._dst;

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  num pings");
//             return;
//         }
//         MyStringT<128> numPings;
//         numPings << reader._dst;

//         RetStatus retStatus = StartHttpPing(isSix, host, numPings);
//         if ( !retStatus.ok( ) )
//             xml.TagEndCode("error").TagText(retStatus.message( ).c_str( ));
//         else
//             xml.TagEndCode("ok");
//     } else if ( reader._dst == "start-iperf" ) {
//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  mode");
//             return;
//         }

//         if ( (reader._dst != "-c") && (reader._dst != "-s") ) {
//             xml.TagEndCode("error").TagText("incorrect mode option");
//             return;
//         }

//         bool isClient = (reader._dst == "-c");

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  host");
//             return;
//         }
//         MyStringT<128> host;
//         host << reader._dst;

//         if ( special_character_exist(host.AsStr( ), E_ProhibitedCharactersSet::Url) ) {
//             xml.TagEndCode("error").TagText("prohibit characters in host parameter");
//             return;
//         }

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  time to transmit");
//             return;
//         }
//         MyStringT<128> timeToTransmit;
//         timeToTransmit << reader._dst;

//         if ( special_character_exist(timeToTransmit.AsStr( ), E_ProhibitedCharactersSet::Url) ) {
//             xml.TagEndCode("error").TagText("prohibit characters in time parameter");
//             return;
//         }

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  protocol");
//             return;
//         }
//         bool isUdp = reader._dst == "udp";

//         if ( reader._dst != "udp" && reader._dst != "tcp" ) {
//             xml.TagEndCode("error").TagText("incorrect protocol");
//             return;
//         }

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing  port");
//             return;
//         }
//         MyStringT<128> port;
//         port << reader._dst;

//         RetStatus retStatus = StartHttpIperf(isClient, isUdp, host, timeToTransmit, port);
//         if ( !retStatus.ok( ) )
//             xml.TagEndCode("error").TagText(retStatus.message( ).c_str( ));
//         else
//             xml.TagEndCode("ok");
//     } else if ( reader._dst == "start-tracert" || reader._dst == "start-tracert6" ) {
//         bool isSix = (reader._dst == "start-tracert6");

//         if ( !reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("missing host");
//             return;
//         }
//         MyStringT<128> host;
//         host << reader._dst;

//         RetStatus retStatus = StartHttpTracert(isSix, host);
//         if ( !retStatus.ok( ) )
//             xml.TagEndCode("error").TagText(retStatus.message( ).c_str( ));
//         else
//             xml.TagEndCode("ok");
//     } else if ( reader._dst == "linux-process-status" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         BuildHttpLinuxProcessStatus(&xml);
//     } else if ( reader._dst == "script-process-status" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         BuildScriptProcessStatus(&xml);
//     } else if ( reader._dst == "stop-linux-process" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         StopLinuxProcess( );
//         xml.TagEndCode("ok");
//     } else if ( reader._dst == "process-status" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         if ( FtpControl::Self( )->_status.IsContinue( ) )
//             xml.TagEndCode("continue");
//         else if ( FtpControl::Self( )->_status.IsOk( ) )
//             xml.TagEndCode("ok");
//         else
//             xml.TagEndCode("error");
//         xml.TagText(FtpControl::Self( )->_status.TextP( ));
//     } else if ( reader._dst == "snmp-user-process-status" ) {
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }

//         if ( SnmpUserRepeater::_httpStatus.IsContinue( ) )
//             xml.TagEndCode("continue");
//         else if ( SnmpUserRepeater::_httpStatus.IsOk( ) )
//             xml.TagEndCode("ok");
//         else
//             xml.TagEndCode("error");
//         xml.TagText(SnmpUserRepeater::_httpStatus.TextP( ));
//     } else if ( reader._dst == "start-process" ) {
//         if ( !reader.ReadWord( ).ok( ) ) {
//             ocli << "<error>"
//                  << "missing start-process sub-command"
//                  << "</error>\n";
//             return;
//         }

//         ValueDictInternalT<20> valueProcessType;
//         valueProcessType._dictionary.Add(CMD_COPY_SW, "copy-sw");
//         valueProcessType._dictionary.Add(CMD_COPY_LICENSE, "copy-license");
//         valueProcessType._dictionary.Add(CMD_COPY_REM2LOC, "copy-rem2loc");
//         valueProcessType._dictionary.Add(CMD_SW_ACCEPT, "sw-accept");
//         valueProcessType._dictionary.Add(CMD_SW_RUN, "sw-run");
//         valueProcessType._dictionary.Add(CMD_SCRIPT_RUN, "script-run");

//         if ( !valueProcessType.SetString(reader._dst.AsStr( )).ok( ) ) {
//             ocli << "<error>"
//                  << "invalid start-process sub-command"
//                  << "</error>\n";
//             return;
//         }

//         MyStringT<128> remote;
//         /* In case of the following copy commands we give a dummy file with prefix http:// of remote URL. Because there is a parsing of the
//            remote URL in the FTP package/software which is used also for other common software download methods which need the remote URL (like: ftp:// tftp:// ....).
//            So to be compatible with other software download methods we use/give this dummy remote URL for http. */
//         if ( valueProcessType == CMD_COPY_SW || valueProcessType == CMD_COPY_REM2LOC || valueProcessType == CMD_COPY_LICENSE )
//             remote << "http://dummy_file.txt";

//         MyStringT<128> local;
//         if ( !reader.IsEol( ) )
//             local << (reader._srcP + reader.GetPos( ));

//         /* In case of copy command rem2loc check that local parameter is legal */
//         if ( valueProcessType == CMD_COPY_REM2LOC ) {
//             const char* script_dir              = "flash:scripts/";
//             const char* startup_config_dir      = "flash:startup-config/";
//             int         script_path_len         = strlen(script_dir);
//             int         startup_config_path_len = strlen(startup_config_dir);

//             if ( strncmp(local.AsStr( ), script_dir, script_path_len) == 0 ) {
//                 if ( special_character_exist(local.AsStr( ) + script_path_len, E_ProhibitedCharactersSet::Url) ) {
//                     xml.TagEndCode("error").TagText("prohibit characters in request");
//                     return;
//                 }
//             } else if ( strncmp(local.AsStr( ), startup_config_dir, startup_config_path_len) == 0 ) {
//                 if ( special_character_exist(local.AsStr( ) + startup_config_path_len, E_ProhibitedCharactersSet::Url) ) {
//                     xml.TagEndCode("error").TagText("prohibit characters in request");
//                     return;
//                 }
//             } else {
//                 xml.TagEndCode("error").TagText("Invalid local path");
//                 return;
//             }
//         }

//         if ( valueProcessType == CMD_SW_RUN ) {
//             if ( std::string declainReasoon = Sys( )->checkVersionAllowed(MoSw::getSwVersionsData( ).passiveSwVer( )); !declainReasoon.empty( ) ) {
//                 xml.TagEndCode("error").TagText(declainReasoon.c_str( ));
//                 return;
//             }
//         }

//         if ( valueProcessType == CMD_SCRIPT_RUN ) {
//             if ( special_character_exist(local.AsStr( ), E_ProhibitedCharactersSet::Url) || (local.GetLen( ) > 32) ) {
//                 xml.TagEndCode("error").TagText("prohibit characters in request");
//                 return;
//             }

//             MyStringT<128> file_path;
//             file_path << SCRIPTS_DIR << local.AsStr( );

//             if ( (access(file_path.AsStr( ), F_OK) != 0) || (local.GetLen( ) == 0) ) {
//                 xml.TagEndCode("error").TagText("script file does not exist");
//             }

//             FtpControl::Self( )->StartActionRunScript(FtpControl::SOURCE_HTTP, (local.GetLen( ) != 0) ? local.AsStr( ) : nullptr);
//         } else
//             FtpControl::Self( )->StartAction(FtpControl::SOURCE_HTTP, valueProcessType.GetEnum<FtpCmd>( ), remote, local);

//         if ( FtpControl::Self( )->_status.IsContinue( ) )
//             xml.TagEndCode("continue");
//         else if ( FtpControl::Self( )->_status.IsOk( ) )
//             xml.TagEndCode("ok");
//         else
//             xml.TagEndCode("error");
//         xml.TagText(FtpControl::Self( )->_status.TextP( ));
//     } else if ( reader._dst == "mo-stats-days" ) {
//         if ( !reader.ReadWord( ).ok( ) ) {
//             ocli << "<error>"
//                  << "missing mo-type"
//                  << "</error>\n";
//             return;
//         }
//         if ( reader._dst != "rf" ) {
//             ocli << "<error>"
//                  << "only rf accepted"
//                  << "</error>\n";
//             return;
//         }

//         if ( Mo::FindMo(MO_RF) )
//             ((MoRf*)Mo::FindMo(MO_RF))->ShowXmlDaysHistory( );
//     } else if ( reader._dst == "show-log" || reader._dst == "show-user-activity-log" ) {
//         std::string cmd {reader._dst.view( )};
//         if ( reader.ReadWord( ).ok( ) ) {
//             xml.TagEndCode("error").TagText("input parameter is not supported");
//             return;
//         }
//         xml.TagEndCode("ok");
//         ocli << "<text>";
//         for ( int i = 20; i >= 0; i-- ) {
//             MyStringT<32> name;

//             if ( cmd == "show-log" )
//                 name << EVENT_LOG_FILE_PATH;
//             else
//                 name << AUDIT_LOG_FILE_PATH;

//             if ( i != 0 )
//                 name << "." << i;

//             FileWrapper fp(name.AsStr( ), "r");
//             if ( fp.is_open( ) ) {
//                 char tmp[1000];
//                 while ( fgets(tmp, sizeof(tmp) - 1, fp) != nullptr ) {
//                     char encoded[2000] {0};
//                     Xml::EncodeText(encoded, sizeof(encoded) - 1, tmp);
//                     ocli << encoded;
//                 }
//             }
//         }
//         ocli << "</text>";
//     } else if ( reader._dst == "simple-command" ) {
//         if ( reader.IsEol( ) ) {
//             ocli << "<error>"
//                  << "missing command body"
//                  << "</error>\n";
//             return;
//         }

//         MyStringT<CLI_REPLY_PRINT_BUFFER + 1> replyBuffer;

//         const size_t command_buff_size              = 2000;
//         char         command_buf[command_buff_size] = {0};
//         std::string  decodedCmd;

//         if ( command_buff_size >= (strlen(reader._srcP + reader.GetPos( )) + 1) ) {
//             strcpy(command_buf, reader._srcP + reader.GetPos( ));
//         } else {
//             xml.TagEndCode("error").TagText("encoded string too big");
//             return;
//         }

//         /* decode the base64 data. Remark for knowledge: The encoded string length is always bigger then the decoded string length */
//         decodedCmd = base64_decode(command_buf);
//         if ( decodedCmd.empty( ) ) {
//             xml.TagEndCode("error").TagText("string command decode problem");
//             return;
//         }
//         fTRACE(false, "decoded cmd: {}", decodedCmd);
//         auto tokens = SkString::removeEmptyTokens(SkString::tokenizeSimplified(decodedCmd, ";"));
//         for ( const std::string_view token: tokens ) {
//             CliContextExe context(SkString::trimWhitespaces(token, " \t\n\r"));
//             AdapterCli(context);
//             Xml::TagEndCode(&replyBuffer, context.Status( ).ok( ) ? "ok" : "error");
//             Xml::TagText(&replyBuffer, ocli.AsStr( ));
//         }

//         ocli.Clean( );
//         ocli << replyBuffer;
//     } else if ( reader._dst == "mo-info" || reader._dst == "mo-stats-history" ) {
//         bool isFrequency = false;

//         if ( reader._dst == "mo-info" )
//             xml.SetShowActionType(XML_SHOW_INFO);
//         else if ( reader._dst == "mo-stats-history" )
//             xml.SetShowActionType(XML_SHOW_STATS_HISTORY);

//         for ( ;; ) {
//             T_MoType       moType = MO_UNKNOWN;
//             MyStringT<128> moName;

//             reader.ReadWord(" ;");
//             if ( reader._dst == "all" ) {
//                 if ( !reader.IsEol( ) ) {
//                     ocli << "<error>"
//                          << "all cannot have more parameters"
//                          << "</error>\n";
//                     return;
//                 }
//             } else if ( reader._dst != ";" ) {
//                 ValueDictInternalT<MO_MAX> valueMoType;
//                 AddMoTypes(&valueMoType._dictionary);

//                 isFrequency = reader._dst == "frequency";

//                 if ( !isFrequency ) /* Frequency is not MO type */
//                 {
//                     const std::string moTypeStr = reader._dst.AsStr( );
//                     if ( !valueMoType.SetString(moTypeStr).ok( ) ) {
//                         ocli << "<mo>"
//                              << "Invalid mo type "
//                              << "</mo>\n";
//                         fLOG_EX_ERR(false, "We received invalid mo type {} from http request", moTypeStr);
//                     }
//                 }

//                 moType = valueMoType.GetEnum<T_MoType>( );

//                 while ( reader.ReadWord(" ;").ok( ) ) {
//                     if ( reader._dst == ";" )
//                         break;
//                     if ( moName.GetLen( ) == 0 )
//                         moName << valueMoType;
//                     moName << " " << reader._dst;
//                 }
//             }

//             /* If it's frequency http request then we build the HTML commands to be replied */
//             if ( isFrequency ) {
//                 DoFrequencyXmlShow(&xml, &moName);
//             } else {
//                 for ( int i = 0; i < MO_MAX; i++ ) {
//                     if ( (moType != MO_UNKNOWN) && (moType != i) )
//                         continue;

//                     if ( moType == MO_ARP )
//                         MoArp::ReloadDynamicObjects( );

//                     if ( moType == MO_FDB_ENTRY ) {
//                         MoFdbEntry::ReloadDynamicFdbObjects( );
//                     }

//                     SampleMo(moType)->DoSampleXmlShow(&xml, &moName);
//                 }
//             }

//             // here ';' or end
//             if ( reader._dst != ";" )
//                 break;
//         }
//     } else {
//         ocli << "<error>"
//              << "unknown command"
//              << "</error>\n";
//     }
// }
