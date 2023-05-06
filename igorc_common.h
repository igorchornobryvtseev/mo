#pragma once

#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <vector>

typedef int  UINT8;
typedef int UINT16;
typedef int UINT32;
typedef int UINT64;
typedef int  UCHAR;

typedef int  INT8;
typedef int INT16;
typedef int INT32;
typedef int INT64;
typedef char    CHAR;

//typedef int uint32_t;

//#define string_view string

enum class E_Status {
    Error = 0,
    Ok,
};

enum class E_AdminState {
    Disable = 2,
    Enable  = 1,
};


typedef int T_EthernetNum;
//using T_EthernetNum = int;

struct EthNum {
    T_EthernetNum id {0};
    EthNum(int n) {id=n;};

    bool operator== (EthNum second) const { return id == second.id; };

    bool IsValid ( ) const { 
        return id >= 0 && id <= 3; 
    };

    const char*   AsName( ) const {
        switch (id) {
        case 0: return "0";
        case 1: return "1";
        case 2: return "2";
        }
        return "xx";
    };

    bool          IsNone( ) const {
        return !IsValid();
    };

    static T_EthernetNum MaxEthNumPossible( ) {
        return 3;
    };

    T_EthernetNum AsEnum( ) const
    {
        return id;
    };
};

////////////////////////////////////////////////////////////

#define MAX_CLI_INPUT_LEN      1024
#define CLI_REPLY_PRINT_BUFFER /*600000*/ 2400000

#define MIN_VLAN_ID          1
#define MAX_VLAN_ID          4094
#define VLAN_ID_UNTAGGED     4095
#define VLAN_ID_ALL          4096
#define VLAN_ID_UNDEF        4097
#define VLAN_ID_NONE         4098
#define MAX_EXTENDED_VLAN_ID VLAN_ID_NONE

#define MAX_NEW_BRIDGE_ID      512
#define MAX_NUM_OF_NEW_BRIDGES 100

//////////////////////////////
#include "AutoComplete.h"
#include "BitMap.h"
#include "CaStream.h"

#include "CaSharedRegion.h"
#include "CliContext.h"
#include "CliMo.h"
#include "Database.h"
#include "Dictionary.h"
#include "IpAddress.h"
#include "MoEthernet.h"
#include "Mo.h"
#include "MoPipeMode.h"
#include "MoType.h"
#include "MyArray.h"
#include "MyString.h"
#include "PlaceBuffer.h"
#include "RetStatus.h"
#include "Value.h"

template< class C >
size_t std_size( const C& c )// -> decltype(c.size());
{
    return c.size();
}
