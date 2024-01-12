#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <functional>

// #include "BitMap.h"
// #include "gl_types.h"
#include "MoType.h"

//////////////////////////////////////////

class MoId
{
public:
    explicit MoId(INT64 id) : _id(id) { }

    MoId( ) : _id(0) { }

    INT64 AsNumber ( ) { return _id; }

    bool operator== (MoId second) { return _id == second._id; }

    bool operator!= (MoId second) { return _id != second._id; }

    bool operator> (MoId second) { return _id > second._id; }

private:
    INT64 _id;
};

//////////////////////////////////////////

enum T_MoActionStatus { ACTION_NONE = 0, ACTION_CREATED = 0x1, ACTION_DELETED = 0x2, ACTION_MODIFIED = 0x4 };

inline const char* to_string (T_MoActionStatus act)
{
    switch ( act ) {
        case ACTION_NONE: return "none";
        case ACTION_CREATED: return "created";
        case ACTION_MODIFIED: return "modified";
        case ACTION_DELETED: return "deleted";
        default: return nullptr;
    }
}

class Mo;

typedef UINT16 MainIndex;  // index in the main MO table

#define MAX_NUM_MOS 20000

void InitSampleMo( );

Mo*  SampleMo(T_MoType moType);
Mo*  SampleMo(const char* name);
void setVisibleMo(T_MoType moType, bool visible);
bool isVisibleMo(T_MoType moType, bool ignoreDebug = false);

template<class MO_CLASS>
MO_CLASS* SampleMoT ( )
{
    return dynamic_cast<MO_CLASS*>(SampleMo(MO_CLASS::MO_TYPE));
}

// ***************************************************************

// ordered by number

class ValueMo
{
public:
    static void DeleteForced ( ) { _DeleteMo(ACTION_NONE); }

    static Mo*  _FindMo(T_MoType moType, MoId moNumber);
    static void _AddMo(Mo* newMoP);
    static void _DeleteMo(T_MoActionStatus action);  // ACTION_NONE is used for DeleteForced and include SNMP instance deletion

    ValueMo(T_MoType moType = MO_MAX) : _moType(moType) { }

    virtual ~ValueMo( ) = default;

    Mo* MoP( );

    template<class MO>
    MO* MoP ( )
    {
        return dynamic_cast<MO*>(MoP( ));
    }

    virtual bool IsValid ( ) { return _sortedIndex < ValueMo::_numOfSorted; }

    void SetFirst ( )
    {
        _sortedIndex = 0;
        if ( IsValid( ) && !IsMatch( ) )
            SetNext( );
    }

    void SetNext ( )
    {
        if ( IsValid( ) )
            for ( _sortedIndex++; IsValid( ) && !IsMatch( ); _sortedIndex++ )
                ;
    }

    // std algorithm analogs for ValueMo iterator

    // usage:
    // ValueMo::for_each<MoEthernet>([](auto* pEtehrnet) // <-- note MoEthernet duplication removed
    // {
    //       pEthernet->setAdmin(ADMIN_UP);
    // });
    template<class MO>
    static void for_each (std::function<void(MO*)> func)
    {
        auto valueMo = ValueMo(MO::MO_TYPE);

        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            MO* moP = valueMo.MoP<MO>( );
            if ( moP )
                func(moP);
        }
    }

    // usage:
    // ValueMo::for_every([](Mo* pMo)
    // {
    //         pMo->setVisible(VISIBLE_ON);
    // });
    static void for_every (std::function<void(Mo*)> func)
    {
        ValueMo valueMo;
        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            Mo* moP = valueMo.MoP( );
            if ( moP )
                func(moP);
        }
    }

    // usage:
    // auto pRunningEthernet = ValueMo::find_if<MoEthernet>([](auto* pEtehrnet) // <-- note MoEthernet duplication removed
    // {
    //         return pEthernet->isAdminUp();
    // });
    template<class MO>
    static MO* find_if (std::function<bool(MO*)> func)
    {
        auto valueMo = ValueMo(MO::MO_TYPE);

        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            MO* moP = valueMo.MoP<MO>( );
            if ( moP && func(moP) )
                return moP;
        }
        return nullptr;
    }

    template<class MO>
    static MO* find_if_not (std::function<bool(MO*)> func)
    {
        auto valueMo = ValueMo(MO::MO_TYPE);

        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            MO* moP = valueMo.MoP<MO>( );
            if ( moP && !func(moP) )
                return moP;
        }
        return nullptr;
    }

    template<class MO>
    static bool any_of (std::function<bool(MO*)> func)
    {
        return ValueMo::find_if<MO>(func) != nullptr;
    }

    template<class MO>
    static bool none_of (std::function<bool(MO*)> func)
    {
        return ValueMo::find_if<MO>(func) == nullptr;
    }

    template<class MO>
    static bool all_of (std::function<bool(MO*)> func)
    {
        return ValueMo::find_if_not<MO>(func) == nullptr;
    }

    template<class MO>
    static size_t count_if (std::function<bool(MO*)> func)
    {
        size_t cnt     = 0;
        auto   valueMo = ValueMo(MO::MO_TYPE);

        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            MO* moP = valueMo.MoP<MO>( );
            if ( moP && func(moP) )
                cnt++;
        }
        return cnt;
    }

protected:
    virtual bool IsMatch( );

private:
    static UINT32    _numOfSorted;
    static MainIndex _sortedTable[MAX_NUM_MOS];

    T_MoType _moType;
    UINT32   _sortedIndex {0};
};

template<class MO_CLASS>
struct ValueMoT : public ValueMo {
    ValueMoT( ) : ValueMo(MO_CLASS::MO_TYPE) { }

    MO_CLASS* MoT ( ) { return (MO_CLASS*)MoP( ); }
};

class ValueMoMatchCli : public ValueMo
{
public:
    ValueMoMatchCli(T_MoType moType) : ValueMo(moType) { }

    bool IsValid( ) override;

    template<class MO>
    static void for_each (std::function<void(MO*)> func)
    {
        auto valueMo = ValueMoMatchCli(MO::MO_TYPE);

        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            MO* moP = valueMo.MoP<MO>( );
            if ( moP )
                func(moP);
        }
    }

protected:
    bool IsMatch( ) override;
};

//////////////////////////////////////////

class Database
{
public:
    Database( ) { bzero(_mainTable, sizeof(_mainTable)); }

    Mo* MoByMainIndex (MainIndex mainIndex) { return _mainTable[mainIndex]; }

    void AddMo (Mo* newMoP) { ValueMo::_AddMo(newMoP); }

private:
    friend class ValueMo;

    Mo* _mainTable[MAX_NUM_MOS];
    //	BitMapT<0,(MAX_NUM_MOS-1)> _occupiedMainTable;
};

Database* Db( );

//////////////////////////////////////////

void TransactionStart( );

enum TransactionEndType { TRANSACTION_END_NORMAL, TRANSACTION_END_CONFIG, TRANSACTION_END_CONFIG_DEFAULT };

void TransactionEnd(TransactionEndType endType);
void TransactionEndRestore( );

//////////////////////////////////////////

#endif  // DATABASE_H_INCLUDED
