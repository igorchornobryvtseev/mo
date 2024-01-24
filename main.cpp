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
    CliContextExe setContext("set aaa shared-secret \"ABC DE\" mode local");
    //CliContextExe setContext("set aaa shared-secret ABC mode local");
    AdapterCli(setContext);
    LOG("RESP='%s'", ocli.AsStr());
    LOG("---------------");

    CliContextExe showContext("show aaa");
    AdapterCli(showContext);
    LOG("RESP='%s'", ocli.AsStr());
    LOG("---------------");

    return 0;
}
