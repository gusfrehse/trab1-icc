#include <stdio.h>
#include <likwid.h>

int main(int argc, char const *argv[])
{
    LIKWID_MARKER_INIT;

    LIKWID_MARKER_START("foo");
    printf("Hello World.\n");
    LIKWID_MARKER_STOP("foo");

    LIKWID_MARKER_CLOSE;
    return 0;
}
