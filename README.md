apple2-converters
=================

These are some programs I wrote many years ago to convert Apple II
binary file formats to things that are usable on current systems.

They are:

* [a2pic.c](a2pic.c): Convert hi-res images to XPM. Sorry, it seemed
  like a future-proof format choice at the time.

* [awp.c](awp.c): Convert AppleWorks Word Processor files to HTML

* [fp.c](fp.c): Convert AppleSoft BASIC source files to text

* [orca.c](orca.c): Convert ORCA/EZ assembler source files to text

* [pack.c](pack.c): Decompress run-length-encoded compressed images

* [ps.c](ps.c): Convert small images from The Print Shop to XPM

and less usefully:

* [crypto.c](crypto.c): Superficially obscure images

* [txt.c](txt.c): Convert CR-delimited text files to LF-delimited

* [zb.c](zb.c): Convert ZBASIC source files to text. This isn't complete
  and doesn't know what a lot of the tokens are.
