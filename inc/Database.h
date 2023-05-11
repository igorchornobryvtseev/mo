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

    /**    @brief Execute func for each Mo-derived object of given type.

           Before executing checks that object exists (pointer is not null).

           @tparam    MO    Specifies type of Mo object func should be called for
           @param[in] func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument:
                                @code{.cpp}
                                void func(MoObject* ptr);
                                @endcode

           @par Usage example:
                @code{.cpp}
                // Pre c++17 syntax
                ValueMo::for_each<MoEthernet>([](MoEthernet* pEtehrnet) // <-- note MoEthernet duplication
                {
                      pEthernet->setAdmin(ADMIN_UP);
                });

                // Post c++17 syntax
                ValueMo::for_each<MoEthernet>([](auto* pEtehrnet) // <-- note MoEthernet duplication removed
                {
                      pEthernet->setAdmin(ADMIN_UP);
                });
                @endcode

           @see std::for_each
           @see for_every
    */
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

    /*!    @brief Execute func for each Mo object, regardless of its type.

           Before executing checks that object exists (pointer is not null).

           @param[in] func  Functional object (function, functor, lambda) that accepts Mo object pointer as argument:
                                @code{.cpp}
                                void func(Mo* ptr);
                                @endcode

           @par Usage example:
                @code{.cpp}
                ValueMo::for_every([](Mo* pMo)
                {
                      pMo->setVisible(VISIBLE_ON);
                });
                @endcode

           @see std::for_each
           @see for_each
     */
    static void for_every (std::function<void(Mo*)> func)
    {
        ValueMo valueMo;
        for ( valueMo.SetFirst( ); valueMo.IsValid( ); valueMo.SetNext( ) ) {
            Mo* moP = valueMo.MoP( );
            if ( moP )
                func(moP);
        }
    }

    /**        @brief Find first Mo-derived object for which func returns @c true.
    *
    *          Execute func for each Mo-derived object of given type while func returns @c false.
    *          Return pointer to first object that func returned @c true for. Before executing checks that object exists (pointer is not null).
    *          If no object is found, returns @c nullptr. When func is called, it is guaranteed that pointer is not null.
    *
    *          @tparam    MO    Specifies type of Mo object func should be called for
    *          @param[in] func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument and returns @c bool:
    *                               @code
    *                               bool func(MoObject* ptr);
    *                               @endcode
    *          @return @li pointer to object
    *                  @li @c nullptr if no object if found
    *
    *          @par Usage example
    *               @code{.cpp}
    *               // Pre c++17 syntax
    *               auto pRunningEthernet = ValueMo::find_if<MoEthernet>([](MoEthernet* pEtehrnet) // <-- note MoEthernet duplication
    *               {
    *                     return pEthernet->isAdminUp();
    *               });
    *
                    // Post c++17 syntax
    *               auto pRunningEthernet = ValueMo::find_if<MoEthernet>([](auto* pEtehrnet) // <-- note MoEthernet duplication removed
    *               {
    *                     return pEthernet->isAdminUp();
    *               });
    *               @endcode
    *
    *          @see std::find_if
    *          @see find_if_not
    */
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

    /**        @brief Find first Mo-derived object for which func returns @c false.
    *
    *          Execute func for each Mo object of given type while func returns @c true.
    *          Return pointer to first object that func returned @c false for. Before executing checks that object exists (pointer is not null).
    *          If no object is found, returns @c nullptr. When func is called, it is guaranteed that pointer is not null.
    *
    *          @tparam    MO    Specifies type of Mo object func should be called for
    *          @param[in] func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument and returns @c bool:
    *                               @code
    *                               bool func(MoObject* ptr);
    *                               @endcode
    *          @return @li pointer to object
    *                  @li @c nullptr if no object if found
    *
    *          @par Usage example
    *               @code{.cpp}
    *               // Pre c++17 syntax
    *               auto pStoppedEthernet = ValueMo::find_if_not<MoEthernet>([](MoEthernet* pEtehrnet) // <-- note MoEthernet duplication
    *               {
    *                     return pEthernet->isAdminUp();
    *               });
    *
                    // Post c++17 syntax
    *               auto pStoppedEthernet = ValueMo::find_if_not<MoEthernet>([](auto* pEtehrnet) // <-- note MoEthernet duplication removed
    *               {
    *                     return pEthernet->isAdminUp();
    *               });
    *               @endcode
    *
    *          @see std::find_if_not
    *          @see find_if
    */
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

    /**
     *      @brief Checks if at least one Mo-derived object returns @c true for func.
     *
     *      Execute func for each object of given type until func returns @c true. When func is called, it is guaranteed that pointer is not null.
     *
     *      Truth table for different functions behaviour:
     *      function | all true,\n none false | some true,\n some false | none true,\n all false
     *      -------- | :--------------------: | :---------------------: | :--------------------:
     *      all_of   |    true                |    false                |    true
     *      any_of   |    true                |    true                 |     false
     *      none_of  |    false               |    false                |     true
     *
     *      @tparam      MO    Specifies type of Mo object func should be called for
     *      @param[in]   func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument and returns @c bool:
     *                               @code
     *                               bool func(MoObject* ptr);
     *                               @endcode
     *       @return   @li @c true if at least one object returns @c true for func,
     *                @li @c false otherwise
     *
     *       @par Usage example
     *           @code{.cpp}
     *           bool localHostDefined = ValueMo::any_of<MoEthernet>([](MoEthernet* pEthernet)
     *           {
     *               return pEthernet->ipAddress().str() == "127.0.0.1";
     *           });
     *           @endcode
     *
     *      @see std::any_of
     *      @see all_of
     *      @see none_of
     */
    template<class MO>
    static bool any_of (std::function<bool(MO*)> func)
    {
        return ValueMo::find_if<MO>(func) != nullptr;
    }

    /**
     *      @brief Checks if all Mo-derived objects return @c false for func.
     *
     *      Execute func for each Mo object of given type until func returns @c true. When func is called, it is guaranteed that pointer is not null.
     *
     *      Truth table for different functions behaviour:
     *      function | all true,\n none false | some true,\n some false | none true,\n all false
     *      -------- | :--------------------: | :---------------------: | :--------------------:
     *      all_of   |    true                |    false                |    true
     *      any_of   |    true                |    true                 |     false
     *      none_of  |    false               |    false                |     true
     *
     *      @tparam      MO    Specifies type of Mo object func should be called for
     *      @param[in]   func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument and returns @c bool:
     *                               @code
     *                               bool func(MoObject* ptr);
     *                               @endcode
     *           @return   @li @c true if all objects return @c false for func,
     *                 @li @c false otherwise
     *
     *       @par Usage example
     *       @code{.cpp}
     *       bool allLldpEnabled = ValueMo::none_of<MoLldp>([](MoLldp* pLldp)
     *       {
     *           return pLldp->adminStatus() == LLDP_ADMIN_DISABLE;
     *           });
     *       @endcode
     *
     *      @see std::none_of
     *      @see any_of
     *      @see all_of
     */
    template<class MO>
    static bool none_of (std::function<bool(MO*)> func)
    {
        return ValueMo::find_if<MO>(func) == nullptr;
    }

    /**
     *      @brief Checks if all Mo-derived objects return @c true for func.
     *
     *      Execute func for each Mo object of given type until func returns @c false. When func is called, it is guaranteed that pointer is not null.
     *
     *      Truth table for different functions behaviour:
     *          function | all true,\n none false | some true,\n some false | none true,\n all false
     *      -------- | :--------------------: | :---------------------: | :--------------------:
     *      all_of   |    true                |    false                |    true
     *      any_of   |    true                |    true                 |     false
     *      none_of  |    false               |    false                |     true
     *
     *      @tparam      MO    Specifies type of Mo object func should be called for
     *      @param[in]   func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument and returns @c bool:
     *                               @code
     *                               bool func(MoObject* ptr);
     *                               @endcode
     *           @return   @li @c true if all objects return @c true for func,
     *                 @li @c false otherwise
     *
     *       @par Usage example
     *       @code{.cpp}
     *       bool allIpAreStatic = ValueMo::all_of<MoIp>([](MoIp* pIp)
     *       {
     *           return pIp->isStatic();
     *       });
     *       @endcode
     *
     *      @see std::all_of
     *      @see any_of
     *      @see none_of
     */
    template<class MO>
    static bool all_of (std::function<bool(MO*)> func)
    {
        return ValueMo::find_if_not<MO>(func) == nullptr;
    }

    /**
     *      @brief Count number for Mo-derived objects that return @c true for func.
     *
     *      Execute func for each Mo object of given type and count number of those return @c true. When func is called, it is guaranteed that pointer is not null.
     *
     *      @tparam      MO    Specifies type of Mo object func should be called for
     *      @param[in]   func  Functional object (function, functor, lambda) that accepts Mo-derived object pointer as argument and returns @c bool:
     *                               @code
     *                               bool func(MoObject* ptr);
     *                               @endcode
     *       @return   number of objects
     *
     *       @par Usage example
     *       @code{.cpp}
     *       uint ipv6Count = ValueMo::count_if<MoIpV6>([](MoIpV6* pIp)
     *       {
     *           return pIp->ip() != IpV6Address::ZERO;
     *       });
     *       @endcode
     *
     *      @see std::count_if
     */
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
