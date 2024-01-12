#include "igorc_common.h"
#include "Database.h"
#include "Mo.h"

////////////////////////////////////////////////////

static Mo*  sampleMoTable[MO_MAX];
static bool _visibleMo[MO_MAX] = {true};

void setVisibleMo (T_MoType moType, bool visible)
{
    _visibleMo[moType] = visible;
}

extern bool IsDebugMode( );

bool IsBistMode( );

bool isVisibleMo (T_MoType moType, bool ignoreDebug)
{
    if ( IsBistMode( ) )
        return true;
    if ( !ignoreDebug && IsDebugMode( ) )
        return true;
    return _visibleMo[moType];
}

Mo* SampleMo (T_MoType moType)
{
    return sampleMoTable[moType];
}

Mo* SampleMo (const char* name)
{
    for ( int i = 0; i < MO_MAX; i++ ) {
        if ( sampleMoTable[i] && strcmp(name, sampleMoTable[i]->TypeName( )) == 0 )
            return sampleMoTable[i];
    }
    return nullptr;
}

void InitSampleMo ( )
{
    for ( int i = 0; i < MO_MAX; i++ ) {
        sampleMoTable[i] = Mo::BuildNew((T_MoType)i);
        if ( sampleMoTable[i] != nullptr ) {
            sampleMoTable[i]->_properties._asBits.isSample = true;
            sampleMoTable[i]->DoPerform(REGISTER_STATIC, nullptr);
        }
        _visibleMo[i] = true;
    }

    // hide some MO if needed
    //setVisibleMo(MO_DEBUG_LOG, false);
}

Database* Db ( )
{
    static Database database;
    return &database;
}

//////////////////////////////////////////////////////////////////////////////

//static MyTimer transactionTimer;

void TransactionStart ( )
{
    //transactionTimer.Start( );
}

void TransactionEndRestore ( )
{
    //MyTimer terminationTimer;

    ValueMo::_DeleteMo(ACTION_CREATED);

    for ( UINT16 index = 0; index < MAX_NUM_MOS; index++ ) {
        Mo* moP = Db( )->MoByMainIndex(index);
        if ( moP != nullptr )
            moP->TransactionEndRestore( );
    }

    // if ( GetDebugFlag(DEBUG_FLAG_TRANSACTION) )
    //     ocli << "\n"
    //          << "TransactionRestore"
    //          << "  total time=" << transactionTimer << " msec, termination time=" << terminationTimer << " msec, cli size=" << ocli.GetLen( ) << "\n";
}

void AllDownloadConfig(bool isDefaultDatabase);

