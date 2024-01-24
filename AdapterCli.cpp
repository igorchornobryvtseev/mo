#include "igorc_common.h"
#include "MyString.h"
#include "RetStatus.h"
#include "CliContext.h"
#include <string>
#include <sstream>

bool     disableLicenseConfigMismatchCheck = false;
uint32_t requestClientId;


uint32_t GetClientId ( )
{
    return requestClientId;
}

void LogUserActivity (std::string_view text)
{
    LOG("'%s'", text.data());
}

CliContext* cliP = nullptr;

// Global: for CLI>set var ...
// Local:  for RunScriptRepeater
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
    // RetStatus ClearGlobal (const char* nameP)
    // void AddLocals (const char* startP)
    // void ClearLocals ( ) { _numOfLocals = 0; }

    char* Find (const char* nameP) { /*igorc stub*/ return nullptr; }

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
// void ClearLocalCliVars ( )

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
            for ( UINT32 i = 0; Cmd(i) != nullptr; i++ ) {
                LOG("add [%u] '%s'", i, Cmd(i)->_cmdName);
                myDictionary.Add(Cmd(i)->_cmdType, Cmd(i)->_cmdName);
            }
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

////////////////////////////////////////////////////////////

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
        const char* s = moP->TypeName( );
        LOG("%s", s);
        dstP->Add(moP->Type( ), s);
    }
}


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

////////////////////////////////////////////////////////////

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
        //valueMoType._dictionary.Add(MO_MAX + 1, "var");

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
        // else if ( valueMoType == "var" ) {...}
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

//////////////////////////////////////////////////////////

bool IsBistMode ( )
{
    return false;
}

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
    std::stringstream ss;
    ss << "\n" << context._originalLine.view( ) << "\n" << ocli.AsStr( ) << "\n";
    LOG("context.IsError:%d '%s'", context.IsError( ), ss.str());
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

        // CheckPermission - example of reader
        // // if ( !context._ignorePermissions ) {
        // //     std::string first;
        // //     std::string second;

        // //     WordReaderString reader(context._originalLine.AsStr( ));

        // //     if ( reader.ReadWord( ).ok( ) ) {
        // //         first = reader._dst.AsStr( );

        // //         if ( reader.ReadWord( ).ok( ) )
        // //             second = reader._dst.AsStr( );
        // //     }

        // // }
    }

    // read command
    ValueCmd valueCmd;
    valueCmd.SetCli(&context);
    // igorc: below can be commented out
    if ( context.CheckExit(true) ) {
        PrintErrorToFile(context);
        return;
    }

    // build per-command help (singletone)
    cliP->_helper.SetAssignCmd(valueCmd.Cmd( ));

    bool wasConfiguring = false;

    // perform the command
    valueCmd.Cmd( )->Run(&context);
    //return; // igorc - use if fail?

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

            LOG("logging for CMD_SET: reader._dst.AsStr='%s' from str '%s'", reader._dst.AsStr(), context._originalLine.AsStr( ));
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
        } 
        else
            LogUserActivity(context._originalLine.AsStr( ));
    }

    if ( !wasConfiguring )
        TransactionEnd(TRANSACTION_END_NORMAL);
    else if ( context.IsExeMode( ) && (!context.IsError( )) ) {
        TransactionEnd(TRANSACTION_END_CONFIG);
        // if ( (GetDebugFlag(DEBUG_FLAG_CONFIGURING) || GetDebugFlag(DEBUG_FLAG_CONFIGURING_DETAILS)) && context.IsExeMode( ) )
        //    ocli << "\n***************** END DUMP ********************\n";
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
