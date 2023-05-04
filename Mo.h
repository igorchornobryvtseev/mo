#pragma once

// #include "CaExternal.h"
 #include "Database.h"
// #include "DebugFlag.h"
// #include "Dictionary.h"
// #include "gl_types.h"
 #include "MoType.h"
#include "MyArray.h"
// #include "MyDump.h"
// #include "MyOid.h"
// #include "RetStatus.h"
#include "Value.h"

class MyString;
class CliContext;

class CliReaderIdBase;
class CliModifierShow;
class AttrListSet;

class AttrArray;
class Xml;

class CountersLine;
class CountersBucket;

class SnmpSetList;

//////////////////////////////////////////////////////////

#define FLAG_SET      0x1
#define FLAG_INFO     0x2
#define FLAG_HIDE     0x4

#define NO_CLI        (0)
#define CLI_INFO      (FLAG_INFO)
#define CLI_INFO_CONF (FLAG_INFO | FLAG_SET)
#define CLI_CONF_ONLY (FLAG_SET)

#define INTERFACE_LEN (64)

void CreateDefaultDatabase(bool firstTime);

enum ActionType {
    GET_MO_TYPE,      // mandatory
    PRINT_TYPE_NAME,  // mandatory
    GET_MO_NUMBER,    // optional,    default is 1 - singletone
    PRINT_FULL_NAME,  // optional,    default is type name - singletone

    REGISTER_STATIC,  // mandatory
    CONFIRM_CHANGE,   // optional,    returns status, default is OK
    UPDATE_DYNAMIC,   // optional,    default is do nothing

    ACTION_CLI_CMD,
    ACTION_CLI_HELP,

    CREATE_SNMP_COLUMNS,
    CREATE_SNMP_INSTANCE,
    DELETE_SNMP_INSTANCE
};

constexpr const char* to_string (ActionType e, const char* defValue = nullptr) noexcept
{
    switch ( e ) {
        case ActionType::GET_MO_TYPE:           return "GET_MO_TYPE";
        case ActionType::PRINT_TYPE_NAME:       return "PRINT_TYPE_NAME";
        case ActionType::GET_MO_NUMBER:         return "GET_MO_NUMBER";
        case ActionType::PRINT_FULL_NAME:       return "PRINT_FULL_NAME";

        case ActionType::REGISTER_STATIC:       return "REGISTER_STATIC";
        case ActionType::CONFIRM_CHANGE:        return "CONFIRM_CHANGE";
        case ActionType::UPDATE_DYNAMIC:        return "UPDATE_DYNAMIC";

        case ActionType::ACTION_CLI_CMD:        return "ACTION_CLI_CMD";
        case ActionType::ACTION_CLI_HELP:       return "ACTION_CLI_HELP";

        case ActionType::CREATE_SNMP_COLUMNS:   return "CREATE_SNMP_COLUMNS";
        case ActionType::CREATE_SNMP_INSTANCE:  return "CREATE_SNMP_INSTANCE";
        case ActionType::DELETE_SNMP_INSTANCE:  return "DELETE_SNMP_INSTANCE";
    }
    return defValue != nullptr ? defValue : "undefined";
}

//////////////////////////////////////////////////////////

enum CmdType {
    CMD_HELP,
    CMD_SHOW,
    CMD_SET,
    CMD_CLEAR,
    CMD_RESET,
    CMD_COPY,
    CMD_DEBUG,
    CMD_DEBUG_FLAG,
    CMD_DEBUG_NPU,
    CMD_DEBUG_ACTION,
    CMD_NPU,
    CMD_CFM,
    CMD_RUN,
    CMD_ACCEPT,
    CMD_DIR,
    CMD_PING,
    CMD_PING6,
    CMD_TRACERT,
    CMD_TRACERT6,
    CMD_IPERF,
    CMD_SPECTRUM,
    CMD_BIST,
    CMD_RELOAD,
    CMD_PIC,
    CMD_MAX
};

//////////////////////////////////////////////////////////

struct AttrId {
    AttrId( ) : _attrType(ATTR_INVALID) { }

    AttrId(int32_t attrType) : _attrType((T_AttrType)attrType) { }

