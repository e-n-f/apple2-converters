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
	int maj;
	int min;

	static int lines[] = {
		0x000,
		0x080,
		0x100,
		0x180,
		0x200,
		0x280,
		0x300,
		0x380,
		0x028,
		0x0a8,
		0x128,
		0x1a8,
		0x228,
		0x2a8,
		0x328,
		0x3a8,
		0x050,
		0x0d0,
		0x150,
		0x1d0,
		0x250,
		0x2d0,
		0x350,
		0x3d0
	};

	unsigned char buf[8192];

	fread (buf, sizeof (unsigned char), 8192, f);

	printf ("/* XPM */\n");
	printf ("static char *apple2[] = {\n");
	printf ("\"280 192 2 1\",\n");
	printf ("\"0 c #000000\",\n");
	printf ("\"1 c #ffffff\",\n");

	for (maj = 0; maj < 24; maj++) {
		for (min = lines[maj]; min < lines[maj] + 8192; min += 1024) {
			int x;

			printf ("\"");

			for (x = min; x < min + 40; x++) {
				int bit;

				for (bit = 0; bit < 7; bit++) {
					if (buf[x] & (1 << bit))
						printf ("1");
					else
						printf ("0");
				}
			}

			printf ("\",\n");
		}
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
