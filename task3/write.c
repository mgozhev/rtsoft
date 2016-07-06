#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/blackboard"

int
main (void)
{
    int fd = open (DEVICE, O_WRONLY);
    if (fd < 0) {
        fprintf (stderr, "Cannot open device.\n");
        exit (1);
    }
    
    char *word = "wordn ";

    int i;
    while (1) {
        for (i = 0; i <= 9; i++) {
            word[4] = '0' + i;
            write (fd, word, 6);
        }
    }


    close (fd);

    return 0;
}


