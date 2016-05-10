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
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 8192

int main( int argc, char *argv[] )  {
	int c;
	static int help = 0;
	static int version = 0;
	int lines_per_file = 500;
	char output_filename[100];
	if (argc <= 1) {
		printf("At least one parameter (filename) is required\n");
	}
	else {
		opterr = 0;
		while (1) {
			static struct option long_options[] = {
				{"version", no_argument, &version, 1},
				{"help", no_argument, &help, 1},
				{"lines", required_argument, 0, 'l'},
				{"output", required_argument, 0, 'o'},
				{0, 0, 0, 0}
			};

			int option_index = 0;

			c = getopt_long(argc, argv, "o:l:", long_options, &option_index);
			if (c == -1)
				break;

			switch(c) {
				case 'l':
					lines_per_file = atoi(optarg);
					break;
				case 'o':
					strcpy(output_filename,optarg);
					break;
			}
		}

		if (help == 1) {
			printf("Usage: burst [OPTION]... [FILE]...\nburst is high level shell program for splitting a file into segments.\nThe result will be multiple files with the max number of lines as defined by the user (default is 500).\n\nMandatory arguments to long options are mandatory for short options too.\n	-l, --lines	specify the maximum amount of lines per output file\n	-o, --output	specify output file name\n	    --help  	display this help and exit\n	    --version	display the version information and exit\n");
			return 1;
		}
		else if (version == 1) {
			printf("burst 1.0\nCopyright (C) 2016 The University of Akron.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Eric Svonavec.\n");
			return 1;
		}

		char *filename = argv[argc-1];

		 if (strpbrk(filename, ".") == 0) {
                        printf("Input filename must contain a period\n");
                        return 0;
                }

		int opener = open(filename, O_RDONLY);
		if (opener == -1) {
			printf("Unable to open file: %s\n", filename);
			return 1;
		}
		
		if (output_filename[0] == '\0') {
			strcpy(output_filename, filename);
		}
		
		if (strpbrk(output_filename, ".") == 0) {
			printf("Output filename must contain a period\n");
			return 0;
		}

		char original_new_filename[100];
		char *new_filename = malloc(strlen(output_filename) + 1);
		char *dot;
		char *ext = strrchr(output_filename, '.');
		strcpy(new_filename, output_filename);
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

