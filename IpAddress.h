//***************************************************************
// File: IpAddress.h
// Class/Module:IpAddress
//
// Purpose: A general purpose wrapper that represent IP address.
// Visibility: Public
//
// Description: IpAddress can be constructed in 3 ways
//      1. dotted notation - like 192.15.23.56
//      2. 32 bit value of the Ip address - like 0x0a000001
//      3. A,B,C,D numbers
//
// Examples:
//-------------------------------------------
//    // construct from the dotted string
//    IpAddress   serverAddr("192.115.45.78");
//
//    struct sockaddr_in  fromAddr;
//
//    // system call
//    recvfrom(sock,bufP,BUF_SIZE, &fromAddr ...)
//
//    // address always should be in HOST byte order
//    IpAddress fromIP(ntohl(fromAddr.sin_addr.s_addr));
//
//    // we can get IpAddress as A, B, C, D numbers
//    uint8 elemA, elemB, elemC, elemD;
//    fromIp.AsABCD(elemA, elemB, elemC, elemD);
//
//    // now we can compare 2 IpAddresses ( <,> <=, >= are also allowed for SNMP getnext)
//    if (fromIP == serverAddr){
//        Process();
//    } else {
//        IpAddress::DottedString  ipStr = fromIP.AsString();
//        smon.Print("Unknown message from %s", ipStr.itsData);
//    }
// ***************************************************************
#ifndef IPADDRESS_H
#define IPADDRESS_H

//#include <iosfwd>

//#include "gl_types.h"

class IpAddress
{
public:
    struct DottedString {
        enum maxLen {
            MAX_STRING_LEN = 16  // 255.255.255.255\0 = 3*4+3+1
        };

        operator const char* ( );

        char itsData[MAX_STRING_LEN];
    };

    enum SpecialAdresses {
        FIRST_FAKED      = 0x00000001UL,  //  0.0.0.1
        LAST_FAKED       = 0x00ffffffUL,  //  0.255.255.255
        FIRST_NOT_FAKED  = 0x01000001UL,  //  1.0.0.1
        LAST_BEFORE_LOOP = 0x7efffffeUL,  //  126.255.255.254
        FIRST_LOOP       = 0x7f000000UL,  //  127.0.0.0
        LAST_LOOP        = 0x7fffffffUL,  //  127.255.255.255
        FIRST_AFTER_LOOP = 0x80000001UL,  //  128.0.0.1
        LAST_UNICAST     = 0xdffffffeUL,  //  223.255.255.254
        FIRST_MULTICAST  = 0xe0000000UL,  //  224.0.0.0
        LAST_MULTICAST   = 0xe0ffffffUL   //  224.255.255.255
    };

    static const IpAddress ZERO;
    //----------------------------------------------------------------------
    // Constructor & Destructor
    //----------------------------------------------------------------------
    IpAddress( );

    // creates IP address by string like "192.112.24.16"
    explicit IpAddress(const char* theDotStringP);

    // creates IP address by Berkeley Internet address
    // theBerkeleyAddr should be in HOST byte order.
    explicit IpAddress(struct in_addr theBerkeleyAddr);

    // creates IP address by a 32-bit value
    // theIpValue should be in HOST byte order.
    explicit IpAddress(UINT32 theIpValue);

    // creates IP address as A,B,C,D numbers
    IpAddress(UINT8 theA, UINT8 theB, UINT8 theC, UINT8 theD);

    //----------------------------------------------------------------------
    // Get methods
    //----------------------------------------------------------------------
    // both methods return the IP address in HOST byte order !
    // return the IP address in BSD format
    struct in_addr AsBerkeley( ) const;
    UINT32         AsUINT32( ) const;

    // get IP address as A,B,C,D numbers ( output parameters )
    void AsABCD(UINT8& theA, UINT8& theB, UINT8& theC, UINT8& theD) const;

    // get in NETWORK byte-order (like htonl)
    UINT32 Host2Net( ) const;

    // returns string representation BY-VALUE
    DottedString AsString( ) const;              // return short form 0 = "0"
    DottedString AsFullString( ) const;          // return long form 0  = "000"
    DottedString AsRightAlignedString( ) const;  // returns a right-aligned string with no zeros

    //----------------------------------------------------------------------
    // Actions
    //----------------------------------------------------------------------

    // Check if string is valid IP address
    static bool IsValidDottedString(const char* theString);

    // check that address is a valid mask
    bool IsValidMask( ) const;

    UINT8 Mask2PrefixSize( ) const;

    bool IsUnicastClass( ) const;

    bool IsValidMulticast( ) const;

    bool IsUnicast4Mask(IpAddress mask) const;

    bool IsValidInterfaceAddress( ) const;

    bool IsLoopbackAddress( ) const;

    bool IsFakedPeerAddress( ) const;

    bool IsValidInterfaceMask( ) const;

    IpAddress LongestSubnetMask( ) const;

    // returns mask for class A, B or C
    IpAddress StandardClassMask( ) const;

private:
    //----------------------------------------------------------------------
    // Data members
    //----------------------------------------------------------------------
    UINT32 itsAddress;
};

//-------------------------------------
// operators
//-------------------------------------
bool operator== (const IpAddress& theLeft, const IpAddress& theRight);
bool operator!= (const IpAddress& theLeft, const IpAddress& theRight);
bool operator<(const IpAddress& theLeft, const IpAddress& theRight);
bool operator> (const IpAddress& theLeft, const IpAddress& theRight);
bool operator<= (const IpAddress& theLeft, const IpAddress& theRight);
bool operator>= (const IpAddress& theLeft, const IpAddress& theRight);

std::ostream& operator<< (std::ostream& stream, const IpAddress& addr);

#endif  // IPADDRESS_H
