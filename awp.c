#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char **av;

void
usage (char *s)
{
	fprintf (stderr, "Usage: %s [file]\n", s);
}

void
process (FILE *f, char *name)
{
	unsigned char header[300];
	unsigned char rec[2];

	printf("<html>\n");
	printf("<pre>\n");

	int bold = 0, under = 0, sub = 0, super = 0;

	if (fread (header, sizeof (unsigned char), 300, f) != 300) {
		fprintf (stderr,
			 "%s: %s: could not read word processor header\n",
			 av[0], name);
		return;
	}

	if (header[4] != 79) {
		fprintf (stderr,
			 "%s: %s: is not an AppleWorks word processor file\n",
			 av[0], name);
		return;
	}

	while (fread (rec, sizeof (unsigned char), 2, f) == 2) {
		if (rec[1] == 0) {
			unsigned char *line;
			int i;

			line = malloc (sizeof (unsigned char) * rec[0]);
			if (!line) {
				fprintf (stderr, "%s: %s: out of memory\n",
					 av[0], name);
				exit (EXIT_FAILURE);
			}

			if (fread (line, sizeof (unsigned char),
				   rec[0], f) != rec[0]) {
				fprintf (stderr, "%s: %s: read error\n",
					 av[0], name);
				return;
			}

			for (i = 0; i < (line[0] & 0x7f); i++)
				printf (" ");

			for (i = 2; i < (line[1] & 0x7f) + 2; i++) {
				if (line[i] == 1 && bold == 0) {
					printf("<b>");
					bold++;
				} else if (line[i] == 2 && bold > 0) {
					printf("</b>");
					bold--;
				} else if (line[i] == 3 && super == 0) {
					printf("<sup>");
					super++;
				} else if (line[i] == 4 && super > 0) {
					printf("</sup>");
					super--;
				} else if (line[i] == 5 && sub == 0) {
					printf("<sub>");
					sub++;
				} else if (line[i] == 6 && sub > 0) {
					printf("</sub>");
					sub--;
				} else if (line[i] == 7 && under == 0) {
					printf("<u>");
					under++;
				} else if (line[i] == 8 && under > 0) {
					printf("</u>");
					under--;
				} else if (line[i] == 11) {
					printf ("&nbsp;");  /* sticky space */
				} else if (line[i] == 0x16) {
					printf ("&nbsp;");  /* tab */
				} else if (line[i] == 0x17) {
					printf ("&nbsp;");  /* tab spacer */
				} else if (line[i] >= 32) {
					if (line[i] == '&') {
						printf("&amp;");
					} else if (line[i] == '<') {
						printf("&lt;");
					} else if (line[i] == '>') {
						printf("&gt;");
					} else {
						printf ("%c", line[i]);
					}
				}
			}

			free (line);
			printf ("\n");

			if (line[1] & 0x80) {
				while (bold > 0) {
					printf("</b>");
					bold--;
				}
				while (under > 0) {
					printf("</u>");
					under--;
				}
				while (super > 0) {
					printf("</sup>");
					super--;
				}
				while (sub > 0) {
					printf("</sub>");
					sub--;
				}
			}
		} else if (rec[1] = 0xd0) {
			int i;

			for (i = 0; i < rec[0]; i++)
				printf (" ");

			printf ("\n");

			while (bold > 0) {
				printf("</b>");
				bold--;
			}
			while (under > 0) {
				printf("</u>");
				under--;
			}
			while (super > 0) {
				printf("</sup>");
				super--;
			}
			while (sub > 0) {
				printf("</sub>");
				sub--;
			}

			bold = under = super = sub = 0;
		}
	}

	printf("</pre>");
	printf("</html>");
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
