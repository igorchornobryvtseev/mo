#ifndef CLICONTEXT_H_INCLUDED
#define CLICONTEXT_H_INCLUDED

#include "AutoComplete.h"
// #include "BitMap.h"
// #include "CaExternal.h"
#include "Dictionary.h"
// #include "gl_types.h"
// #include "MyString.h"
#include "RetStatus.h"

struct MsgCliTab;
class CliContext;
class CliCmd;
class Value;
class Dictionary;
class AttrListSet;

///////////////////////////////////////////

struct END {
    explicit END( ) = default;
};

///////////////////////////////////////////

struct ListElement {
private:
    enum Type { ELEMENT_ERROR, ELEMENT_NUMBER, ELEMENT_RANGE, ELEMENT_WORD };

    Type _type;

    bool _isSnmp;

    int _minRange;
    int _maxRange;

    int _intValue;

    MyStringT<64> _word;  // word or number

    static ListElement FromCli (Type type, char* wordP)
    {
        ListElement element;
        element._type   = type;
        element._isSnmp = false;
        element._word << wordP;
        return element;
    }

public:
    bool IsSnmp ( ) { return _isSnmp; }

    RetStatus GetStatus ( ) const { return RetStatus {(_type != ELEMENT_ERROR) ? E_RetStatus::Ok : E_RetStatus::Parsing}; }

    bool IsRange ( ) { return (_type == ELEMENT_RANGE); }

    int GetMinRange ( ) { return _minRange; }

    int GetMaxRange ( ) { return _maxRange; }

    int GetInt ( ) { return _intValue; }

    char* GetStrP ( ) { return _word.AsStr( ); }  // of number or word

    static ListElement NumberSnmp (int intValue)
    {
        ListElement element;
        element._type     = ELEMENT_NUMBER;
        element._isSnmp   = true;
        element._intValue = intValue;
        element._word << intValue;
        return element;
    }

    static ListElement ErrorCli ( ) { return FromCli(ELEMENT_ERROR, ""); }

    static ListElement WordCli (char* strP) { return FromCli(ELEMENT_WORD, strP); }

    static ListElement NumberCli (char* strP)
    {
        ListElement element = FromCli(ELEMENT_NUMBER, strP);
        element._intValue   = element._word.AsInt( );
        return element;
    }

    static ListElement RangeCli (char* strP)
    {
        ListElement element = FromCli(ELEMENT_RANGE, strP);
        sscanf(element._word.AsStr( ), "%d - %d", &element._minRange, &element._maxRange);
        return element;
    }
};

class WordReader
{
public:
    virtual ~WordReader( ) = default;

    void        ReadTo(END end);
    WordReader* ReadTo(Value* dstP);

    // virtual behavior
    virtual char Cur( )                                                                                             = 0;
    virtual bool IsExeMode( ) const                                                                                 = 0;
    virtual void ReturnAutoComplete(DictAutoCompletion* dict, int multi_step_separator = AutoComplete::SINGLE_STEP) = 0;
    virtual bool CheckExit(bool printError = false)                                                                 = 0;

    void NextChar ( ) { _pos++; }

    UINT32 GetPos ( ) const { return _pos; }

    void SetPos (UINT32 pos) { _pos = pos; }

    // read
    bool IsEol ( )
    {
        while ( Cur( ) == ' ' )
            NextChar( );
        return (Cur( ) == 0);
    }

    RetStatus ReadWord(char* termsP = " ");

    ListElement ReadListElement( );

    bool IsOk ( ) const { return _status.ok( ); }

    bool operator== (const char* stringP)
    {
        if ( !IsOk( ) )
            return false;
        return (_dst == stringP);
    }

    WordReader& Read ( )
    {
        RetStatus status = ReadWord( );
        SetStatus(status);
        return *this;
    }

    bool TestNext(char* sampleP);
    bool TestExisting(char* sampleP);

    bool IsEnter ( ) { return IsExeMode( ) && IsEol( ); }

    bool NowAutoComplete ( ) { return (!IsExeMode( )) && IsEol( ); }

    RetStatus Status ( ) const { return _status; }

    void SetStatus (RetStatus status) { _status = status; }

    void ReturnError (RetStatus status) { _status = status; }

    bool IsEolExe ( ) { return IsEol( ) && IsExeMode( ); }

    bool IsError ( ) const { return !_status.ok( ); }

    bool HandleAutoComplete ( )
    {
        bool nowAutoComplete = NowAutoComplete( );
        if ( nowAutoComplete )
            ReturnAutoComplete(nullptr);
        return nowAutoComplete;
    }

    void ReturnErrorAndPrint (RetStatus status)
    {
        _status = status;
        CheckExit(true);  // it is necessary to print error
    }

    MyStringT<MAX_CLI_INPUT_LEN> _dst;
    RetStatus                    _status;
    UINT32                       _pos {0};
    bool                         _isWordPartOfList {false};
};

