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

char *
token (int c)
{
	switch (c) {
		case 128: return "end";
		case 129: return "for";
		case 130: return "next";
		case 131: return "data";
		case 132: return "input";
		case 133: return "del";
		case 134: return "dim";
		case 135: return "read";
		case 136: return "gr";
		case 137: return "text";
		case 138: return "pr#";
		case 139: return "in#";
		case 140: return "call";
		case 141: return "plot";
		case 142: return "hlin";
		case 143: return "vlin";
		case 144: return "hgr2";
		case 145: return "hgr";
		case 146: return "hcolor=";
		case 147: return "hplot";
		case 148: return "draw";
		case 149: return "xdraw";
		case 150: return "htab";
		case 151: return "home";
		case 152: return "rot=";
		case 153: return "scale=";
		case 154: return "shload";
		case 155: return "trace";
		case 156: return "notrace";
		case 157: return "normal";
		case 158: return "inverse";
		case 159: return "flash";
		case 160: return "color=";
		case 161: return "pop";
		case 162: return "vtab";
		case 163: return "himem:";
		case 164: return "lomem:";
		case 165: return "onerr";
		case 166: return "resume";
		case 167: return "recall";
		case 168: return "store";
		case 169: return "speed=";
		case 170: return "let";
		case 171: return "goto";
		case 172: return "run";
		case 173: return "if";
		case 174: return "restore";
		case 175: return "&";
		case 176: return "gosub";
		case 177: return "return";
		case 178: return "rem";
		case 179: return "stop";
		case 180: return "on";
		case 181: return "wait";
		case 182: return "load";
		case 183: return "save";
		case 184: return "def";
		case 185: return "poke";
		case 186: return "print";
		case 187: return "cont";
		case 188: return "list";
		case 189: return "clear";
		case 190: return "get";
		case 191: return "new";
		case 192: return "tab";
		case 193: return "to";
		case 194: return "fn";
		case 195: return "spc(";
		case 196: return "then";
		case 197: return "at";
		case 198: return "not";
		case 199: return "step";
		case 200: return "+";
		case 201: return "-";
		case 202: return "*";
		case 203: return "/";
		case 204: return "^";
		case 205: return "and";
		case 206: return "or";
		case 207: return ">";
		case 208: return "=";
		case 209: return "<";
		case 210: return "sgn";
		case 211: return "int";
		case 212: return "abs";
		case 213: return "usr";
		case 214: return "fre";
		case 215: return "scrn";
		case 216: return "pdl";
		case 217: return "pos";
		case 218: return "sqr";
		case 219: return "rnd";
		case 220: return "log";
		case 221: return "exp";
		case 222: return "cos";
		case 223: return "sin";
		case 224: return "tan";
		case 225: return "atn";
		case 226: return "peek";
		case 227: return "len";
		case 228: return "str$";
		case 229: return "val";
		case 230: return "asc";
		case 231: return "chr$";
		case 232: return "left$";
		case 233: return "right$";
		case 234: return "mid$";
		default:  return "???";
	}
}

void
process (FILE *f, char *name)
{
	int oldptr = 0;

	while (1) {
		int a, b;
		int c;

		a = getc (f);  /* useless (to us) pointer to next line */
		b = getc (f);  /* high half of pointer */

		if (a == EOF || b == EOF) {
			fprintf(stderr, "Unexpected EOF\n");
			exit(EXIT_FAILURE);
		}
		if (a == 0 && b == 0) {
			int c = getc(f);
			if (c != EOF) {
				fprintf(stderr, "More data follows 0/0 EOF tag\n");
				exit(EXIT_FAILURE);
			}
			return;
		}

		int ptr = a + 256 * b;
		if (ptr < 2048) {
			fprintf(stderr, "Impossible pointer %d\n", ptr);
			exit(EXIT_FAILURE);
		}
		if (ptr < oldptr) {
			fprintf(stderr, "Pointer %d < old %d\n", ptr, oldptr);
			exit(EXIT_FAILURE);
		}
		oldptr = ptr;

		a = getc (f);  /* line number */
		b = getc (f);  /* high half of line number */

		if (a == EOF || b == EOF) {
			fprintf(stderr, "Unexpected EOF\n");
			exit(EXIT_FAILURE);
		}

		printf ("%d ", a + 256 * b);

		while ((c = getc (f)) != EOF) {
			if (c == 0)
				break;

			if (c > 127)
				printf (" %s ", token (c));
			else
				printf ("%c", c);
		}

		if (c == EOF) {
			fprintf(stderr, "Unexpected EOF\n");
			exit(EXIT_FAILURE);
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
