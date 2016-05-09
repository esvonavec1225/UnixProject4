/*
 * Copyright (c) 2016
 *	The University of Akron
 *
 * Author
 *	Eric Svonavec
 *	eps8@zips.uakron.edu
 */

#include <stdio.h>
#include <fcntl.h>

int main( int argc, char *argv[] )  {
	if (argc <= 1) {
		printf("At least one parameter (filename) is required\n");
	}
	else {
		char *filename = argv[argc-1];

		int open_int = open(filename, O_RDONLY);
		if (open_int == -1) {
			printf("Unable to open file: %s\n", filename);
			return 1;
		}
	}
	return 1;
}
