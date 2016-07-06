#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/blackboard"

int
main (void)
{
    int fd = open (DEVICE, O_RDONLY);
    if (fd < 0) {
        fprintf (stderr, "Cannot open device.\n");
        exit (1);
    }
    
    char c;
    while (1) {
        read (fd, &c, 1);

        fprintf (stdout, "%c", c);
        fflush (stdout);
    }

    close (fd);
    return 0;
}

