#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#ifndef BUFF_SIZE
#define BUFF_SIZE 32768
#endif

int openFileForReading(char* path) {
	int fd;
	errno = 0;
	if ((fd = open(path, O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to read %s: %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}	
	return fd;
}

int openFileForWriting(char* path) {
	int fd;
	int file_permission_bits = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	errno = 0;
	if ((fd = open(path, O_WRONLY | O_CREAT, file_permission_bits)) == -1) {
		fprintf(stderr, "Unable to create %s: %s\n", path, strerror(errno));
		exit(EXIT_FAILURE);
	}
	return fd;
}

void copyData(int fd1, int fd2) {
	char buf[BUFF_SIZE];	
	int read_val;
	bzero(buf, BUFF_SIZE);
	while ((read_val = read(fd1, buf, BUFF_SIZE)) > 0) {
		if (write(fd2, buf, read_val) == -1) {
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
 * with the same name in that new directory. 
 */
int main(int argc, char **argv) {

	int file1, file2;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s file_name destination\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	file1 = openFileForReading(argv[1]);	
	file2 = openFileForWriting(argv[2]);

	copyData(file1, file2);
	
	close(file1);
	close(file2);
	return(EXIT_SUCCESS);
 }
