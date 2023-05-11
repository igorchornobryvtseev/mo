#ifndef AUTOCOMPLETE_H_INCLUDED
#define AUTOCOMPLETE_H_INCLUDED

#include <stddef.h>

size_t memlcp(const char* s1, const char* s2, size_t len);

struct AutoComplete {
    enum {
        SINGLE_STEP = -1,
    };

    AutoComplete(char* buf, size_t buf_size, const char* prefix, int multi_step_separator = SINGLE_STEP) { Reset(buf, buf_size, prefix, multi_step_separator); }

    void Reset(char* buf, size_t buf_size, const char* prefix, int multi_step_separator = SINGLE_STEP);
    void Update(const char* complete_option);

    bool WasMatch ( ) const { return !_isFirst; }

    bool WasFullMatch ( ) const { return _fullMatch; }

    bool WasSingleStep ( ) const { return _multiStepSeparator == SINGLE_STEP; }

    bool WasMultiStep ( ) const { return !WasSingleStep( ); }

    size_t GetPostfixLen ( ) const { return _postfixLen; }

private:
    char*       _postfix;
    size_t      _postfixSize;
    size_t      _postfixLen;
    const char* _prefix;
    size_t      _prefixLen;
    int         _multiStepSeparator;
    bool        _isFirst;
    bool        _fullMatch;
};

#endif  // #ifndef AUTOCOMPLETE_H_INCLUDED