void TransactionEnd (TransactionEndType endType)
{
    LOG("entry");
    //MyTimer terminationTimer;

    BitMapT<0, MO_MAX> touchedTypes;

    for ( int index = 0; index < MAX_NUM_MOS; index++ ) {
        Mo* moP = Db( )->MoByMainIndex(index);
        if ( moP != nullptr ) {
            if ( moP->_transaction._asBits.created && moP->_transaction._asBits.deleted )
                continue;  // created and immediate deleted, do nothing

            T_MoActionStatus actionStatus = moP->Action( );

            if ( endType == TRANSACTION_END_CONFIG_DEFAULT )
                moP->_properties._asBits.isDefault = true;
            else if ( actionStatus != ACTION_NONE )
                moP->_properties._asBits.isDefault = false;

            if ( actionStatus != ACTION_NONE ) {
                moP->DoPostConfig(actionStatus);

                T_MoType moType   = moP->Type( );
                LOG("index:%d moType:%d", index, moType);
                touchedTypes.Set(moP->Type( ), true);
            }
        }
    }

    if ( endType == TRANSACTION_END_CONFIG_DEFAULT ) {
        AllDownloadConfig(true);
    } else if ( endType == TRANSACTION_END_CONFIG ) {
        //MoSystem* systemP = Mo::FindMoT<MoSystem>( );

        bool configCallback = false, configSaved = false;
        ValueMo::for_every([&configSaved, &configCallback] (Mo* moP) {
            if ( configSaved )
                return;  // works like continue here
            if ( moP->Action( ) != ACTION_NONE ) {
                // if a settable attribute was modified, update _valueConfigSaved
                for ( auto it = moP->BeginAttr( ); it.IsValid( ); it++ ) {
                    if ( it->IsSettable( ) && it->ValueP(moP)->IsDataChanged( ) ) {
                        configSaved = true;
                        //MoSystem::_configChangeCounter++;
                        break;
                    }
                }
                configCallback = true;
            }
        });

        // if ( configSaved )
        //     systemP->_valueConfigSaved.SetUint32(E_AGENT_FALSE);
        if ( configCallback )
            AllDownloadConfig(false);
    }

    ValueMo::_DeleteMo(ACTION_DELETED);

    for ( auto it = touchedTypes.Begin( ); it.IsValid( ); it++ ) {
        T_MoType moType = (T_MoType)(*it);
        LOG("igorc moType:%d", moType);

        Mo* moP =  SampleMo( moType);
        LOG("moP=%p", moP);

        SampleMo((T_MoType)(*it))->DoSamplePostConfig( );
    }
    //return;

    // Transaction Cleanup
    for ( UINT16 index = 0; index < MAX_NUM_MOS; index++ ) {
        Mo* moP = Db( )->MoByMainIndex(index);
        if ( moP != nullptr )
            moP->TransactionEndConfig( );
    }

    // Transaction Log
    // if ( GetDebugFlag(DEBUG_FLAG_TRANSACTION) ) {
    //     ocli << "\n" << ((endType == TRANSACTION_END_CONFIG) ? "TransactionConfiguring" : "TransactionNormal");
    //     ocli << "  total time=" << transactionTimer << " msec, termination time=" << terminationTimer << " msec, cli size=" << ocli.GetLen( ) << "\n";
    // }
    LOG("exit");
}

//////////////////////////////////////////////////////////////////////////////

UINT32    ValueMo::_numOfSorted = 0;
MainIndex ValueMo::_sortedTable[MAX_NUM_MOS];

void ValueMo::_AddMo(Mo* newMoP)
{
    LOG("entry");
    Database* dbP = Db( );

    if ( ValueMo::_numOfSorted >= sizeof(_sortedTable) / sizeof(_sortedTable[0]) ) {
        ocli << "LookUpTable::EntryInsert() - no place in lookup table (" << newMoP << ")\n";
        LOG("error - no place");
        return;
    }

    MainIndex mainIndex;
    for ( mainIndex = 0; mainIndex < MAX_NUM_MOS; mainIndex++ ) {
        if ( dbP->_mainTable[mainIndex] == nullptr )
            break;
    }
    if ( mainIndex >= MAX_NUM_MOS ) {
        ocli << "LookUpTable::AddMoHard() - no place for managed object " << newMoP << "\n";
        LOG("error - no place");
        return;
    }

    dbP->_mainTable[mainIndex] = newMoP;
    newMoP->_mainIndex         = mainIndex;

    newMoP->DoInsertToDb( );

    T_MoType moType   = newMoP->Type( );
    MoId     moNumber = newMoP->GetNumber( );
    LOG("moType:%d moNumber:%d", moType, moNumber);

    int mid, low = 0, high = ValueMo::_numOfSorted - 1;
    while ( low <= high ) {
        mid = (low + high) / 2;

        Mo* moP = dbP->MoByMainIndex(ValueMo::_sortedTable[mid]);

        int compareResult;

        if ( moP->Type( ) != moType )
            compareResult = (moP->Type( ) > moType) ? 1 : -1;
        else if ( moP->GetNumber( ) == moNumber )
            compareResult = 0;
        else
            compareResult = (moP->GetNumber( ) > moNumber) ? 1 : -1;

        if ( compareResult == 0 ) {
            // found
            ocli << "Database::AddMo() - object already exists " << newMoP << "\n";
            LOG("error - already exist");
            return;
        }
        if ( compareResult > 0 )
            high = mid - 1;
        else
            low = mid + 1;
    }
    UINT32 sortedIndexToAdd = low;

    // found place to insert, insert new
    memmove(&ValueMo::_sortedTable[sortedIndexToAdd + 1], &ValueMo::_sortedTable[sortedIndexToAdd], (ValueMo::_numOfSorted - sortedIndexToAdd) * sizeof(MainIndex));
    ValueMo::_sortedTable[sortedIndexToAdd] = mainIndex;
    ValueMo::_numOfSorted++;
    LOG("exit mainIndex=%d sortedIndexToAdd=%d _numOfSorted=%d", mainIndex, sortedIndexToAdd, _numOfSorted);
}

