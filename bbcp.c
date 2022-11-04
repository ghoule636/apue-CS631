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

/**
 * This function will accept two params. A file and a file or directory.
 * Then it will attempt to open both files and copy from the first
 * to the second. If the second is a directory the file will be created
 * with the same name in that new directory. 
 */
int main(int argc, char **argv) {

	int file1;
	int file2;
	int read_val;
	char buf[BUFF_SIZE];
	int file_permission_bits = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	bzero(buf, BUFF_SIZE);	
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s file_name destination\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	errno = 0;	
	if ((file1 = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to read %s: %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	} else if ((file2 = open(argv[2], O_WRONLY | O_CREAT, file_permission_bits)) == -1) {
		fprintf(stderr, "Unable to create %s: %s\n", argv[2], strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		while ((read_val = read(file1, buf, BUFF_SIZE)) > 0) {
			if (write(file2, buf, BUFF_SIZE) == -1) {
				fprintf(stderr, "Unable to write to %s: %s\n", argv[2], strerror(errno));
				exit(EXIT_FAILURE);
			 }
		}
		if (read_val < 0) {
			fprintf(stderr, "Unable to read from %s: %s\n", argv[1], strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	return(EXIT_SUCCESS);
 }
