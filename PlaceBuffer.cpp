#include "igorc_common.h"
#include "PlaceBuffer.h"

// #include "MyString.h"
// #include "siklu_fmt.h"
// #include "sk_log.h"

void* operator new (size_t objSize, PlaceBuffer* placeP)
{
    void* ptr = placeP->Alloc(objSize);
    if ( ptr == nullptr ) {
        printf("fail\n");
        // std::string tmp = fmt::format("ERROR-NO-PLACE in operator new, text:\n[{}]\n", placeP->_textP);
        // fLOG_EX_ERR(false, "{}", tmp);
        // TTYOUT("!!!!!! {}", tmp);
    }
    return ptr;
}

void* operator new (size_t objSize, PlaceBuffer& placeR)
{
    return operator new (objSize, &placeR);
}

void* operator new[] (size_t objSize, PlaceBuffer& placeR)
{
    return operator new (objSize, placeR);
}
