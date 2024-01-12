#include "igorc_common.h"

// #include "CliContext.h"

// #include "AutoComplete.h"
// #include "CaStream.h"
// #include "CliMo.h"

//////////////////////////////////////////////////////////

bool WordReader::TestExisting(const char* sampleP)
{
    UINT32 keepPos = GetPos( );
    while ( ReadWord( ).ok( ) ) {
        if ( _dst == sampleP ) {
            SetPos(keepPos);
            return true;
        }
    }
    SetPos(keepPos);
    return false;
}

bool WordReader::TestNext(const char* sampleP)
{
    UINT32 keepPos = GetPos( );
    bool   result  = ReadWord( ).ok( ) ? (_dst == sampleP) : false;
    SetPos(keepPos);
    return result;
}

bool IsTerm (char cur, const char* termsP)
{
    for ( ; *termsP; termsP++ ) {
        if ( *termsP == cur )
            return true;
    }
    return false;
}

RetStatus WordReader::ReadWord(const char* termsP)
{
    //LOG("_srcP='%s'", _srcP ? _srcP : "NULL");
    _dst.Clean( );

    while ( Cur( ) == ' ' )  // skip whitespaces
        NextChar( );
    if ( Cur( ) == 0 )
        return E_RetStatus::Empty;

    if ( Cur( ) == '\"' ) {
        _dst.Print("%c", Cur( ));
        NextChar( );

        for ( ; (Cur( ) != 0) && (Cur( ) != '\"'); NextChar( ) )
            _dst.Print("%c", Cur( ));
        if ( Cur( ) == 0 )
            return E_RetStatus::Parsing;

        _dst.Print("%c", Cur( ));
        NextChar( );
        return E_RetStatus::Ok;
    }

    // here not 0 " whitespace
    if ( (Cur( ) == ',') || IsTerm(Cur( ), termsP) ) {
        _dst.Print("%c", Cur( ));
        NextChar( );
        return E_RetStatus::Ok;
    }

    // here not 0 " whitespace , separator
    while ( (Cur( ) != 0) && (!IsTerm(Cur( ), termsP)) && (Cur( ) != ',') ) {
        _dst.Print("%c", Cur( ));
        NextChar( );
    }
    return E_RetStatus::Ok;
}

ListElement WordReader::ReadListElement( )
{
    _dst.Clean( );

    while ( Cur( ) == ' ' )
        NextChar( );  // skip spaces

    if ( isdigit(Cur( )) ) {
        for ( ; isdigit(Cur( )); NextChar( ) )
            _dst.Print("%c", Cur( ));
        if ( (Cur( ) != ' ') && (Cur( ) != 0) && (Cur( ) != ',') && (Cur( ) != '-') )
            return ListElement::ErrorCli( );

        while ( Cur( ) == ' ' )
            NextChar( );  // skip spaces

        if ( Cur( ) != '-' )
            return ListElement::NumberCli(_dst.AsStr( ));
        NextChar( );
        _dst << " - ";
        while ( Cur( ) == ' ' )
            NextChar( );  // skip spaces
        if ( isdigit(Cur( )) == 0 )
            return ListElement::ErrorCli( );

        for ( ; isdigit(Cur( )); NextChar( ) )
            _dst.Print("%c", Cur( ));
        if ( (Cur( ) != ' ') && (Cur( ) != 0) && (Cur( ) != ',') )
            return ListElement::ErrorCli( );

        while ( Cur( ) == ' ' )
            NextChar( );  // skip spaces

        return ListElement::RangeCli(_dst.AsStr( ));
    }
    if ( isalpha(Cur( )) ) {
        RetStatus status = ReadWord( );
        while ( Cur( ) == ' ' )
            NextChar( );  // skip spaces
        return status.ok( ) ? ListElement::WordCli(_dst.AsStr( )) : ListElement::ErrorCli( );
    }
    return ListElement::ErrorCli( );

    //	while(Cur() == ' ') NextChar(); // skip spaces
    //	return E_RetStatus::Ok;
}

//////////////////////////////////////////////////////////

CliContextExe::CliContextExe(std::string_view line)
{
    _status = E_RetStatus::Ok;
    ocli.Clean( );

    _originalLine.Print(line.length( ), line.data( ));
}

CliContextTab::CliContextTab(MsgCliTab* msgP) : _isAsk(msgP->_isAsk)
{
    _status = E_RetStatus::Ok;
    ocli.Clean( );

    int cliCurrentPos = msgP->_cliCurrentPos;

    // find start of last word to be auto-completed
    unsigned prefixPos = cliCurrentPos;
    while ( prefixPos > 0 ) {
        if ( (msgP->_cliLine[prefixPos - 1] == ' ') || (msgP->_cliLine[prefixPos - 1] == ',') )
            break;
        prefixPos--;
    }

    _originalLine.Print(prefixPos, msgP->_cliLine);
    _tabPrefix.Print(cliCurrentPos - prefixPos, msgP->_cliLine + prefixPos);
}

