#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi-book/lib/tlpi_hdr.h"

#define MAX_READ 20
char buffer[MAX_READ + 1];

int
main(int argc, char* argv[])
{
    ssize_t numRead = read(STDIN_FILENO, buffer, MAX_READ);
    if ( numRead == -1) {
        errExit("Read error");
    }
    //buffer[MAX_READ - 1] = 'x';
    buffer[numRead] = '\0';
    //buffer[0] = 'x';
    printf("Then input data was: %s\n", buffer);
    exit(EXIT_SUCCESS);
}