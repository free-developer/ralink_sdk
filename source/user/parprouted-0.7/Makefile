EXTRA_CFLAGS = 

CFLAGS = -g -O2 -Wall $(EXTRA_CFLAGS)
# For ARM:
# CFLAGS =  -Wall $(EXTRA_CFLAGS)
LDFLAGS = 
OBJS = parprouted.o arp.o

LIBS = -lpthread

all: parprouted

romfs: all
	$(ROMFSINST) parprouted /sbin/parprouted

clean:
	rm -f $(OBJS) parprouted core

parprouted:	${OBJS}
	${CC} -g -o parprouted ${OBJS} ${CFLAGS} ${LDFLAGS} ${LIBS}

parprouted.8:	parprouted.pod
	pod2man --section=8 --center="Proxy ARP Bridging Daemon" parprouted.pod --release "parprouted" --date "`date '+%B %Y'`" > parprouted.8

parprouted.o : parprouted.c parprouted.h

arp.o : arp.c parprouted.h
