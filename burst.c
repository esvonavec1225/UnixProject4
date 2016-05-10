/*
 * Copyright (c) 2016
 *	The University of Akron
 *
 * Author
 *	Eric Svonavec
 *	eps8@zips.uakron.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192

int main( int argc, char *argv[] )  {
	int c;
	int help = 0;
	int version = 0;
	int lines_per_file = 500;
	if (argc <= 1) {
		printf("At least one parameter (filename) is required\n");
	}
	else {
		opterr = 0;
		while ((c = getopt(argc, argv, "hvl:")) != -1) {
			switch(c) {
				case 'h':
					help = 1;
					break;
				case 'l':
					lines_per_file = atoi(optarg);
					break;
				case 'v':
					version = 1;
					break;
				default:
					printf("Invalid Parameter(s)");
			}
		}

		if (help == 1) {
			printf("HELP!\n");
			return 1;
		}
		else if (version == 1) {
			printf("burst 1.0\nCopyright (C) 2016 The University of Akron.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Eric Svonavec.\n");
			return 1;
		}

		char *filename = argv[argc-1];

		int opener = open(filename, O_RDONLY);
		if (opener == -1) {
			printf("Unable to open file: %s\n", filename);
			return 1;
		}

		char original_new_filename[100];
		char *new_filename = malloc(strlen(filename) + 1);
		char *dot;
		char *ext = strrchr(filename, '.');
		strcpy(new_filename, filename);
		dot = strrchr(new_filename, '.');
		*dot = '\0';
		char new_ext[100];		
		strcpy(original_new_filename, new_filename);
		strcpy(new_ext, ext);
		strcat(new_filename,"1");
		strcat(new_filename,new_ext);

		int curline = 0;
		int curfile = 0;
		char buffer[BUFFER_SIZE];
		int writer_file;
		ssize_t writer;
		ssize_t reader;
		char content[BUFFER_SIZE];
                int characters = 0;
		while ((reader = read(opener, &buffer, BUFFER_SIZE)) > 0) {
			for (int i = 0; i < reader; ++i) {
				content[characters] = buffer[i];
				++characters;
				if (buffer[i]=='\n') {
					curline++;
					if (curline == lines_per_file) {
						curline = 0;
						++curfile;
						strcpy(new_filename,original_new_filename);
						char buf[12];
						sprintf(buf, "%d", curfile);
						strcat(new_filename,buf);
				                strcat(new_filename,new_ext);
						writer_file = open(new_filename, O_WRONLY | O_CREAT, 0644);
						writer = write(writer_file, &content, characters);
						memset(&content[0], 0, sizeof(content));
						characters = 0;
					}
				}
			}
		}
		if (characters > 0) {
			 ++curfile;
                         strcpy(new_filename,original_new_filename);
                         char buf[12];
                         sprintf(buf, "%d", curfile);
                         strcat(new_filename,buf);
                         strcat(new_filename,new_ext);
                         writer_file = open(new_filename, O_WRONLY | O_CREAT, 0644);
                         writer = write(writer_file, &content, characters);	
		}
	}
	return 1;
}
