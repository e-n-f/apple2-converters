#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char **av;

void
usage (char *s)
{
	fprintf (stderr, "Usage: %s [file]\n");
}

char *
token (int c)
{
	static char s[200];

	switch (c) {
		case 0: return "0";
		case 128: return "long";
		case 129: return "for";
		case 131: return "plot131 ";
		case 132: return "cls";
		case 133: return "else";
		case 135: return "next";
		case 137: return "input";
		case 138: return "dim";
		case 141: return "goto";
		case 143: return "if";
		case 147: return "rem";
		case 159: return "end";
		case 162: return "open";
		case 171: return "plot171 ";
		case 176: return "def";
		case 178: return "print";
		case 180: return "do";
		case 181: return "until";
		case 185: return "while";
		case 189: return "line";
		case 186: return "wend";
		case 190: return "fn";
		case 199: return "peek";
		case 204: return "to";
		case 207: return "and";
		case 205: return "then";
		case 206: return "step";
		case 213: return "len";
		case 217: return "int";
		case 238: return "chr$";
		case 248: return "inkey$";
		case 249: return "left$";
		case 251: return "mid$";
		default:
			  sprintf (s, "[%d]", c);
			  return s;
	}
}

void
process (FILE *f, char *name)
{
	while (1) {
		int a, b;
		int c;

		a = getc (f);  /* line number */
		b = getc (f);  /* high half of line number */

		if (a == EOF || b == EOF)
			return;

		printf ("%05d ", a + 256 * b);

		a = getc (f);  /* line length */

		if (a == EOF)
			return;

		while (a-- > 1 && (c = getc (f)) != EOF) {
			if (c > 127)
				printf ("%s", token (c));
			else
				printf ("%c", c);
		}

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