    bool IsValid ( ) { return _attrType != ATTR_INVALID; }

    bool operator== (AttrId second) { return _attrType == second._attrType; }

    bool operator!= (AttrId second) { return _attrType != second._attrType; }

    T_AttrType _attrType;
};

//////////////////////////////////////////////////////////

class Mo
{
public:
    static Mo* FindMo (T_MoType moType, MoId moId = MoId( )) { return ValueMo::_FindMo(moType, moId); }

    template<class MO_CLASS>
    static MO_CLASS* FindMo (T_MoType moType, MoId moId = MoId( ))
    {
        return dynamic_cast<MO_CLASS*>(ValueMo::_FindMo(moType, moId));
    }

    template<class MO_CLASS>
    static MO_CLASS* FindMoT (MoId moId = MoId( ))
    {
        return dynamic_cast<MO_CLASS*>(ValueMo::_FindMo(MO_CLASS::MO_TYPE, moId));
    }

    static bool CopySampleTo(Mo* dstP, AttrListSet* attrListP);

    // entire MO handling
    T_MoType Type( ) const;
    char*    TypeName( );

    MoId GetNumber( );

    static Mo* BuildNew(T_MoType moType, void* extraP = nullptr);  // creates only, does not insert to the list

    template<class MO_CLASS>
    static MO_CLASS* BuildNewT (void* extraP = nullptr)
    {
        return dynamic_cast<MO_CLASS*>(BuildNew(MO_CLASS::MO_TYPE, extraP));
    }

    template<class MO_CLASS>
    static size_t NumOf ( )
    {
// #if __cplusplus < 202000L
//         static_assert(std::is_base_of_v<Mo, MO_CLASS>);
// #endif
        return ValueMo::count_if<MO_CLASS>([] (const MO_CLASS*) {
            return true;
        });
    }

    static int  NumOf(T_MoType moType);
    static bool Has(T_MoType moType);

    void MarkToDelete( );
    void MarkToDeleteForced( );

    Value* AttrValue(AttrId attrId) const;
    UINT32 AttrUint32(AttrId attrId) const;
    template<class ENUM>
// #if __cplusplus > 202000L
//     requires std::is_enum_v<ENUM> /*&& std::convertible_to<uint32_t, ENUM>*/
// #endif
        ENUM AttrEnum (AttrId attrId) const
    {
// #if __cplusplus < 202000L
//         static_assert(std::is_enum_v<ENUM> /*&& std::is_convertible_v<uint32_t, ENUM>*/);
// #endif
        return AttrValue(attrId) ? AttrValue(attrId)->GetEnum<ENUM>( ) : static_cast<ENUM>(0);
    }
    bool AttrWasChanged(const AttrId& attrId);
    bool AttrWasChanged(const std::vector<AttrId>& attrId);
    bool WasStatsClear( );

    // data control
    T_MoActionStatus Action( );

    void UpdateDynamic( );
    void ResetStatistics( );

    time_t _elapsedBaseTimeSecs;

    virtual ~Mo( ) = default;

    virtual RetStatus DoPerform(ActionType performType, void* dataP) = 0;
    virtual void      DoHandleShow(ActionType cmdType)               = 0;
    virtual void      DoHandleSet(ActionType cmdType);
    virtual void      DoHandleClear(ActionType cmdType);

    virtual void DoPostConfig (T_MoActionStatus action)
    {
        if ( action == ACTION_CREATED )
            DoSnmpPerform(CREATE_SNMP_INSTANCE);
        else if ( action == ACTION_DELETED )
            DoSnmpPerform(DELETE_SNMP_INSTANCE);
    }

    virtual void DoSamplePostConfig ( ) { }

    virtual RetStatus DoSampleSnmpCreate (Mo** /*dstPP*/, SnmpSetList* /*snmpListP*/) { return RetStatus {E_RetStatus::NotOk}; }

    virtual RetStatus DoSampleSnmpDelete (Mo* moP)
    {
        moP->MarkToDelete( );
        return RetStatus {E_RetStatus::Ok};
    }

    virtual void DoInsertToDb ( ) { }

