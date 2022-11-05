#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef BUFF_SIZE
#define BUFF_SIZE 32768
#endif

static void usage();

/**
 * This opens a file for reading and returns the file descriptor.
 */
int
openFileForReading(char* path)
{
	int fd;
	errno = 0;
	if ((fd = open(path, O_RDONLY | O_REGULAR)) == -1) {
		fprintf(stderr, "Unable to read %s: %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}	
	return fd;
}

/**
 * This will open the target file or create it.
 *
 */
int
openFileForWriting(char* path)
{
	int fd;
	int permission_bits = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	errno = 0;
	
	if ((fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, permission_bits)) == -1) {
		fprintf(stderr, "Unable to create file %s: %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}	
	return fd;
}

/**
 * Determines if the target path is a directory and if so appends
 * the name of the source file to it's path.
 */
char*
getWritePath(char* path, char* source_file)
{
	errno = 0;
	if (open(path, O_WRONLY | O_CREAT) == -1) {
		if (errno == EISDIR) {
			strcat(path, "/");
			strcat(path, basename(source_file));
		} else {
			fprintf(stderr, "Unable to open file %s: %s\n", path, strerror(errno));
			exit(EXIT_FAILURE);
		}

	}
	return path;
}

/**
 * This will read data from a file descriptor and write it back out
 * to another file descriptor.
 */
void 
copyData(int fd_source, int fd_dest) 
{
	char buf[BUFF_SIZE];	
	int read_val;
	bzero(buf, BUFF_SIZE);
	while ((read_val = read(fd_source, buf, BUFF_SIZE)) > 0) {
		if (write(fd_dest, buf, read_val) == -1) {
			fprintf(stderr, "Unable to write: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	if (read_val < 0) {
		fprintf(stderr, "Unable to read: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/**
 * This function will accept two params. A file and a file or directory.
 * Then it will attempt to open both files and copy from the first
 * to the second. If the second is a directory the file will be created
 * with the same name in that directory. Directory must exist prior to call.
 */
int 
main(int argc, char **argv) 
{
	int file1, file2;
	struct stat f1_stat, f2_stat;

	setprogname(argv[0]);
	
	if (argc != 3) {
		usage();
	}

	getWritePath(argv[2], argv[1]);

	/* Check if the files are actually the same. */
	stat(argv[1], &f1_stat);
	stat(argv[2], &f2_stat);
	if (f1_stat.st_ino == f2_stat.st_ino) {
		fprintf(stderr, "The files are the same!\n");
		exit(EXIT_FAILURE);
	}		
	chmod(argv[2], f1_stat.st_mode);	

	file1 = openFileForReading(argv[1]);	
	file2 = openFileForWriting(argv[2]);
	
	copyData(file1, file2);
	
	close(file1);
	close(file2);
	return(EXIT_SUCCESS);
}

static void
usage(void) {
	fprintf(stderr, "usage: %s source destination\n", getprogname());
	exit(EXIT_FAILURE);
}
