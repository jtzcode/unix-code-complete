#include <sys/stat.h>
#include <fcntl.h>
#include "../tlpi-book/lib/tlpi_hdr.h"
#include <ctype.h>
/* Compile command: 
gcc seek_io.c ../tlpi-book/lib/error_functions.c ../tlpi-book/lib/get_num.c -o seek-io 
*/
int main(int argc, char *argv[]) {
    size_t len;
    off_t offset;
    int fd, ap, j;
    char *buf;
    ssize_t numRead, numWritten;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
    }

    fd = open(argv[1], O_RDWR | O_CREAT,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH /* rw-rw-rw- */
    );

    if (fd == -1) {
        errExit("open error");
    }

    for (ap = 2; ap < argc; ap++) {
        switch(argv[ap][0]) {
            case 'r': /* Display at current offset as text */
            case 'R': /* Display at current offset as HEX */
                len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
                buf = malloc(len);
                if (buf == NULL) {
                    errExit("malloc error");
                }
                numRead = read(fd, buf, len);
                if (numRead == -1) {
                    errExit("read error");
                }
                if (numRead == 0) {
                    printf("%s: end-of-file\n", argv[ap]);
                } else {
                    printf("%s: ", argv[ap]);
                    for (j = 0; j < numRead; j++) {
                        if (argv[ap][0] == 'r') {
                            printf("%c", isprint((unsigned char)buf[j]) ? buf[j] : '?');
                        } else {
                            printf("%02x", (unsigned char)buf[j]);
                        }
                    }
                    printf("\n");
                }
                free(buf);
                break;
            /* Write at current offset */
            case 'w':
                numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
                if (numWritten == -1) {
                    errExit("write error");
                }
                printf("%s:  wrote %ld bytes\n", argv[ap], (long)numWritten);
                break;
            case 's': /* Change file offset */
                offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
                if (lseek(fd, offset, SEEK_SET) == -1) {
                    errExit("lseek error");
                }
                printf("%s: seek succeeded\n", argv[ap]);
                break;
            default:
                cmdLineErr("Argument must start with [rRws]: %s\n", argv[ap]);
        }
    }
    exit(EXIT_SUCCESS);
}