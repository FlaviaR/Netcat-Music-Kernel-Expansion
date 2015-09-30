/* Usage:
 * $ git clone https://github.com/usrbinnc/netcat-cpi-kernel-module.git
 * $ gcc array-to-ogg.c -o array-to-ogg
 * $ ./array-to-ogg netcat-cpi-kernel-module/tracks/*data.h
 * $ ls netcat-cpi-kernel-module/tracks/*data.ogg
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>

char *program_name;

enum {
	TOK_MAX = 100,
};

char tok[TOK_MAX];
int tokp;

void print_usage_exit(void)
{
	fprintf(stderr, "usage: %s FILE [FILE]...\n", program_name);
	exit(2);
}

int main(int argc, char **argv)
{
	char *filename_out;
	FILE *fin, *fout;
	int i;

	program_name = basename(argv[0]);

	if (argc <= 1)
		print_usage_exit();

	for (i = 1; i < argc; i++) {
		char *filename, *t;
		int c, len;
		filename = argv[i];
		fin = fopen(filename, "r");
		if (!fin) {
			fprintf(stderr, "Can't open \"%s\"\n", filename);
			continue;
		}
		len = strlen(filename);
		filename_out = malloc(len + 3); /* 3: strlen("ogg") */
		memcpy(filename_out, filename, len + 1);
		t = strrchr(filename_out, '.');
		if (!t) {
			fprintf(stderr, "Filename with no extension \"%s\"\n", filename);
			continue;
		}
		memcpy(t+1, "ogg", 4);
		fout = fopen(filename_out, "w");
		assert(fout);
		
		while ((c = fgetc(fin)) != EOF) {
			if (('0' <= c && c <= '9') || (c == 'x' || c == 'X') ||
					('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')) {
				assert(tokp < TOK_MAX);
				tok[tokp++] = c;
			} else {
				char v;
				if (tokp <= 0)
					continue;
				assert(tokp < TOK_MAX);
				tok[tokp] = '\0';
				v = strtol(tok, &t, 0);
				fputc(v, fout);
				tokp = 0;
			}
		}
		fclose(fout);
		fclose(fin);
	}
	return 0;
}
