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
	printf ("\"280 192 6 1\",\n");
	printf ("\"0 c #000000\",\n");
	printf ("\"1 c #ffffff\",\n");

	printf ("\"g c #55ff55\",\n");

	printf ("\"p c #b055b0\",\n");

	printf ("\"r c #ff5555\",\n");

	printf ("\"b c #5555ff\",\n");

	for (maj = 0; maj < 24; maj++) {
		for (min = lines[maj]; min < lines[maj] + 8192; min += 1024) {
			int x;
			char bits[280];

			printf ("\"");

			for (x = 0; x < 280; x++) {
				bits[x] = (buf[min + x/7] & (1 << (x%7))) != 0;
			}

#define HIGH(x) (buf[min + (x)/7] & (1 << 7))

			if (bits[0] != bits[1]) {
				if (HIGH (0)) {
					if (bits[0])
						printf ("b");
					else
						printf ("r");
				} else {
					if (bits[0])
						printf ("p");
					else
						printf ("g");
				}
			} else {
				if (bits[0])
					printf ("1");
				else
					printf ("0");
			}

			for (x = 1; x < 279; x++) {
				if (bits[x] != bits[x-1] &&
				    bits[x] != bits[x+1]) {
					if (x % 2 == 0) {
						if (HIGH (x)) {
							if (bits[x])
								printf ("b");
							else
								printf ("r");
						} else {
							if (bits[x])
								printf ("p");
							else
								printf ("g");
						}
					} else {
						if (HIGH (x)) {
							if (bits[x])
								printf ("r");
							else
								printf ("b");
						} else {
							if (bits[x])
								printf ("g");
							else
								printf ("p");
						}
					}
				} else {
					if (bits[x])
						printf ("1");
					else
						printf ("0");
				}
			}

			if (bits[278] != bits[279]) {
				if (HIGH (279)) {
					if (bits[279])
						printf ("r");
					else
						printf ("b");
				} else {
					if (bits[279])
						printf ("g");
					else
						printf ("p");
				}
			} else {
				if (bits[279])
					printf ("1");
				else
					printf ("0");
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
