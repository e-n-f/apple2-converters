#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
istxt (FILE *f)
{
	int nch = 0;
	int ncr = 0;
	int c;

	while ((c = getc (f)) != EOF) {
		nch++;

		if (c == '\r')
			ncr++;
		if (c == '\n')
			return 0;
	}

	if (ncr > (nch / 80))
		return 1;
	else
		return 0;
}

int
main (int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++) {
		FILE *f = fopen (argv[i], "r");
		int iftxt;

		if (!f) {
			fprintf (stderr, "%s: %s: %s\n", argv[0], argv[i],
				 strerror (errno));
			continue;
		}

		if (istxt (f)) {
			char *ofn;
			FILE *of;
			int c;

			ofn = malloc (strlen (argv[i]) + 4 + 1);
			if (!ofn)
				abort();

			sprintf (ofn, "%s.txt", argv[i]);
			printf ("%s\n", ofn);

			of = fopen (ofn, "w");
			if (!of) {
				fprintf (stderr, "%s: %s: %s\n", argv[0],
					 ofn, strerror (errno));
				fclose (f);
				free (ofn);
				continue;
			}

			rewind (f);
			while ((c = getc (f)) != EOF) {
				if (c == '\r')
					putc ('\n', of);
				else
					putc (c, of);
			}

			fclose (of);
			free (ofn);
		}

		fclose (f);
	}
}
