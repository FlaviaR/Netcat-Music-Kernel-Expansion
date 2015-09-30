/* Usage:
 * $ g++ ogg2Array.c -o ogg2Array
 * $ ./ogg2Array netcat-cpi-kernel-module/tracks/*data.h
 * $ ls netcat-cpi-kernel-module/tracks/*data.ogg
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>
#include <string>

using namespace std;

char *program_name;

enum {
	TOK_MAX = 100,
};

char tok[TOK_MAX];

void print_usage_exit(void)
{
	fprintf(stderr, "usage: %s FILE [FILE]...\n", program_name);
	exit(2);
}

/**
 *   Non-standard function.
 *   Converts a long value to a null-terminated string using
 *   the specified base and stores the result in the array given by str parameter.
 *   <p>
 *   If base is 10 and value is negative, the resulting string is preceded with a minus sign (-).
 *   With any other base, value is always considered unsigned.
 *   <p>
 *   str should be an array long enough to contain any possible value:
 *   (sizeof(int)*8+1) for radix=2, i.e. 17 bytes in 16-bits platforms and 33 in 32-bits platforms.
 *
 *   @param num    Value to be converted to a string.
 *   @param str    Array in memory where to store the resulting null-terminated string.
 *   @param base   Numerical base used to represent the value as a string, between 2 and 36,
 *                 where 10 means decimal base, 16 hexadecimal, 8 octal, and 2 binary.
 *   @return A pointer to the resulting null-terminated string, same as parameter str.
 */
char* ltoa(unsigned char num, char* str, int base) {
    if ( base < 2 || base > 36 ) return NULL; // starting at 10 and using 26 letters equals 36
    string bStr = (!num)? "0": "";
    int n = abs(num);
    char c;
    int nb;
    while (n != 0) {
       nb = n%base;
       if ( base > 10 && nb > 9 )
            c = (char)(nb-10 + 'A');
       else
            c = (char)(nb + '0');
       bStr = c + bStr;
       n /= base;
    }
    if ( num < 0 ) bStr = '-' + bStr;
	if ( bStr.length() < 2 )
		bStr = '0' + bStr;
    bStr = "0x" + bStr;	
    if ( str )
         strcpy ( str, bStr.c_str() );
    return str;
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
		fin = fopen(filename, "rb");
		if (!fin) {
			fprintf(stderr, "Can't open \"%s\"\n", filename);
			continue;
		}
		len = strlen(filename);
		filename_out = (char*)malloc(len + 2); /* 1: strlen("h") */
		memcpy(filename_out, filename, len + 2);
		t = strrchr(filename_out, '.');
		if (!t) {
			fprintf(stderr, "Filename with no extension \"%s\"\n", filename);
			continue;
		}
		memcpy(t+1, "h", 2);
		fout = fopen(filename_out, "w");
		assert(fout);

		printf ( "%s %s\n", filename, t );	
		unsigned char n;	// I just want two bytes
		while (fread(&n,sizeof(unsigned char),1,fin) > 0) {
			ltoa(n, tok, 16);
			// printf ( "%lu %s\n", n, tok );
			fprintf(fout, "%s,\n", tok);			
		}
		fclose(fout);
		fclose(fin);
	}
	return 0;
}
