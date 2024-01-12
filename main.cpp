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

    // main_ca.cpp: case CadMsgType::CLI_REQUEST_ENTER:

    // ocli.AttachBuffer(msgP->replyPrintBuffer, CLI_REPLY_PRINT_BUFFER);

    // regular execution
    //char requestLine[32] = "show pipe-mode";
    //char requestLine[64] = "set pipe-mode eth1 admin enabled";
    //char requestLine[32] = "help set synce";
    //char requestLine[32] = "show synce";

    CliContextExe context("set aaa shared-secret ABCDE");
    //CliContextExe context("set aaa shared-secret \"AB, C\"");
    LOG("---------------");
    AdapterCli(context);
    LOG("---------------");
    LOG("RESP='%s'", ocli.AsStr());

    printf("-----\n");
    return 0;
}
