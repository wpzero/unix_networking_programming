include ../Make.defines

PROGS =	tcpserv01 \
	tcpcli01

all:	${PROGS}

tcpserv01:	tcpserv01.o
		${CC} ${CFLAGS} -o $@ tcpserv01.o ${LIBS}

tcpcli01:	tcpcli01.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
