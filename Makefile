HGROBJS = a2pic.o
AWPOBJS = awp.o
FPOBJS = fp.o
ORCAOBJS = orca.o
CRYPTOBJS = crypto.o
PACKOBJS = pack.o
PSOBJS = ps.o
TXTOBJS = txt.o
ZBOBJS = zb.o

all: a2pic awp2txt fpbasic2txt orca2txt crypto unpack ps2xpm a2txt zb2txt

a2pic: $(HGROBJS)
	$(CC) -o a2pic $(HGROBJS)

awp2txt: $(AWPOBJS)
	$(CC) -o awp2txt $(AWPOBJS)

fpbasic2txt: $(FPOBJS)
	$(CC) -o fpbasic2txt $(FPOBJS)

orca2txt: $(ORCAOBJS)
	$(CC) -o orca2txt $(ORCAOBJS)

crypto: $(CRYPTOBJS)
	$(CC) -o crypto $(CRYPTOBJS)

unpack: $(PACKOBJS)
	$(CC) -o unpack $(PACKOBJS)

ps2xpm: $(PSOBJS)
	$(CC) -o ps2xpm $(PSOBJS)

a2txt: $(TXTOBJS)
	$(CC) -o a2txt $(TXTOBJS)

zb2txt: $(ZBOBJS)
	$(CC) -o zb2txt $(ZBOBJS)