    virtual void DoRemoveFromDb ( ) { }

    void RegisterCreatable( );
    bool IsCreatable( );

    void Cli_PrintStandardHelpShow(CliModifierShow* m);
    void Cli_PrintStandardHelpClear( );
    void Cli_PrintStandardHelpSet(AttrListSet* attrListP);

    void Cli_PrintStandardShow(CliModifierShow* modifierP, AttrArray* infoTableAttrsP);
    void Cli_PrintStandardShowInfoAsList(CliModifierShow* modifierP);
    void Cli_PrintStandardShowInfoAsTable(AttrArray* infoTableAttrsP);
    void Cli_PrintStandardShowStatistics(CliModifierShow* modifierP);

    virtual void DoSampleXmlShow(Xml* xmlP, MyString* instancesP);

    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////

    virtual CountersLine* GetCountersLine ( ) { return nullptr; }

    virtual CountersBucket* GetCountersBucket ( ) { return nullptr; }

    virtual void DoSnmpPerform (ActionType performType) { (void)performType; }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    struct AttrDescriptor {
        void Init (AttrId attrId, char* nameP, UINT32 flags, Mo* baseMoP, Value* baseValueP)
        {
            _attrId = attrId;
            _nameP  = nameP;
            _offset = baseMoP->CalcOffset(baseValueP);
            _flags  = (UINT8)flags;
        }

        void Hide (bool beHidden)
        {
            if ( beHidden )
                _flags |= FLAG_HIDE;
            else
                _flags &= (~FLAG_HIDE);
        }

        bool IsHidden ( ) { return (_flags & FLAG_HIDE); }

        bool IsSettable ( ) { return IsHidden( ) ? false : (_flags & FLAG_SET); }

        bool IsInfo ( ) { return IsHidden( ) ? false : (_flags & FLAG_INFO); }

        char* GetNameP ( ) { return _nameP; }

        AttrId __Id ( ) { return _attrId; }

        Value* ValueP (const Mo* moP) { return (_offset != 0) ? moP->OffsetToValue(_offset) : GetInvalidValue( ); }

        Dictionary::Entry GetDictEntry ( ) { return Dictionary::Entry(_attrId._attrType, _nameP); }

    private:
        static Value* GetInvalidValue( );

        AttrId _attrId {T_AttrType::ATTR_INVALID};
        char*  _nameP {"????"};
        long   _offset {0};
        UINT8  _flags {NO_CLI};
    };

    class IteratorAttr
    {
    private:
        static MyArray<AttrDescriptor>* Attrs (T_MoType moType)
        {
            static MyArray<AttrDescriptor> attrs[MO_MAX];
            return &attrs[moType];
        }

    public:
        void __RegisterAttr (Mo* moP, AttrId attrId, UINT32 flags, Value* valueP, const char* nameP)
        {
            AttrDescriptor descr;
            descr.Init(attrId, (char*)nameP, flags, moP, valueP);
            Attrs(_moType)->Push(descr);
        }

        IteratorAttr(T_MoType moType) : _moType(moType) { }

        Mo::AttrDescriptor* operator->( ) const
        {
            static AttrDescriptor attrInvalid;
            return IsValid( ) ? (&(*Attrs(_moType))[_index]) : &attrInvalid;
        }

        IteratorAttr& operator++ (int)
        {
            if ( IsValid( ) )
                _index++;
            return *this;
        }

        bool IsValid ( ) const { return (_index < (UINT32)Attrs(_moType)->NumOf( )); }

        T_MoType _moType;
        UINT32   _index {0};
    };

    IteratorAttr BeginAttr ( ) const { return IteratorAttr(Type( )); }

    IteratorAttr FindAttr (AttrId attrId) const
    {
        auto it = BeginAttr( );
        for ( ; it.IsValid( ); it++ ) {
            if ( it->__Id( ) == attrId )
                break;
        }
        return it;
    }

    void RegisterAttr (AttrId attrId, UINT32 flags, Value* valueP, const char* nameP) { BeginAttr( ).__RegisterAttr(this, attrId, flags, valueP, nameP); }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    static CmdType          _cmdType;
    static int              _numOfReadersId;
    static CliReaderIdBase* _readersId[10];

