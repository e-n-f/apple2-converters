#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char **av;

void
usage (char *s)
{
	fprintf (stderr, "Usage: %s [file]\n");
}

void
process (FILE *f, char *name)
{
	int a, b;

	a = getc (f);  /* load address */
	b = getc (f);  /* high half of load address */

	if (a == EOF || b == EOF)
		return;

	a = getc (f);  /* file length */
	b = getc (f);  /* high half of file length */

	if (a == EOF || b == EOF)
		return;

	a = getc (f);  /* language */
	b = getc (f);  /* revision */

	if (a == EOF || b == EOF)
		return;

	if (a != 1) {
		fprintf (stderr, "%s: %s: not an ORCA/EZ file\n", *av, name);
		return;
	}

	printf ("; revision %d\n", b);

	a = getc (f);  /* unused date */
	b = getc (f);  /* unused date */

	if (a == EOF || b == EOF)
		return;

	while (1) {
		int c;
		int i, j;
		int len;

		len = getc (f);
		if (len == 0 || len == EOF)
			break;

		for (i = 0; i < len - 2; i++) {
			c = getc (f);

			if (c < 128)
				for (j = 0; j < c; j++)
					printf (" ");
			else
				printf ("%c", c & 0x7f);
		}

		len = getc (f);
		printf ("\n");
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
