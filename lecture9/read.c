#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/saw"

int
main (int argc, char**argv)
{
    int fd = open (DEVICE, O_RDONLY);
    if (fd < 0) {
        fprintf (stderr, "Cannot open device.\n");
        exit (1);
    }

    struct timespec delay = {0, 999000000};
	
if (argc == 2) {
	delay.tv_nsec=atoi(argv[1]);
}

    char c;
    while (1) {
        read (fd, &c, 1);

        fprintf (stdout, "%d ", c);
        fflush (stdout);

	nanosleep(&delay,NULL);
    }

    close (fd);
    return 0;
}