    long CalcOffset (Value* valueP) { return (long)valueP - (long)(&_properties); }

    Value* OffsetToValue (long offset) const { return (Value*)((long)(&_properties) + offset); }

    MainIndex GetMainIndex ( ) { return _mainIndex; }

    void ResetMoChangeFlags ( ) { _transaction._asInt = 0; }

    virtual void TransactionEndRestore( );
    virtual void TransactionEndConfig( );
    virtual void KeepAllData( );

    // protected:
    Mo( );

    // private:
    union {
        UINT32 _asInt;

        struct {
            bool isSample        : 1;
            bool supportModified : 1;
            bool isCreatable     : 1;  // used in sample only
            bool isDefault       : 1;
            bool internalOnly;
        } _asBits;
    } _properties;  // use it as base line too

    void Touch ( ) { _transaction._asBits.modified = true; }

    union {
        UINT32 _asInt;

        struct {
            bool created : 1;
            bool deleted : 1;
            bool forced  : 1;

            bool modified        : 1;
            bool resetStatistics : 1;

            bool dynamicUpdated : 1;  // currently used in SNMP only
        } _asBits;
    } _transaction;

    MainIndex _mainIndex;

    friend class Database;  // to keep _mainIndex
    friend void TransactionEnd(TransactionEndType);
};

// void BuildAllScript(MyOutStream* outP);
// void BuildAllDebugScript(MyOutStream* outP);

MyOutStream& operator<< (MyOutStream& dstR, const Mo* srcP);

//////////////////////////////////////////////////////////

struct FOCUS {
    FOCUS(Mo* moP) : _moFocusP(moP) { }

    Mo* _moFocusP;
};

MyOutStream& operator<< (MyOutStream& dstR, FOCUS aFocus);

struct NAME {
    NAME(AttrId attrId, int width = -1) : _attrId(attrId), _width(width) { }

    AttrId _attrId;
    int    _width;
};

MyOutStream& operator<< (MyOutStream& dstR, NAME aName);

struct VAL {
    VAL(AttrId attrId, int width = -1) : _attrId(attrId), _width(width) { }

    AttrId _attrId;
    int    _width;
};

MyOutStream& operator<< (MyOutStream& dstR, VAL aValue);

struct NAME_VAL {
    NAME_VAL(AttrId attrId, int width = -1) : _attrId(attrId), _width(width) { }

    AttrId _attrId;
    int    _width;
};

MyOutStream& operator<< (MyOutStream& dstR, NAME_VAL aNameValue);

struct NAME_VAL_FILTERED {
    NAME_VAL_FILTERED(AttrId attrId, int width = -1) : _attrId(attrId), _width(width) { }

    AttrId _attrId;
    int    _width;
};

MyOutStream& operator<< (MyOutStream& dstR, NAME_VAL_FILTERED aNameValue);

struct FIELD {
    FIELD(const char* stringP, int width = -1) : _dataType(DATA_STRING), _width(width) { _data._stringP = stringP; }

    FIELD(UINT64 aUint64, int width = -1) : _dataType(DATA_INT), _width(width) { _data._uint64 = aUint64; }

    FIELD(Value* valueP, int width = -1) : _dataType(DATA_VALUE), _width(width) { _data._valueP = valueP; }

    enum { DATA_STRING, DATA_INT, DATA_VALUE } _dataType;

    union {
        const char* _stringP;
        UINT64      _uint64;
        Value*      _valueP;
    } _data;

    int _width;
};

MyOutStream& operator<< (MyOutStream& dstR, FIELD aField);

// FMT_BEGIN_NAMESPACE

// template<>
// struct formatter<Mo> : formatter<std::string_view> {
//     template<typename FormatContext>
//     auto format (const Mo& mo, FormatContext& ctx) const
//     {
//         MyStringT<128> str;
//         ((Mo&)mo).DoPerform(PRINT_FULL_NAME, &str);
//         return formatter<std::string_view>::format(str, ctx);
//     }
// };

// FMT_END_NAMESPACE