void PrintCliHelperRoot( );       // in AdapterCli.cpp
void PrintCmdHelp(CliCmd* cmdP);  // in AdapterCli.cpp

void HelperData::PrintHelp( )
{
    switch ( _helperType ) {
        case HELPER_ROOT:
            {
                // build common help for normal or debug mode
                PrintCliHelperRoot( );
            }
            break;
        case HELPER_CMD:
            {
                ocli << "\n";
                PrintCmdHelp(_cliCmdP);
            }
            break;
        case HELPER_MO:
            {
                Mo* sampleP = SampleMo(_moType);

                Mo::_numOfReadersId = 0;

                ocli << "\n";
                if ( _cmdType == CMD_SHOW ) {
                    Mo::_cmdType = CMD_SHOW;
                    sampleP->DoHandleShow(ACTION_CLI_HELP);
                } else if ( _cmdType == CMD_SET ) {
                    Mo::_cmdType = CMD_SET;
                    sampleP->DoHandleSet(ACTION_CLI_HELP);
                } else if ( _cmdType == CMD_CLEAR ) {
                    Mo::_cmdType = CMD_CLEAR;
                    sampleP->DoHandleClear(ACTION_CLI_HELP);
                }
            }
            break;
        case HELPER_VALUE:
            {
                _valueP->PrintHelp( );
            }
            break;
        case HELPER_FREE_TEXT:
            {
                ocli << "\n";
                ocli << _freeText;
            }
            break;
    };
}

/////////////////////////////////////////////////////////////

bool CliContextExe::CheckExit(bool printError)
{
    if ( _status.ok( ) )
        return false;

    if ( printError ) {
        if ( _isCmdHelp ) {
            ocli << "\n";
            _helper.PrintHelp( );
            ocli << "\n";
        } else {
            ocli << "\n"
                 << "%% " << _status.message( );
            LOG("ERROR '%s'", _status.message( ).c_str());
            if ( _status.code( ) != E_RetStatus::Execution ) {
                MyStringT<512> tmp;
                tmp.Print(_pos, _originalLine.AsStr( ));
                ocli << ": " << tmp << "?"
                     << "\n";
                _helper.PrintHelp( );
            }
            ocli << "\n";
        }
    }

    return true;
}

void WordReader::ReadTo(END end)
{
    LOG("entry");
    (void)end;
    if ( !CheckExit(false) ) {
        if ( !IsExeMode( ) )
            ReturnAutoComplete(nullptr);
        else if ( !IsEol( ) )
            ReturnError(RetStatus {E_RetStatus::TooManyParams});
    }
    LOG("exit");
}

WordReader* WordReader::ReadTo(Value* dstP)
{
    LOG("entry");
    if ( !CheckExit(false) ) {
        LOG("goon");
        dstP->SetCli(this);
    }
    LOG("exit");
    return this;
}

CliContext& CliContext::SetAttrList(AttrListSet& dstR, Mo* dstMoP)
{
    LOG("entry");
    if ( CheckExit(false) ) {
        LOG("exit - CheckExit 1");
        return *this;
    }

    while ( (!IsEol( )) /*|| NowAutoComplete( )*/ ) {
        UINT32 i;
        // build dictionary of attrNames that are not handled yet
        ValueDictInternalT<50> attrName;
        for ( i = 0; i < dstR._numOf; i++ ) {
            if ( !dstR.IsUsed(i) )
                attrName._dictionary.Add(dstR.GetAttr(i)._attrType, dstMoP->FindAttr(dstR.GetAttr(i))->GetNameP( ));
        }

        // read attrName
        if ( !CheckExit(false) )
            attrName.SetCli(this);
        if ( CheckExit(false) ) {
            LOG("exit - CheckExit 2");
            return *this;
        }

        for ( i = 0; i < dstR._numOf; i++ ) {
            if ( dstR.GetAttr(i) != (T_AttrType)attrName.GetUint32( ) )
                continue;
            dstR.SetUsed(i);
            break;
        }

        // read attrValue
        T_AttrType attrType = (T_AttrType)attrName.GetUint32( );
        //		HelpAssignValue( _moType, attrType);
        Value* valueP = dstMoP->AttrValue(attrType);
        _helper.HelpAssignValue(valueP);

        if ( !CheckExit(false) ) {
            valueP->SetCli(this);
            // this variable dictates if the word that's being printed in Cli is part of a list
            // of values, which means that when Tab is pressed then no space will be added.
            _isWordPartOfList = valueP->IsWordInCliPartOfList( );
        }
        if ( CheckExit(false) ) {
            LOG("exit - CheckExit 3");
            return *this;
        }

        //		if(attrName.GetNumOf() == 0)
        //			break; // all are handled already
    }

    // check that all mandatory attributes are inserted
    for ( UINT32 i = 0; i < dstR._numOf; i++ ) {
        if ( dstR.IsMandat(i) && !dstR.IsUsed(i) ) {
            MyStringT<128> tmp;
            tmp << FOCUS(dstMoP) << NAME(dstR.GetAttr(i)) << " is mandatory but not specified. Operation failed.";
            LOG("exit - no mandatory");
            ReturnError(RetStatus {tmp});
            return *this;
        }
    }

    LOG("exit");
    return *this;
}

