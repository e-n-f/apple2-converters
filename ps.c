#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void
usage (char *s)
{
	fprintf (stderr, "Usage: %s [file]\n");
}

void
process (FILE *f)
{
	int x, y, z;

	printf ("/* XPM */\n");
	printf ("static char *apple2[] = {\n");
	printf ("\"88 52 2 1\",\n");
	printf ("\"0 c #000000\",\n");
	printf ("\"1 c #ffffff\",\n");
	
	for (y = 0; y < 52; y++) {
		printf ("\"");

		for (x = 0; x < 11; x++) {
			int c = getc (f);

			for (z = 7; z >= 0; z--)
				if (c & (1 << z))
					putchar ('0');
				else
					putchar ('1');
		}

		printf ("\",\n");
	}

	printf ("};\n");
}

int
main (int argc, char **argv)
{
	if (argc < 2) {
		process (stdin);
	} else if (argc == 2) {
		FILE *f = fopen (argv[1], "r");

		if (f) {
			process (f);
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
