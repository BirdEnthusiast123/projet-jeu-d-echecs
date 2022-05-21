#include <stdio.h>
#include <stdlib.h>

#include <string.h>


__declspec( dllexport ) int square(int x)
{
    return x*x;
}

int main(int argc, char **argv)
{
    printf("hey");
    return 0;
}