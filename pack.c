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
	int c;

	while ((c = getc (f)) != EOF) {
		if (c == 0xfe) {
			int len;
			int i;

			len = getc (f);
			c = getc (f);

			if (len != EOF)
				for (i = 0; i < len; i++)
					putchar (c);
		} else
			putchar (c);
	}
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
