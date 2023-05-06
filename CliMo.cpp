#include "igorc_common.h"

#include "CliMo.h"

//#include "HistoryBucket.h"

////////////////////////////////////////////////

void AttrArray::Remove(AttrId attrId)
{
    UINT32 i;

    for ( i = 0; (i < _numOf) && (_array[i]._attrId != attrId); i++ )
        ;

    if ( i >= _numOf )
        return;

    _numOf--;
    for ( ; i < _numOf; i++ )
        _array[i] = _array[i + 1];
}

///////////////////////////////////////////////////////////////////

MyOutStream& operator<< (MyOutStream& dstR, WIDTH width)
{
    MyStringT<64> text;
    text << width._valueP;

    char format[16];
    sprintf(format, "%%-%ds", width._width);

    dstR.Print(format, text.AsStr( ));

    return dstR;
}

///////////////////////////////////////////////////////////////////

CliModifierShow::CliModifierShow(T_MoType moType) : _info(moType)
{
    Mo* sampleP = SampleMo(moType);

    for ( Mo::IteratorAttr it = sampleP->BeginAttr( ); it.IsValid( ); it++ ) {
        if ( it->IsInfo( ) ) {
            _dictionary.Add(MODIF_INFO, "info");
            break;
        }
    }

    // if ( sampleP->GetCountersLine( ) )
    //     _dictionary.Add(MODIF_STATS, "statistics");
    // if ( sampleP->GetCountersLine( ) && sampleP->GetCountersLine( )->HasStatsSummary( ) )
    //     _dictionary.Add(MODIF_STATS_SUMMARY, "statistics-summary");

    for ( Mo::IteratorAttr it = sampleP->BeginAttr( ); it.IsValid( ); it++ ) {
        if ( it->IsInfo( ) ) {
            AttrId attrId = it->__Id( );
            _dictionary.Add(it->GetDictEntry( ));
            _info.Add(attrId);
        }
    }
}

void CliModifierShow::DoSetCli(WordReader* inP)
{
    if ( inP->IsEnter( ) )
        SetString("info");
    else {
        ValueDictInternalT<80>::DoSetCli(inP);
        if ( inP->CheckExit( ) )
            return;
        if ( IsStats( ) || IsStatsSummary( ) ) {
            if ( IsStatsSummary( ) ) {
                // in statistics summary, if users didn't enter any thing, run the full range.
                _minHistory   = 0;
                _maxHistory   = 95;
                _historyGiven = true;
            } else {
                _minHistory = _maxHistory = 0;
                _historyGiven             = false;
            }

            if ( inP->IsEnter( ) )
                return;
            if ( !_hasHistory )
                return;

            ValueLongRangeT<0, 95> valMinRange;
            valMinRange.DoSetCli(inP);
            _minHistory = _maxHistory = valMinRange.GetUint32( );
            _historyGiven             = true;

            if ( inP->CheckExit( ) )
                return;

            if ( inP->IsEnter( ) )
                return;
            ValueLongRangeT<0, 95> valMaxRange;
            valMaxRange.DoSetCli(inP);
            if ( inP->CheckExit( ) )
                return;
            _maxHistory = valMaxRange.GetUint32( );

            if ( _maxHistory < _minHistory )
                inP->SetStatus(RetStatus {"Invalid range"});
        }
    }
}
