#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define DEVICE "/dev/blackboard"

int
main (void)
{
    int fd = open (DEVICE, O_WRONLY);
    if (fd < 0) {
        fprintf (stderr, "Cannot open device.\n");
        exit (1);
    }
    
  
/*    write (fd, word, strlen(word)); */

    int i;
    char c;
    while (1) {
        for (i = 0; i <= 9; i++) {
            c = '0' + i;
            write (fd, "word", 4);
            write (fd, &c, 1);
            write (fd, " ", 1);
        }
    }

    close (fd);

    return 0;
}