struct WordReaderString : public WordReader {
    WordReaderString(const char* srcP) : _srcP((char*)srcP) { }

    WordReaderString(MyString* srcP) : _srcP(srcP->AsStr( )) { }

    char Cur ( ) override { return _srcP[_pos]; }

    bool IsExeMode ( ) const override { return true; }

    void ReturnAutoComplete (DictAutoCompletion* dict, int multi_step_separator = AutoComplete::SINGLE_STEP) override
    {
        (void)dict;
        (void)multi_step_separator;
    }

    bool CheckExit (bool printError = false) override
    {
        (void)printError;
        return !Status( ).ok( );
    }

    char* _srcP;
};

struct HelperData {
    enum HelperType { HELPER_ROOT, HELPER_CMD, HELPER_MO, HELPER_VALUE, HELPER_FREE_TEXT };

    void HelpAssignRoot ( ) { _helperType = HELPER_ROOT; }

    void SetAssignCmd (CliCmd* cliCmdP)
    {
        _helperType = HELPER_CMD;
        _cliCmdP    = cliCmdP;
    }

    void HelpAssignMo (int cmdType, T_MoType moType)  // CmdType
    {
        _helperType = HELPER_MO;
        _cmdType    = cmdType;
        _moType     = moType;
    }

    void HelpAssignFreeText (char* freeTextP)
    {
        _helperType = HELPER_FREE_TEXT;
        _freeText.Clean( );
        _freeText << freeTextP;
    }

    void HelpAssignValue (Value* valueP)
    {
        _helperType = HELPER_VALUE;
        _valueP     = valueP;
    }

    void PrintHelp( );

    HelperType     _helperType;
    CliCmd*        _cliCmdP;
    int            _cmdType;  // CmdType
    T_MoType       _moType;
    MyStringT<640> _freeText;
    Value*         _valueP;
};

class CliContext : public WordReader
{
public:
    using WordReader::WordReader;

    void operator>> (END end) { ReadTo(end); }

    CliContext& operator>> (Value* dstP)
    {
        ReadTo(dstP);
        return *this;
    }

    CliContext& operator>> (Value& dstR)
    {
        ReadTo(&dstR);
        return *this;
    }

    CliContext& SetAttrList(AttrListSet& dstR, Mo* dstMoP);
    CliContext& operator>> (AttrListSet& dstR);

    MyStringT<MAX_CLI_INPUT_LEN> _originalLine;  // not include tab prefix in auto-compl mode
    HelperData                   _helper;

    bool _ignorePermissions {false};
};

class CliContextExe : public CliContext
{
public:
    explicit CliContextExe(std::string_view line);

    char Cur ( ) override { return _originalLine.AsStr( )[_pos]; }

    bool IsExeMode ( ) const override { return true; }

    void ReturnAutoComplete (DictAutoCompletion* /*dict*/, int /*multi_step_separator*/ = AutoComplete::SINGLE_STEP) override { }

    bool CheckExit(bool printError = false) override;

    bool _isCmdHelp {false};
};

class CliContextTab : public CliContext
{
public:
    CliContextTab(MsgCliTab* msgP);  // for ask or tab mode

    char Cur ( ) override { return _originalLine.AsStr( )[_pos]; }

    bool IsExeMode ( ) const override { return false; }

    void ReturnAutoComplete(DictAutoCompletion* dict, int multi_step_separator = AutoComplete::SINGLE_STEP) override;

    bool CheckExit (bool printError = false) override
    {
        (void)printError;
        return (!_status.ok( )) || _isDone;
    }

    bool _isDone {false};
    bool _isAsk;
    bool _wasFullWordCompleted {true};

    MyStringT<MAX_CLI_INPUT_LEN> _tabPrefix;
    MyStringT<MAX_CLI_INPUT_LEN> _tabPostfix;
};

class CliTable
{
public:
    CliTable( ) { StartRow( ); }

    CliTable& Column(int width);
    CliTable& Column(char* nameP, int width);

    CliTable& StartRow ( )
    {
        _curColumn = 0;
        _rowString.Clean( );
        return *this;
    }

    void PrintHeader( );
    void PrintRow( );

    CliTable& Cell(const char* srcP);
    CliTable& Cell(Value& srcR);
    CliTable& Cell(int src);
    // CliTable& Cell(Mac src);
    CliTable& CellCounter64(UINT64 value);
    CliTable& CellCounterSigned64(UINT64 value);
    CliTable& CellDateAndTime(time_t value);

    enum { MAX_COLUMNS = 20 };

    struct ColumnDef {
        int           _width;
        char          _format[16];
        MyStringT<32> _name;
    };

    ColumnDef _columns[MAX_COLUMNS];
    int       _numOfColumns {0};
    int       _curColumn {0};

    MyStringT<128> _rowString;
};

///////////////////////////////////////////

#endif  // CLICONTEXT_H_INCLUDED
