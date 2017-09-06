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
	static char s[200];

	switch (c) {
		case 0: return "0";
		case 128: return "LONG";
		case 129: return "FOR";
		case 130: return "COLOR";
		case 131: return "PLOT";
		case 132: return "CLS";
		case 133: return "XELSE";
		case 134: return "RANDOM";
		case 135: return "NEXT";
		case 136: return "DATA";
		case 137: return "INPUT";
		case 138: return "DIM";
		case 139: return "READ";
		case 140: return "LET";
		case 141: return "GOTO";
		case 142: return "RUN";
		case 143: return "IF";
		case 144: return "RESTORE";
		case 145: return "GOSUB";
		case 146: return "RETURN";
		case 147: return "REM";
		case 148: return "STOP";
		case 149: return "ELSE";
		case 150: return "TRON";
		case 151: return "TROFF";
		case 152: return "DEFSTR";
		case 153: return "DEFINT";
		case 154: return "DEFSNG";
		case 155: return "DEFDBL";
		case 156: return "FILL";
		case 157: return "SWAP";
		case 158: return "SOUND";
		case 159: return "END";
		case 160: return "OUTSLOT";
		case 161: return "ON";
		case 162: return "OPEN";
		case 163: return "ROUTE";
		case 164: return "RATIO";
		case 165: return "CIRCLE";
		case 166: return "CLOSE";
		case 167: return "MACHLG";
		case 168: return "LOCATE";
		case 169: return "WIDTH";
		case 170: return "KILL";
		case 171: return "BOX";
		case 172: return "RECORD";
		case 173: return "CALL";
		case 174: return "MODE";
		case 175: return "LPRINT";
		case 176: return "DEF";
		case 177: return "POKE";
		case 178: return "PRINT";
		case 179: return "WRITE";
		case 180: return "DO";
		case 181: return "UNTIL";
		case 182: return "DELAY";
		case 183: return "RENAME";
		case 184: return "CLEAR";
		case 185: return "WHILE";
		case 186: return "WEND";
		case 187: return "PAGE";
		case 188: return "INDEXF";
		case 189: return "LINE";
		case 190: return "FN";
		case 191: return "USR";
		case 192: return "ERROR";
		case 193: return "VARPTR";
		case 194: return "INSTR";
		case 195: return "POINT";
		case 196: return "MEM";
		case 197: return "NOT";
		case 198: return "MAYBE";
		case 199: return "PEEK";
		case 200: return "SPC";
		case 201: return "USING";
		case 202: return "TAB";
		case 203: return "WORD";
		case 204: return "TO";
		case 205: return "THEN";
		case 206: return "STEP";
		case 207: return "AND";
		case 208: return "OR";
		case 209: return "XOR";
		case 210: return "MOD";
		case 211: return "FRAC";
		case 212: return "FIX";
		case 213: return "LEN";
		case 214: return "VAL";
		case 215: return "ASC";
		case 216: return "SGN";
		case 217: return "INT";
		case 218: return "ABS";
		case 219: return "INSLOT";
		case 220: return "POS";
		case 221: return "SQR";
		case 222: return "RND";
		case 223: return "LOG";
		case 224: return "EXP";
		case 225: return "COS";
		case 226: return "SIN";
		case 227: return "TAN";
		case 228: return "ATN";
		case 229: return "CVI";
		case 230: return "CVB";
		case 231: return "MOUSE";
		case 232: return "REC";
		case 233: return "LOC";
		case 234: return "LOF";
		case 235: return "PSTR$";
		case 236: return "MKI$";
		case 237: return "MKB$";
		case 238: return "CHR$";
		case 239: return "HEX$";
		case 240: return "OCT$";
		case 241: return "BIN$";
		case 242: return "UNS$";
		case 243: return "STR$";
		case 244: return "INDEX$";
		case 245: return "ERRMSG$";
		case 246: return "TIME$";
		case 247: return "DATE$";
		case 248: return "INKEY$";
		case 249: return "LEFT$";
		case 250: return "RIGHT$";
		case 251: return "MID$";
		case 252: return "STRING$";
		case 253: return "SPACE$";
		case 254: return "UCASE$";
		case 255: return "* END *";
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
