#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi-book/lib/tlpi_hdr.h"

int main(int argc, char* argv[]) {
    if (close(STDIN_FILENO) == -1) {
        errExit("error closing stdin");
    }
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH; /* rw-rw-rw- */
    // Should specify mode parameter if file not exists. 
    // Or the new file will have a random permission on stack.
    int fd = open("test.txt", O_CREAT | O_RDONLY, filePerms);
    if (fd == -1) {
        errExit("open test.txt failure");
    }
    printf("File descriptor is: %d\n", fd);
    // Read from standard input: fd=0
    int numRead, BUF_SIZE = 100;
    char buf[BUF_SIZE];
    while ((numRead = read(fd, buf, BUF_SIZE)) > 0)
        printf("You have input: %s\n", buf);
    if (numRead == -1)
        errExit("Read");
    close(fd);
    exit(EXIT_SUCCESS);
}