CliContext& CliContext::operator>> (AttrListSet& dstR)
{
    return SetAttrList(dstR, SampleMo(dstR._moType));
}

/////////////////////////////////////////////////////////////

void CliContextTab::ReturnAutoComplete(DictAutoCompletion* dictionary, int multi_step_separator)
{
    if ( _isAsk ) {
        _isDone = true;
        ocli.Clean( );
        _helper.PrintHelp( );
    }
    // else {
    //     _isDone = true;
    //     _status = RetStatus {E_RetStatus::Tab};

    //     if ( dictionary == nullptr )
    //         return;

    //     AutoComplete ac(_tabPostfix.AsStr( ), MAX_CLI_INPUT_LEN + 1, _tabPrefix.AsStr( ), multi_step_separator);
    //     UINT32       context;
    //     for ( dictionary->___FirstEntry(&context); dictionary->___GetEntry(context).IsValid( ); dictionary->___NextEntry(&context) ) {
    //         ac.Update(dictionary->___GetEntry(context)._nameP);
    //     }

    //     // make sure that the auto completion doesn't complete a random word
    //     // from the dictionary:
    //     if ( _tabPrefix.IsEmpty( ) && !ac.WasFullMatch( ) ) {
    //         _tabPostfix.Clean( );
    //         return;
    //     }

    //     if ( ac.WasMatch( ) )
    //         _status = E_RetStatus::Ok;
    //     _wasFullWordCompleted = ac.WasFullMatch( );
    // }
}

//////////////////////////////////////////////////////////

CliTable& CliTable::Column(char* nameP, int width)
{
    if ( _numOfColumns < MAX_COLUMNS ) {
        ColumnDef* columnP = &_columns[_numOfColumns++];
        columnP->_width    = width + 1;
        sprintf(columnP->_format, "%%-%ds", columnP->_width);
        columnP->_name << nameP;
    }
    return *this;
}

CliTable& CliTable::Column(int width)
{
    if ( _numOfColumns < MAX_COLUMNS ) {
        ColumnDef* columnP = &_columns[_numOfColumns++];
        columnP->_width    = width + 1;
        sprintf(columnP->_format, "%%-%ds", columnP->_width);
        columnP->_name << "";
    }
    return *this;
}

CliTable& CliTable::Cell(const char* srcP)
{
    if ( _curColumn < _numOfColumns ) {
        _rowString.Print(_columns[_curColumn]._format, srcP);
        _curColumn++;
    }
    return *this;
}

CliTable& CliTable::Cell(Value& srcR)
{
    MyStringT<128> text;
    text << srcR;
    return Cell(text.AsStr( ));
}

// CliTable& CliTable::Cell(Mac src)
// {
//     MyStringT<128> text;
//     text << src;
//     return Cell(text.AsStr( ));
// }

CliTable& CliTable::Cell(int src)
{
    MyStringT<128> text;
    text << src;
    return Cell(text.AsStr( ));
}

// CliTable& CliTable::CellCounter64(UINT64 value)
// {
//     ValueCounter64 valueCounter(value);
//     return Cell(valueCounter);
// }

// CliTable& CliTable::CellCounterSigned64(UINT64 value)
// {
//     ValueCounter64 valueCounter(value);
//     valueCounter.AssignSignedPrintFormat(true);
//     return Cell(valueCounter);
// }

// CliTable& CliTable::CellDateAndTime(time_t value)
// {
//     ValueDateAndTime valueDateAndTime(value);
//     return Cell(valueDateAndTime);
// }

void CliTable::PrintRow( )
{
    ocli << _rowString << "\n";
}

void CliTable::PrintHeader( )
{
    StartRow( );
    for ( int i = 0; i < _numOfColumns; i++ ) {
        ColumnDef* columnP = &_columns[i];
        Cell(columnP->_name.AsStr( ));
    }
    PrintRow( );
}

/////////////////////////////////////////////////////////////