void ValueMo::_DeleteMo(T_MoActionStatus action)
{
    LOG("entry");
    if ( action == ACTION_NONE ) {
        for ( UINT16 index = 0; index < MAX_NUM_MOS; index++ ) {
            Mo* moP = Db( )->MoByMainIndex(index);
            if ( moP && moP->_transaction._asBits.forced )
                moP->DoSnmpPerform(DELETE_SNMP_INSTANCE);
        }
    }

    Database* dbP = Db( );

    MainIndex *srcP, *dstP;
    srcP = dstP = _sortedTable;

    int i = ValueMo::_numOfSorted;
    while ( i-- ) {
        bool needToDelete = false;

        MainIndex mainIndex = *srcP;

        Mo* moP = dbP->MoByMainIndex(mainIndex);

        if ( action == ACTION_NONE )
            needToDelete = moP->_transaction._asBits.forced;
        else
            needToDelete = (action == moP->Action( ));

        if ( needToDelete ) {
            moP->DoRemoveFromDb( );
            delete moP;
            dbP->_mainTable[mainIndex] = nullptr;
        } else
            *(dstP++) = mainIndex;

        srcP++;
    }
    ValueMo::_numOfSorted -= srcP - dstP;
    LOG("exit");
}

Mo* ValueMo::MoP( )
{
    return IsValid( ) ? Db( )->MoByMainIndex(ValueMo::_sortedTable[_sortedIndex]) : nullptr;
}

Mo* ValueMo::_FindMo(T_MoType moType, MoId moNumber)
{
    LOG("entry");
    if ( moNumber.AsNumber( ) == 0 ) {
        ValueMo valueMo(moType);
        valueMo.SetFirst( );
        LOG("exit");
        return valueMo.MoP( );
    }

    Database* dbP = Db( );

    int mid, low = 0, high = ValueMo::_numOfSorted - 1;

    while ( low <= high ) {
        mid = (low + high) / 2;

        Mo* moP = dbP->MoByMainIndex(ValueMo::_sortedTable[mid]);

        int compareResult;

        if ( moP->Type( ) != moType )
            compareResult = (moP->Type( ) > moType) ? 1 : -1;
        else if ( moP->GetNumber( ) == moNumber )
            compareResult = 0;
        else
            compareResult = (moP->GetNumber( ) > moNumber) ? 1 : -1;

        if ( compareResult == 0 ) {
            // found
            UINT32    sortedIndex = (UINT32)mid;
            MainIndex mainIndex   = ValueMo::_sortedTable[sortedIndex];
            LOG("exit mainIndex=%d", mainIndex);
            return dbP->MoByMainIndex(mainIndex);
        }
        if ( compareResult > 0 )
            high = mid - 1;
        else
            low = mid + 1;
    }

    LOG("exit - null");
    return nullptr;
}

bool ValueMo::IsMatch( )
{
    return (_moType == MO_MAX) || (MoP( )->Type( ) == _moType);
}

//////////////////////

#include "CliMo.h"

bool ValueMoMatchCli::IsValid( )
{
    if ( !ValueMo::IsValid( ) )
        return false;
    return !cliP->IsError( );
}

bool ValueMoMatchCli::IsMatch( )
{
    if ( !ValueMo::IsMatch( ) )
        return false;

    for ( int i = 0; i < Mo::_numOfReadersId; i++ ) {
        if ( !Mo::_readersId[i]->IsMatch(MoP( )) )
            return false;
    }
    return true;
}

// ***************************************************************
