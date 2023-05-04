#include "igorc_common.h"
#include "Mo.h"

long mygettime ( )
{
    timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return tp.tv_sec;
}

int main()
{
	printf("-----\n");
	return 0;
}
