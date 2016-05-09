/*
 * Copyright (c) 2016
 *	The University of Akron
 *
 * Author
 *	Eric Svonavec
 *	eps8@zips.uakron.edu
 */

#include <stdio.h>

int main( int argc, char *argv[] )  {
	if (argc <= 1) {
		printf("At least one parameter (filename) is required\n");
	}
	else {
		char *filename = argv[argc-1];
		printf("Filename: %s\n", filename);
	}
	return 1;
}
