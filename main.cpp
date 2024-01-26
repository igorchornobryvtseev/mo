#include "igorc_common.h"
#include "Mo.h"

void log(const char* fname, int fline, const char* format, ...)
{
    printf("[%s:%d] ", fname, fline);

    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);

    if (format[strlen(format) - 1] != '\n')
        printf("\n");
}

long mygettime ( )
{
    timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return tp.tv_sec;
}

std::string newString(const char c1, const char c2)
{
    std::string res {c1};
    res += c2;
    return res;
}

ssize_t parseQuoteStr(const std::string& inStr, ssize_t pos, std::string& outStr)
{
    enum E_State {
        initial,
        insideContent,
        insideQuoteContent,
        escapeFound,
    };
    
    E_State state = E_State::initial;
    char    quoteChar = '"';
    
    for (; pos < inStr.size(); ++pos ) {
        const char c = inStr[pos];
        
        switch (state) {
            
        case E_State::initial:
            if (std::isspace(c))
                break;
            
            if (c == '"' || c == '\'') {
                state = E_State::insideQuoteContent;
                quoteChar = c;
                break;
            }            
            
            state = E_State::insideContent;            
            outStr += c;
            break;
            
        case E_State::insideContent:
            if (std::isspace(c))
                return pos;
            outStr += c;
            break;
            
        case E_State::insideQuoteContent:
            if (c == '\\') {
                state = E_State::escapeFound;
                break;
            }
            
            if (c == quoteChar)
                return ++pos;
            
            outStr += c;
            break;
            
        case E_State::escapeFound:
            state = E_State::insideQuoteContent;
            
            switch (c) {
                
            case '"':
            case '\'':
                outStr += quoteChar == c ? std::string {c} : newString('\\', c);
                break;
                
            case '\\':
                outStr += c;
                break;
                
            default:
                outStr += newString('\\', c);
            }            
            break;
        }
    }
    
    if (state == E_State::insideContent)
        return pos;
    
    return -1;    
}


CliOutStream ocli;

void AllDownloadConfig (bool isDefaultDatabase)
{
    MoPipeMode::DownloadConfig();
}

void AdapterCli(CliContext& contextP);

int main()
{
    InitSampleMo( );
    LOG("---------------");

    //MoPipeMode::CreateAll( );
    //MoSyncE::Create( );
    MoAaa::Create( );
    LOG("---------------");

    // regular execution
    // LOG("---------------");
    // CliContextExe context("show pipe-mode");
    // CliContextExe context("set pipe-mode eth1 admin enabled");
    // CliContextExe context("help set synce");
    // CliContextExe context("show synce");
    // AdapterCli(context);
    // LOG("RESP='%s'", ocli.AsStr());
    // LOG("---------------");

    LOG("---------------");
    CliContextExe setContext("set aaa shared-secret  \"AB \\\" DE\"  mode tacacs");
    //CliContextExe setContext("set aaa shared-secret ABC");
    AdapterCli(setContext);
    LOG("RESP='%s'", ocli.AsStr());
    LOG("---------------");

    CliContextExe showContext("show aaa");
    AdapterCli(showContext);
    LOG("RESP='%s'", ocli.AsStr());
    LOG("---------------");

    return 0;
}
