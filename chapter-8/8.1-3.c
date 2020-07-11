/*
 * cat -- UNIX syscalls version
 */
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define STDIN		0
#define STDOUT		1
#define STDERR		2

#define BUFSIZ		1024	/* should be divisor of FS block size */

void cat(int fd)
{
	char buf[BUFSIZ];
	int bc;

	while ((bc = read(fd, buf, BUFSIZ)) > 0)
		if (write(STDOUT, buf, bc) != bc) {
			write(STDERR, "cat err\n", 8);
			exit(1);
		}
}

int main(int argc, char *argv[])
{
	int fd;

	if (argc == 1)
		cat(STDIN);
	else
		while (--argc && *++argv) {
			fd = open(*argv, O_RDONLY, 0);
			if (fd == -1) {
				write(STDERR, "error opening ", 13);
				write(STDERR, *argv, strlen(*argv));
				exit(2);
			}
			cat(fd);
			close(fd);
		}
	exit(0);
}

	
