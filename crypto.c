#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char **av;

void
usage (char *s)
{
	fprintf (stderr, "Usage: %s [file]\n", s);
}

void
process (FILE *f, char *name)
{
	int a;
	int b;  /* lo */
	int c;  /* hi */
	int d;  /* code */

	int i;
	unsigned char data[8192];

	for (i = 0; i < 8192; i++) {
		data[i] = getc (f);
	}

	b = 0;
	c = 0x20;
	d = 255 - 0x8c;

#define BC (256*c + b - 8192)

loop:
	a = data[BC];
	a ^= d;
	data[BC] = a;
	d = (d + 1) & 0xff;
	a = d;
	a ^= c;
	d = a;
	b = (b + 1) & 0xff;
	if (b != 0)
		goto noinc;
	c++;
	a = c;
	if (a == 0x40)
		goto done;
noinc:
	goto loop;

done:

	for (i = 0; i < 8192; i++)
		putchar (data[i]);
}

int
main (int argc, char **argv)
{
	av = argv;

	if (argc < 2) {
		process (stdin, "standard input");
	} else if (argc == 2) {
		FILE *f = fopen (argv[1], "r");

		if (f) {
			process (f, argv[1]);
			fclose (f);
		} else {
			fprintf (stderr, "%s: %s: %s\n", argv[0], argv[1],
				 strerror (errno));
			exit (EXIT_FAILURE);
		}
	} else {
		usage (argv[0]);
		exit (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
