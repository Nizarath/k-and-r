/*
 * directory walking and printing:
 - interface is system-independent;
 - real fundament is based on directory design in each system:
 	- | ind_num | file_name | in Research UNIX/System V
	- | ind_num | name_len | rec_len | name | .... | in BSD
	- others for Linux file systems

 * normally there is a dirent.h file under UNIX system according to POSIX (Windows?),
 * this is its implementation (as with stdio.h for libc before)
 */

/* libc */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* unix */
#include <fcntl.h>	/* O_RDONLY, O_WRONLY, O_RDWR */
#include <sys/types.h>	/* DIR type in inode */
#include <unistd.h>	/* read/write */
#include <dirent.h>	/* DIR, Dirent, opendir, closedir, readdir */

typedef struct dirent Dirent;

int main(int argc, char **argv)
{
	extern void fsize(const char *);

	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			fsize(*++argv);
	return 0;
}

/* 1. stat;
 * 2. if DIR -> dirwalk
 * 3. any case -> print size (dir also)
 */
void fsize(const char *name)
{
	extern void dirwalk(const char *name, void (*fcn)(const char *));
	struct stat stbuf, *st;

	if (stat(name, &stbuf) == -1) {	/* error stat */
		fprintf(stderr, "fsize: cann't access %s\n", name);
		return;
	}
	st = &stbuf;
	if ((st->st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fsize);
	printf("%ld %ld %o %d %d %ld %ld %s\n",
			st->st_ino, st->st_blksize, st->st_mode, st->st_uid,
			st->st_gid, st->st_nlink, st->st_size, name);
}

#define MAX_PATH	1024

void dirwalk(const char *dir, void (*fcn)(const char *))
{
	char name[MAX_PATH];
	Dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: cann't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0 ||
		    strcmp(dp->d_name, "..") == 0)
			continue;
		if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s %s too long\n",
					dir, dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}
