//***************************************************************
// File:	 MyArray.h
// Copyright: Siklu Communications , 2010
// SVN: $Revision$
// SVN: $Author$
// SVN: $Date$
// ***************************************************************

#ifndef __sk_MyArray_h__
#define __sk_MyArray_h__

// #include "MyOid.h"
#include "PlaceBuffer.h"

////////////////////////////////////////////////////

template<typename APP_ENTRY>
struct MyArray {
    struct __ENTRY {
        APP_ENTRY _appEntry;
        bool      _markToDelete {false};

        static int Compare (const void* firstP, const void* secondP) { return ((__ENTRY*)firstP)->_appEntry.Compare(&((__ENTRY*)secondP)->_appEntry); }
    };

    ~MyArray( )
    {
        if ( _startP != nullptr )
            delete[] _startP;
    }

    __ENTRY* GetP (int index) { return &_startP[index]; }

    APP_ENTRY& operator[] (int index) { return _startP[index]._appEntry; }

    int MaxNumOf ( ) { return _maxNumOf; }

    int NumOf ( ) { return _numOf; }

    void Push (APP_ENTRY& appEntry)
    {
        _sortRequest = true;
        if ( _numOf >= _maxNumOf ) {
            int newMaxNumOf = (_maxNumOf == 0) ? 1 : _maxNumOf * 2;
            __Realloc(newMaxNumOf);
        }
        __ENTRY newEntry;
        newEntry._appEntry = appEntry;
        _startP[_numOf++]  = newEntry;
    }

    /*
        void  Add( ENTRY & entry)
        {
            AddToEnd( entry);
            Sort();
        }
    */
    void RequestDelete (int index)
    {
        GetP(index)->_markToDelete = true;
        _compressRequest           = true;
    }

    /*
        void Delete( int index)
        {
            RequestDelete( index);
            Compress();
        }
    */
    void DeleteAll ( )
    {
        for ( int i = 0; i < _numOf; i++ )
            RequestDelete(i);
        Compress( );
    }

    int      _numOf {0};
    int      _maxNumOf {0};
    __ENTRY* _startP {nullptr};

    void Sort ( )
    {
        if ( _sortRequest && (_numOf > 1) )
            qsort(_startP, _numOf, sizeof(__ENTRY), __ENTRY::Compare);
        _sortRequest = false;
    }

    void Compress ( )
    {
        if ( _compressRequest ) {
            //	static MyTimer timerCompress(false);
            //	timerCompress.Continue();

            int i_src, i_dst;
            for ( i_src = 0, i_dst = 0; i_src < _numOf; i_src++ ) {
                if ( false == GetP(i_src)->_markToDelete ) {
                    if ( i_src != i_dst )
                        _startP[i_dst] = _startP[i_src];
                    i_dst++;
                }
            }
            _numOf = i_dst;

            // realloc if need
            int newMaxNumOf = _maxNumOf;
            while ( _numOf < (newMaxNumOf / 2) )
                newMaxNumOf = newMaxNumOf / 2;
            if ( newMaxNumOf != _maxNumOf )
                __Realloc(newMaxNumOf);

            //	timerCompress.Stop();
            //		ocli << "\n" << "instances=" << _numOfInstances << ",  timerCompress=" << timerCompress.GetMsec() << "\n";
        }
        _compressRequest = false;
    }

private:
    bool _sortRequest {false};
    bool _compressRequest {false};

    void __Realloc (int newMaxNumOf)
    {
        PlaceGlobal place(__PRETTY_FUNCTION__);
        __ENTRY*    newArrayP = new (place) __ENTRY[newMaxNumOf];
        memcpy(newArrayP, _startP, _numOf * sizeof(__ENTRY));
        if ( _startP != nullptr )
            delete[] _startP;
        _startP   = newArrayP;
        _maxNumOf = newMaxNumOf;
    }
};

//////////////////////////////////////////////////////////////

#endif /* __sk_MyArray_h__ */
